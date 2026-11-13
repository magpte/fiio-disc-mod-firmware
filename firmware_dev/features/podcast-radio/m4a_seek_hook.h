#ifndef DISC_M4A_SEEK_HOOK_H
#define DISC_M4A_SEEK_HOOK_H

extern char *strstr(const char *, const char *);
extern char *strchr(const char *, int);
extern long time(long *);

/* ==============================================================================
 * m4a_seek_hook.h - FiiO mq_player M4A / DLNA seek repair (1.3.0)
 *
 * VERIFIED ROOT CAUSE (static disassembly + 1.2.23 field logs, see
 * AI_HANDOVER.md):
 *
 * 1) mq_player DLNA seek thread (0x5e1f14) does:
 *        nanosleep(500ms)
 *        s0 = *(int *)0x89acb8          ; target seconds, written by 0x5e2034
 *        t9 = obj->[44]                 ; obj = *(void **)0x89acf8
 *        a0 = s0 * 1000                 ; milliseconds (sll/subu/sll/addu/sll)
 *        jalr t9                        ; return address 0x5e1f94
 *    a0 IS correct; $f12 is not involved at this stage.
 *
 * 2) vtable[44] = 0x458e64 -> 0x4586cc(int ms). That function clamps:
 *        0x458858: lw   a1,52(core)     ; core = *(0x83dfc4)
 *        0x458864: slt  v1,a1,s2
 *        0x458868: movn s2,a1,v1        ; if ([core+52] < ms) ms = [core+52]
 *    [core+52] is the track duration in SECONDS (matches the UPnP
 *    TrackDuration the Go side logs as dur=3409/4954). So every target above
 *    "duration seconds" is clamped to duration-in-seconds, i.e. ~3.4 s / ~5 s,
 *    which is why playback restarts near the file head. Field log:
 *        entry sec=1487 -> in=3409.0 (duration), fixed=0
 *
 * 3) The value is then forwarded unchanged:
 *        0x458884: beqz v1,0x458b14 ; delay slot mtc1 s2,$f0
 *        0x458b14: jal 0x44c994     ; delay slot cvt.d.w $f12,$f0
 *    and 0x44c994 is `mfc1 a2,$f12; mfhc1 a3,$f12; j AudioCodecSeek@plt`,
 *    PLT 0x825f70 -> GOT 0x008380b4 -> libfiio_decoder.so + 0x63a0.
 *    libfiio_decoder's AudioCodecSeek takes the double BITS in a2:a3:
 *        63a0: sltiu v0,a0,8
 *        6414: mtc1  a2,$f20
 *        6428: mthc1 a3,$f20
 *        6458: ldc1  $f0,-21120(v0)   ; 1000.0 (rodata 0xad80)
 *        6464: mul.d $f20,$f20,$f0    ; ms -> us
 *    so the parameter is milliseconds in an o32 a2:a3 pair, not a hard-float
 *    $f12/$f13 double. Any hook must use the same convention on BOTH sides.
 *
 * 4) 1.2.22's hook was inert for three independent reasons:
 *      - no "[M4A_SEEK_HOOK] Installed:" line ever appeared (install gate /
 *        early boot log path), so the GOT was never patched;
 *      - it read *(int *)0x89ad08, which nothing ever writes; the live target
 *        is 0x89acb8;
 *      - it declared (void *, double), so the corrected value was written to
 *        $f12/$f13 while AudioCodecSeek still read the original a2:a3.
 *
 * FIX: intercept AudioCodecSeek at mq_player's GOT (the single choke point on
 * the verified call path). Within 3 s of a UPnP Seek, if the incoming value is
 * not the pending target (0/NaN/absurd, equal to the duration-in-seconds clamp
 * signature, or a sub-10 s value for a much longer target), substitute
 * pending_seconds * 1000.
 * ============================================================================== */

/* --- avformat_flush bypass (kept from 1.2.22; harmless, not the fix) ------ */

static void (*m4a_real_avformat_flush)(void *s) = 0;
static void avformat_flush(void *s);

static void m4a_safe_avformat_flush(void *s) {
  if (s) {
    /* AVFormatContext: offset 4 = AVInputFormat *, its offset 0 = name */
    void *iformat = *(void **)((char *)s + 4);
    if (iformat) {
      const char *name = *(const char **)iformat;
      if (name) {
        for (int i = 0; name[i]; i++) {
          if ((name[i] == 'm' && name[i + 1] == 'o' && name[i + 2] == 'v') ||
              (name[i] == 'm' && name[i + 1] == 'p' && name[i + 2] == '4') ||
              (name[i] == 'm' && name[i + 1] == '4' && name[i + 2] == 'a')) {
            return;
          }
        }
      }
    }
  }
  if (m4a_real_avformat_flush &&
      m4a_real_avformat_flush != avformat_flush &&
      m4a_real_avformat_flush != m4a_safe_avformat_flush) {
    m4a_real_avformat_flush(s);
  }
}

/* Exported symbol: libfiio_decoder.so's call binds here by symbol interposition
 * even when install() never runs. Never dereference a NULL real pointer. */
__attribute__((visibility("default"))) void avformat_flush(void *s) {
  m4a_safe_avformat_flush(s);
}

static unsigned m4a_find_lib_base(const char *libname) {
  int fd = open("/proc/self/maps", 0);
  if (fd < 0) return 0;
  char buf[1024];
  unsigned base = 0;
  int pos = 0;
  while (1) {
    int n = read(fd, buf + pos, sizeof(buf) - 1 - pos);
    if (n <= 0) break;
    pos += n;
    buf[pos] = '\0';
    char *p = buf;
    char *nl;
    while ((nl = strchr(p, '\n')) != 0) {
      *nl = '\0';
      if (strstr(p, libname) && strstr(p, " r-xp ")) {
        unsigned val = 0;
        for (char *c = p; *c && *c != '-'; c++) {
          val = (val << 4);
          if (*c >= '0' && *c <= '9') val += (unsigned)(*c - '0');
          else if (*c >= 'a' && *c <= 'f') val += (unsigned)(*c - 'a' + 10);
          else if (*c >= 'A' && *c <= 'F') val += (unsigned)(*c - 'A' + 10);
        }
        base = val;
        break;
      }
      p = nl + 1;
    }
    if (base) break;
    int rem = pos - (int)(p - buf);
    if (rem > 0 && p != buf) {
      for (int k = 0; k < rem; k++) buf[k] = p[k];
      pos = rem;
    } else if (rem == 0) {
      pos = 0;
    }
  }
  close(fd);
  return base;
}

/* --- AudioCodecSeek hook -------------------------------------------------- */

/* 0x89acb8: pending DLNA seek target in SECONDS (written by 0x5e2034). */
#define DISC_SEEK_TARGET_SEC 0x0089acb8u
#define DISC_MQ_SEEK_GOT     0x008380b4u
/* mq_player's static UPnP seek dispatch table starts at 0x83b960; slot +36 is
 * the only entry that receives REL_TIME seconds (verified: the "Seek" SOAP
 * action at 0x5e4280 calls 0x5dd174 -> *(0x89ac00)+36). */
#define DISC_MQ_SEEK_ENTRY   0x0083b984u

/* Matches libfiio_decoder's AudioCodecSeek: handle in a0, 64-bit double bits
 * in the aligned a2:a3 pair (o32). Do NOT use "double" here: this module is
 * hard-float, so a double parameter would travel in $f12/$f13 instead. */
static int (*real_seek_fn)(void *handle, long long bits) = 0;

/* Set when the UPnP Seek action reaches mq_player's own seek entry. The
 * AudioCodecSeek repair is only allowed for a few seconds afterwards, so a
 * later legitimate "seek to 0" (new track / decoder reset) is never hijacked
 * by the previous DLNA target. */
static int (*real_seek_entry_fn)(int seconds, int flags) = 0;
static volatile long seek_entry_at = 0;

static int seek_entry_hook(int seconds, int flags) {
  seek_entry_at = time(0);
  if (!real_seek_entry_fn) return -1;
  return real_seek_entry_fn(seconds, flags);
}

/* Player-core singleton (mq_player 0x83dfc4) and the duration field inside it.
 * 0x4586cc clamps its "seek target" argument against this field, so reading it
 * lets the hook recognise the clamp signature precisely. */
#define DISC_PLAYER_CORE 0x0083dfc4u

static int seek_core_field(int off) {
  unsigned core = *(volatile unsigned *)DISC_PLAYER_CORE;
  if (!core) return 0;
  return *(volatile int *)(core + (unsigned)off);
}

static int m4a_seek_hook(void *handle, long long bits) {
  union {
    double d;
    long long i;
  } u;
  u.i = bits;

  double in = u.d;
  int pend = *(volatile int *)DISC_SEEK_TARGET_SEC;
  long now = time(0);
  int fresh = seek_entry_at != 0 && now >= seek_entry_at &&
              now - seek_entry_at <= 3;
  int dur = seek_core_field(52);
  double want = (double)pend * 1000.0;

  /* 1.2.23 field logs showed the real failure is NOT a lost $f12: the DLNA
   * thread does put the right value (seconds * 1000 ms) into a0, but
   * 0x4586cc clamps it with
   *       if ([core+52] < target) target = [core+52];
   * where [core+52] is the track duration in SECONDS. So any target above
   * "duration seconds" (i.e. everything past ~3.4 s of a 57 min episode) is
   * clamped down to 3409 ms and playback restarts near the file head.
   * Repair whenever the value reaching the decoder is demonstrably not the
   * pending UPnP target: 0/NaN/absurd, the duration-clamp signature, or a
   * sub-10-second value while the user asked for far more. */
  if (pend > 0) {
    int bad = 0;
    if (!(in >= 1.0 && in <= 36000000.0)) {
      bad = 1;                                  /* 0 / NaN / out of range */
    } else if (dur > 0) {
      double d = (double)dur;
      if ((in >= d - 1.0 && in <= d + 1.0) ||
          (in >= d / 1000.0 - 1.0 && in <= d / 1000.0 + 1.0)) {
        bad = 1;                                /* clamped to duration */
      }
    }
    if (!bad && in < 10000.0 && want > in * 4.0) {
      bad = 1;                                  /* seconds leaked as ms */
    }
    if (bad && (fresh || !real_seek_entry_fn)) {
      u.d = want;
    }
  }

  return real_seek_fn ? real_seek_fn(handle, u.i) : -1;
}

static void m4a_seek_hook_install(void) {
  unsigned base = m4a_find_lib_base("libfiio_decoder.so");
  if (!base) return;

  /* 1. avformat_flush GOT inside libfiio_decoder.so (base + 0x1bc2c) */
  {
    volatile unsigned *got = (volatile unsigned *)(base + 0x0001bc2cu);
    m4a_real_avformat_flush = (void (*)(void *))*got;
    *got = (unsigned)m4a_safe_avformat_flush;
  }

  /* 2. AudioCodecSeek GOT inside mq_player (.got.plt is writable, no RELRO) */
  {
    real_seek_fn = (int (*)(void *, long long))(base + 0x000063a0u);
    {
      void *sym = dlsym((void *)0, "AudioCodecSeek");
      if (sym) real_seek_fn = (int (*)(void *, long long))sym;
    }
    volatile unsigned *got = (volatile unsigned *)DISC_MQ_SEEK_GOT;
    *got = (unsigned)m4a_seek_hook;
  }

  /* 2b. Timestamp the UPnP seek entry so the repair window is bounded. */
  {
    volatile unsigned *slot = (volatile unsigned *)DISC_MQ_SEEK_ENTRY;
    unsigned old = *slot;
    if (old == 0x005e2034u) {
      real_seek_entry_fn = (int (*)(int, int))old;
      *slot = (unsigned)seek_entry_hook;
    }
    /* Different firmware build: leave the slot alone and keep the unbounded
     * fallback instead of patching an unknown function. */
  }
}

#endif /* DISC_M4A_SEEK_HOOK_H */
