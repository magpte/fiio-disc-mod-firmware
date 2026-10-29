/*
 * podcast-radio/ui.h - Native pure-text hierarchical menu for Podcast & Internet Radio
 * Integrated directly into fiio-disc-mod-firmware (X2000 / LVGL v8).
 * Completely replaces external disc_ui_hook.c with zero hook contention.
 */

#ifndef DISC_PODCAST_RADIO_UI_H
#define DISC_PODCAST_RADIO_UI_H

extern char *strstr(const char *, const char *);
extern char *strchr(const char *, int);
extern void *memcpy(void *, const void *, size_t);

enum {
  PR_VIEW_NONE = 0,
  PR_VIEW_PODCAST_FEEDS,
  PR_VIEW_PODCAST_EPS,
  PR_VIEW_RADIO_CATS,
  PR_VIEW_RADIO_STATIONS
};

#define PR_EP_PER_PAGE 6
#define PR_RADIO_PER_PAGE 6
#define PR_MAX_DYN_EPS 80
#define PR_MAX_DYN_FEEDS 12
#define PR_MAX_DYN_CATS 8
#define PR_MAX_DYN_STATIONS 30

typedef struct {
  const char *name;
  const char *url;
} PrStation;

typedef struct {
  const char *cat_name;
  const PrStation *stations;
  unsigned count;
} PrRadioCat;

static const PrStation pr_national_stations[] = {
  {"中国之声 CNR-1", "https://lhttp.qtfm.cn/live/15318317/64k.mp3"},
  {"经济之声 CNR-2", "http://ngcdn002.cnr.cn/live/jjzs/index.m3u8"},
  {"音乐之声 CNR-3", "http://ngcdn003.cnr.cn/live/yyzs/index.m3u8"}
};

static const PrStation pr_province_stations[] = {
  {"广东音乐之声", "https://lhttp.qtfm.cn/live/1260/64k.mp3"},
  {"广东新闻广播", "https://lhttp.qtfm.cn/live/1254/64k.mp3"},
  {"北京新闻广播", "https://lhttp.qtfm.cn/live/339/64k.mp3"},
  {"上海新闻广播", "http://lhttp.qingting.fm/live/270/64k.mp3"},
  {"第一财经广播", "http://lhttp.qingting.fm/live/276/64k.mp3"}
};

static const PrStation pr_network_stations[] = {
  {"清晨音乐台", "http://lhttp.qingting.fm/live/4915/64k.mp3"},
  {"怀旧金曲", "http://lhttp.qingting.fm/live/4917/64k.mp3"},
  {"Radio Paradise", "https://stream.radioparadise.com/mp3-128"},
  {"SomaFM Groove Salad", "https://ice1.somafm.com/groovesalad-128-mp3"}
};

static const PrRadioCat pr_radio_cats[] = {
  {"国家电台", pr_national_stations, sizeof(pr_national_stations)/sizeof(pr_national_stations[0])},
  {"省市广播", pr_province_stations, sizeof(pr_province_stations)/sizeof(pr_province_stations[0])},
  {"网络电台", pr_network_stations, sizeof(pr_network_stations)/sizeof(pr_network_stations[0])}
};
#define PR_RADIO_CAT_COUNT (sizeof(pr_radio_cats)/sizeof(pr_radio_cats[0]))

typedef struct {
  const char *title;
  const char *url;
} PrEpisode;

typedef struct {
  const char *feed_title;
  const char *feed_id;
  const PrEpisode *episodes;
  unsigned ep_count;
} PrPodcastFeed;

/* Built-in default episodes with verified playable MP3/M4A streams */
static const PrEpisode pr_ritan_eps[] = {
  {"客栈里的武林：夜奔北京的游侠列传", "https://jt.ximalaya.com//GKwRIMAOs7DxAyCuoQTvIJVF.m4a?channel=rss&album_id=5574153&track_id=1027435622&uid=59126029&jt=https://aod.cos.tx.xmcdn.com/storages/8c8e-audiofreehighqps/5E/DF/GKwRIMAOs7DxAyCuoQTvIJVF.m4a"},
  {"白天看话剧，晚上夜游神：乌镇戏剧节", "https://jt.ximalaya.com//GKwRIW4OsMkXAaUu_gTtvCsx.m4a?channel=rss&album_id=5574153&track_id=1026966302&uid=59126029&jt=https://aod.cos.tx.xmcdn.com/storages/c72c-audiofreehighqps/DB/8D/GKwRIW4OsMkXAaUu_gTtvCsx.m4a"},
  {"预告：《融！五胡十六国》特别节目", "https://jt.ximalaya.com//GKwRIW4Oq5XQAAq3RgTrR6ZF.m4a?channel=rss&album_id=5574153&track_id=1025428475&uid=59126029&jt=https://aod.cos.tx.xmcdn.com/storages/5806-audiofreehighqps/54/B5/GKwRIW4Oq5XQAAq3RgTrR6ZF.m4a"},
  {"《猎罪图鉴》原型：对话模拟画像师林警官", "https://jt.ximalaya.com//GKwRIJIOqpthAbqCowTq_bJH.m4a?channel=rss&album_id=5574153&track_id=1025236577&uid=59126029&jt=https://aod.cos.tx.xmcdn.com/storages/4e74-audiofreehighqps/58/F9/GKwRIJIOqpthAbqCowTq_bJH.m4a"},
  {"从印度德国到墨西哥：漫游在废墟与无常间", "https://jt.ximalaya.com//GKwRIUEOoVCTAr7dawTmSSTk.m4a?channel=rss&album_id=5574153&track_id=1023316586&uid=59126029&jt=https://aod.cos.tx.xmcdn.com/storages/d3ad-audiofreehighqps/78/DD/GKwRIUEOoVCTAr7dawTmSSTk.m4a"},
  {"三国探墓记：真墓、假墓、玉米地名人冢", "https://jt.ximalaya.com//GKwRIJIOmgzLAitsxATjMnwe.m4a?channel=rss&album_id=5574153&track_id=1021373704&uid=59126029&jt=https://aod.cos.tx.xmcdn.com/storages/7892-audiofreehighqps/EB/2E/GKwRIJIOmgzLAitsxATjMnwe.m4a"}
};

static const PrEpisode pr_gcores_eps[] = {
  {"行走江湖前你需要知道的事儿：《影之刃》设定", "http://alioss.gcores.com/uploads/audio/403f9223-c5be-4dd6-a06a-5e449580ad0d.mp3"},
  {"做玩家能记住的女主！对话《湮灭之潮》开发组", "http://alioss.gcores.com/uploads/audio/bca2f0ce-21b5-4f89-9f71-b485318fe3c0.mp3"},
  {"辣手摧花，核周报 10.24", "http://alioss.gcores.com/uploads/audio/a116915a-18e4-441a-ae65-ed883eaa8254.mp3"},
  {"从鬼武者系列看CAPCOM | 说来话长 Vol.8", "http://alioss.gcores.com/uploads/audio/bdf95310-b688-43a4-b066-48117e07b5dd.mp3"},
  {"新作井喷，核周报 10.17", "http://alioss.gcores.com/uploads/audio/9f7eab3d-917e-482b-bf9c-541e2d2a65cf.mp3"},
  {"只靠这一点，我就能说它是神作", "http://alioss.gcores.com/uploads/audio/8a48dd84-ceeb-468c-bb57-a9b3cf0cd788.mp3"}
};

static const PrEpisode pr_hzyh_eps[] = {
  {"510 泰国侨批沉浮录：下南洋华人经济网络", "https://dts-api.xiaoyuzhoufm.com/track/5e4ee557418a84a0466737b7/6adb27a497726cd4675696ad/media.xyzcdn.net/5e4ee557418a84a0466737b7/ltWAJSww37kmooFownSqmZAbGbZO.m4a"},
  {"509 通往战时陪都之路：卢作孚建设重庆北碚", "https://dts-api.xiaoyuzhoufm.com/track/5e4ee557418a84a0466737b7/6ad72c1ff80d9814a6e11f31/media.xyzcdn.net/5e4ee557418a84a0466737b7/lrESn7lo3olBhoolQZ6CL09SU-wj.m4a"},
  {"508 相蓝欣谈国际关系视野下的中美欧今昔", "https://dts-api.xiaoyuzhoufm.com/track/5e4ee557418a84a0466737b7/6ad1e13c0e4e410a05bdd935/media.xyzcdn.net/5e4ee557418a84a0466737b7/lqrjNxCTeT30iSFmGt-wyavdWFfU.m4a"},
  {"507 葡萄酒盲品漫话：世锦赛、风土与掌故", "https://dts-api.xiaoyuzhoufm.com/track/5e4ee557418a84a0466737b7/6acdee13195d838e2af13466/media.xyzcdn.net/5e4ee557418a84a0466737b7/lj-iFY788jLSfGA_YZhmES-9qb2g.m4a"},
  {"506 亿万美元间谍：托卡契夫与冷战谍案", "https://dts-api.xiaoyuzhoufm.com/track/5e4ee557418a84a0466737b7/6ac8a47fe742e36efcc052a9/media.xyzcdn.net/5e4ee557418a84a0466737b7/lvQmgp0K-6R7O0WhWBAkBUct8kVn.m4a"},
  {"505 茶海轶闻：漫谈中国茶的真假传说", "https://dts-api.xiaoyuzhoufm.com/track/5e4ee557418a84a0466737b7/6abf70bee742e36efcbdbcba/media.xyzcdn.net/5e4ee557418a84a0466737b7/lighspj-Fv3GgyUj1wURPDbGXhza.m4a"}
};

static const PrEpisode pr_buhsy_eps[] = {
  {"能源转型进入下半场：环保与经济增长", "https://dts-api.xiaoyuzhoufm.com/track/5e280fb8418a84a0461fd076/6ae16a4497726cd46758df37/media.xyzcdn.net/5e280fb8418a84a0461fd076/llPsXE4uV4dIZnyyxmDFgRzaais-.m4a"},
  {"当科技让战争触屏可见，为何仍要抵达现场", "https://dts-api.xiaoyuzhoufm.com/track/5e280fb8418a84a0461fd076/6abc9e05195d838e2aebebb9/media.xyzcdn.net/5e280fb8418a84a0461fd076/lkuvT7dRIaodY6cY5Pq-Bj517OrD.m4a"},
  {"在科技时代，重新理解我们的爱与生活", "https://dts-api.xiaoyuzhoufm.com/track/5e280fb8418a84a0461fd076/6aa65785492687f6aad91b26/media.xyzcdn.net/5e280fb8418a84a0461fd076/ljJRm6_OvwduOUAecvyMKBeyUvuq.m4a"},
  {"我们如何老去？中国老年生活的真实图景", "https://dts-api.xiaoyuzhoufm.com/track/5e280fb8418a84a0461fd076/6a9f76a7f03e74ee6b06d00f/media.xyzcdn.net/5e280fb8418a84a0461fd076/lkyBUR9dGIM4VXxyJuQ1eav4I-UP.m4a"},
  {"漫长的告别：失智症、照护者与我们", "https://dts-api.xiaoyuzhoufm.com/track/5e280fb8418a84a0461fd076/6a93e755a0210c197dca6ffc/media.xyzcdn.net/5e280fb8418a84a0461fd076/lscoToGLQYozNYTQATlEfDwV3jeZ.m4a"},
  {"从基因编辑女童说起：自闭症与社会接纳", "https://dts-api.xiaoyuzhoufm.com/track/5e280fb8418a84a0461fd076/6a912d76f03e74ee6b0123d1/media.xyzcdn.net/5e280fb8418a84a0461fd076/lg7DOFyeQv-7noQ9W74oH6hsYk4b.m4a"}
};

static const PrPodcastFeed pr_podcasts[] = {
  {"日谈公园", "ritan", pr_ritan_eps, sizeof(pr_ritan_eps)/sizeof(pr_ritan_eps[0])},
  {"机核网 GCORES", "gcores", pr_gcores_eps, sizeof(pr_gcores_eps)/sizeof(pr_gcores_eps[0])},
  {"忽左忽右", "hzyh", pr_hzyh_eps, sizeof(pr_hzyh_eps)/sizeof(pr_hzyh_eps[0])},
  {"不合时宜", "buhsy", pr_buhsy_eps, sizeof(pr_buhsy_eps)/sizeof(pr_buhsy_eps[0])}
};
#define PR_PODCAST_COUNT (sizeof(pr_podcasts)/sizeof(pr_podcasts[0]))

/* Dynamic parsed episodes from /tmp/disc-podcast-radio/episodes.txt */
typedef struct {
  char title[160];
  char url[384];
} PrDynEp;

typedef struct {
  char feed_title[64];
  PrDynEp eps[PR_MAX_DYN_EPS];
  unsigned ep_count;
} PrDynFeed;

static PrDynFeed pr_dyn_feeds[PR_MAX_DYN_FEEDS];
static unsigned pr_dyn_feed_count = 0;

static void pr_reload_dynamic_episodes(void) {
  int fd = open("/tmp/disc-podcast-radio/episodes.txt", 0);
  if (fd < 0) return;
  static char fbuf[32768];
  int n = read(fd, fbuf, sizeof(fbuf) - 1);
  close(fd);
  if (n <= 0) return;
  fbuf[n] = '\0';

  pr_dyn_feed_count = 0;
  char *line = fbuf;
  int cur = -1;

  while (*line) {
    char *eol = strchr(line, '\n');
    if (eol) *eol = '\0';

    char *p = line;
    while (*p == ' ' || *p == '\t' || *p == '\r') p++;

    if (*p == '[') {
      char *end = strchr(p, ']');
      if (end && pr_dyn_feed_count < PR_MAX_DYN_FEEDS) {
        cur = (int)pr_dyn_feed_count++;
        int len = (int)(end - (p + 1));
        if (len > 60) len = 60;
        memcpy(pr_dyn_feeds[cur].feed_title, p + 1, (size_t)len);
        pr_dyn_feeds[cur].feed_title[len] = '\0';
        pr_dyn_feeds[cur].ep_count = 0;
      }
    } else if (cur >= 0 && pr_dyn_feeds[cur].ep_count < PR_MAX_DYN_EPS) {
      char *sep = strstr(p, " | ");
      if (sep) {
        *sep = '\0';
        char *url = sep + 3;
        char *uend = url;
        while (*uend && *uend != '\r' && *uend != '\n') uend++;
        *uend = '\0';

        unsigned idx = pr_dyn_feeds[cur].ep_count++;
        snprintf(pr_dyn_feeds[cur].eps[idx].title, sizeof(pr_dyn_feeds[cur].eps[idx].title), "%s", p);
        snprintf(pr_dyn_feeds[cur].eps[idx].url, sizeof(pr_dyn_feeds[cur].eps[idx].url), "%s", url);
      }
    }

    if (!eol) break;
    line = eol + 1;
  }
}

/* Dynamic parsed radio stations from /tmp/disc-podcast-radio/radios.txt */
typedef struct {
  char name[96];
  char url[384];
} PrDynStation;

typedef struct {
  char cat_name[64];
  PrDynStation stations[PR_MAX_DYN_STATIONS];
  unsigned count;
} PrDynRadioCat;

static PrDynRadioCat pr_dyn_radio_cats[PR_MAX_DYN_CATS];
static unsigned pr_dyn_radio_cat_count = 0;

static void pr_reload_dynamic_radios(void) {
  int fd = open("/tmp/disc-podcast-radio/radios.txt", 0);
  if (fd < 0) return;
  static char rbuf[16384];
  int n = read(fd, rbuf, sizeof(rbuf) - 1);
  close(fd);
  if (n <= 0) return;
  rbuf[n] = '\0';

  pr_dyn_radio_cat_count = 0;
  int cur = -1;
  char *line = rbuf;

  while (*line) {
    char *eol = strchr(line, '\n');
    if (eol) *eol = '\0';

    char *p = line;
    while (*p == ' ' || *p == '\t' || *p == '\r') p++;

    if (*p == '[') {
      char *end = strchr(p, ']');
      if (end && pr_dyn_radio_cat_count < PR_MAX_DYN_CATS) {
        cur = (int)pr_dyn_radio_cat_count++;
        int len = (int)(end - (p + 1));
        if (len > 60) len = 60;
        memcpy(pr_dyn_radio_cats[cur].cat_name, p + 1, (size_t)len);
        pr_dyn_radio_cats[cur].cat_name[len] = '\0';
        pr_dyn_radio_cats[cur].count = 0;
      }
    } else if (cur >= 0 && pr_dyn_radio_cats[cur].count < PR_MAX_DYN_STATIONS) {
      char *sep = strstr(p, " | ");
      if (sep) {
        *sep = '\0';
        char *url = sep + 3;
        char *uend = url;
        while (*uend && *uend != '\r' && *uend != '\n') uend++;
        *uend = '\0';

        unsigned idx = pr_dyn_radio_cats[cur].count++;
        snprintf(pr_dyn_radio_cats[cur].stations[idx].name, sizeof(pr_dyn_radio_cats[cur].stations[idx].name), "%s", p);
        snprintf(pr_dyn_radio_cats[cur].stations[idx].url, sizeof(pr_dyn_radio_cats[cur].stations[idx].url), "%s", url);
      }
    }

    if (!eol) break;
    line = eol + 1;
  }
}

/* UI Handles & State */
static LvObj *pr_menu_rows[2]; /* 0: 播客, 1: 网络收音机 */
static LvObj *pr_page;
static LvObj *pr_status_label;
static int pr_current_view = PR_VIEW_NONE;
static int pr_open_pending = 0;
static int pr_requested_view = PR_VIEW_NONE;
static int pr_back_pending = 0;
static int pr_selected_feed = 0;
static int pr_selected_cat = 0;
static int pr_ep_page = 0;
static int pr_radio_page = 0;

/* Explicit pointers to row objects to guarantee zero index mismatch */
static LvObj *pr_row_objs[16];
static LvObj *pr_ep_rows[PR_EP_PER_PAGE];
static LvObj *pr_prev_btn = 0;
static LvObj *pr_next_btn = 0;

static void pr_make(int view);
static void pr_clear(void);

static void pr_deleted(void *e) {
  LvObj *target = lv_event_target(e);
  if (target == pr_menu_rows[0]) pr_menu_rows[0] = 0;
  if (target == pr_menu_rows[1]) pr_menu_rows[1] = 0;
  if (target == pr_prev_btn) pr_prev_btn = 0;
  if (target == pr_next_btn) pr_next_btn = 0;
  if (target == pr_page) {
    pr_page = 0;
    pr_status_label = 0;
    pr_current_view = PR_VIEW_NONE;
    pr_prev_btn = 0;
    pr_next_btn = 0;
    for (int i = 0; i < 16; i++) pr_row_objs[i] = 0;
    for (int i = 0; i < PR_EP_PER_PAGE; i++) pr_ep_rows[i] = 0;
  }
}

static void pr_clear(void) {
  if (!pr_page) return;
  LvObj *old = pr_page;
  pr_page = 0;
  pr_status_label = 0;
  pr_prev_btn = 0;
  pr_next_btn = 0;
  for (int i = 0; i < 16; i++) pr_row_objs[i] = 0;
  for (int i = 0; i < PR_EP_PER_PAGE; i++) pr_ep_rows[i] = 0;
  screen_delete(&old);
}

/* Playback command transmission to disc-podcast-radio backend daemon */
static void pr_send_play(const char *type, const char *title, const char *url, const char *artist) {
  /* Guarantee online_music_session is false so DLNA stream audio format/bitrate is shown */
  unlink("/tmp/disc-online-music/session");
  unlink("/tmp/disc-online-music/active");

  mkdir("/tmp/disc-podcast-radio", 0777);

  if (*(unsigned char *)0x8e1729 != 12) {
    dlna_enter_native();
    if (pr_page && lv_valid(pr_page)) {
      screen_load(pr_page);
    }
  }
  int fd = open("/tmp/disc-podcast-radio/play_cmd", 1 | 0x100 | 0x200, 0666);
  if (fd >= 0) {
    char buf[1024];
    int n = snprintf(buf, sizeof(buf), "%s\t%s\t%s\t%s\n", type, title, url, artist ? artist : "");
    write(fd, buf, n);
    close(fd);
  }
  int fdbg = open("/tmp/sdcard/disc-ui.log", 1 | 0x100 | 0x0008, 0666);
  if (fdbg >= 0) {
    char dbgbuf[512];
    int dn = snprintf(dbgbuf, sizeof(dbgbuf), "[UI] Play: type=%s title=%s url=%s artist=%s\n",
                     type, title, url, artist ? artist : "");
    write(fdbg, dbgbuf, dn);
    close(fdbg);
  }
}

/* Entry click callbacks in the Media list */
static void pr_confirmed(void) {
  pr_open_pending = 1;
}

static void pr_entry_click(void *e) {
  if (!stationary_clicked(e)) return;
  LvObj *target = lv_event_target(e);
  if (target == pr_menu_rows[0]) {
    pr_requested_view = PR_VIEW_PODCAST_FEEDS;
  } else {
    pr_requested_view = PR_VIEW_RADIO_CATS;
  }

  if (FN(0x46f290, int (*)(void))() != 17) {
    FN(0x46fdac, void (*)(void (*)(void)))(FN(0x474838, void (*)(void)));
    return;
  }

  if (*(unsigned char *)0x8e171c) {
    FN(0x46b7a0, void (*)(void))();
    FN(0x459f20, void (*)(unsigned))(1);
    FN(0x459f14, void (*)(void (*)(void)))(pr_confirmed);
    FN(0x45a940, void (*)(void))();
    FN(0x459f20, void (*)(unsigned))(0);
  } else {
    pr_confirmed();
  }
}

static void pr_swipe(void *e) {
  if (lv_event_code(e) != 8) return;
  int r = FN(0x488860, int (*)(void *))(e);
  if (r == 3) {
    pr_back_pending = 1;
  }
}

static void pr_feed_click(void *e) {
  if (!stationary_clicked(e)) return;
  LvObj *target = lv_event_target(e);
  unsigned user_data = (unsigned long)FN(0x4e394c, void *(*)(void *))(e);

  pr_reload_dynamic_episodes();
  unsigned count = pr_dyn_feed_count > 0 ? pr_dyn_feed_count : PR_PODCAST_COUNT;

  for (unsigned i = 0; i < count && i < 16; i++) {
    if (target == pr_row_objs[i] || (pr_row_objs[i] && target == child(pr_row_objs[i], 0)) || user_data == i) {
      pr_selected_feed = (int)i;
      pr_ep_page = 0;
      pr_make(PR_VIEW_PODCAST_EPS);
      return;
    }
  }
}

static void pr_prev_page_click(void *e) {
  if (!stationary_clicked(e)) return;
  if (pr_ep_page > 0) {
    pr_ep_page--;
    pr_make(PR_VIEW_PODCAST_EPS);
  }
}

static void pr_next_page_click(void *e) {
  if (!stationary_clicked(e)) return;
  unsigned total_eps = 0;
  pr_reload_dynamic_episodes();
  if (pr_dyn_feed_count > 0 && pr_selected_feed >= 0 && pr_selected_feed < (int)pr_dyn_feed_count) {
    total_eps = pr_dyn_feeds[pr_selected_feed].ep_count;
  } else if (pr_selected_feed >= 0 && pr_selected_feed < (int)PR_PODCAST_COUNT) {
    total_eps = pr_podcasts[pr_selected_feed].ep_count;
  }
  unsigned total_pages = (total_eps + PR_EP_PER_PAGE - 1) / PR_EP_PER_PAGE;
  if (total_pages == 0) total_pages = 1;
  if ((unsigned)pr_ep_page < total_pages - 1) {
    pr_ep_page++;
    pr_make(PR_VIEW_PODCAST_EPS);
  }
}

static unsigned pr_last_play_click = 0;

static void pr_ep_click(void *e) {
  if (!stationary_clicked(e)) return;
  unsigned now = milliseconds();
  if (pr_last_play_click && now - pr_last_play_click < 600) return;
  pr_last_play_click = now;
  LvObj *target = lv_event_target(e);
  unsigned slot = (unsigned long)FN(0x4e394c, void *(*)(void *))(e);
  for (unsigned s = 0; s < PR_EP_PER_PAGE; s++) {
    if (target == pr_ep_rows[s] || (pr_ep_rows[s] && target == child(pr_ep_rows[s], 0))) {
      slot = s;
      break;
    }
  }

  pr_reload_dynamic_episodes();
  const char *feed_title = "播客";
  unsigned total_eps = 0;
  const PrDynEp *dyn_list = 0;
  const PrEpisode *built_list = 0;

  if (pr_dyn_feed_count > 0 && pr_selected_feed >= 0 && pr_selected_feed < (int)pr_dyn_feed_count) {
    feed_title = pr_dyn_feeds[pr_selected_feed].feed_title;
    total_eps = pr_dyn_feeds[pr_selected_feed].ep_count;
    dyn_list = pr_dyn_feeds[pr_selected_feed].eps;
  } else if (pr_selected_feed >= 0 && pr_selected_feed < (int)PR_PODCAST_COUNT) {
    feed_title = pr_podcasts[pr_selected_feed].feed_title;
    total_eps = pr_podcasts[pr_selected_feed].ep_count;
    built_list = pr_podcasts[pr_selected_feed].episodes;
  }

  unsigned ep_idx = (unsigned)pr_ep_page * PR_EP_PER_PAGE + slot;
  const char *ep_title = 0;
  const char *ep_url = 0;

  if (dyn_list && ep_idx < total_eps) {
    ep_title = dyn_list[ep_idx].title;
    ep_url = dyn_list[ep_idx].url;
  } else if (built_list && ep_idx < total_eps) {
    ep_title = built_list[ep_idx].title;
    ep_url = built_list[ep_idx].url;
  }

  if (ep_title && ep_url) {
    pr_send_play("podcast", ep_title, ep_url, feed_title);
    if (pr_status_label) lv_text(pr_status_label, "正在连接播客…");
  }
}

static void pr_cat_click(void *e) {
  if (!stationary_clicked(e)) return;
  LvObj *target = lv_event_target(e);
  unsigned user_data = (unsigned long)FN(0x4e394c, void *(*)(void *))(e);

  pr_reload_dynamic_radios();
  unsigned count = pr_dyn_radio_cat_count > 0 ? pr_dyn_radio_cat_count : PR_RADIO_CAT_COUNT;

  for (unsigned i = 0; i < count && i < 16; i++) {
    if (target == pr_row_objs[i] || (pr_row_objs[i] && target == child(pr_row_objs[i], 0)) || user_data == i) {
      pr_selected_cat = (int)i;
      pr_radio_page = 0;
      pr_make(PR_VIEW_RADIO_STATIONS);
      return;
    }
  }
}

static void pr_prev_radio_click(void *e) {
  if (!stationary_clicked(e)) return;
  if (pr_radio_page > 0) {
    pr_radio_page--;
    pr_make(PR_VIEW_RADIO_STATIONS);
  }
}

static void pr_next_radio_click(void *e) {
  if (!stationary_clicked(e)) return;
  unsigned total_stations = 0;
  pr_reload_dynamic_radios();
  if (pr_dyn_radio_cat_count > 0 && pr_selected_cat >= 0 && pr_selected_cat < (int)pr_dyn_radio_cat_count) {
    total_stations = pr_dyn_radio_cats[pr_selected_cat].count;
  } else if (pr_selected_cat >= 0 && pr_selected_cat < (int)PR_RADIO_CAT_COUNT) {
    total_stations = pr_radio_cats[pr_selected_cat].count;
  }
  unsigned total_pages = (total_stations + PR_RADIO_PER_PAGE - 1) / PR_RADIO_PER_PAGE;
  if (total_pages == 0) total_pages = 1;
  if ((unsigned)pr_radio_page < total_pages - 1) {
    pr_radio_page++;
    pr_make(PR_VIEW_RADIO_STATIONS);
  }
}

static void pr_station_click(void *e) {
  if (!stationary_clicked(e)) return;
  unsigned now = milliseconds();
  if (pr_last_play_click && now - pr_last_play_click < 600) return;
  pr_last_play_click = now;
  LvObj *target = lv_event_target(e);
  unsigned user_data = (unsigned long)FN(0x4e394c, void *(*)(void *))(e);

  pr_reload_dynamic_radios();
  const char *cat_name = "电台";
  const char *st_name = 0;
  const char *st_url = 0;

  if (pr_dyn_radio_cat_count > 0 && pr_selected_cat >= 0 && pr_selected_cat < (int)pr_dyn_radio_cat_count) {
    const PrDynRadioCat *cat = &pr_dyn_radio_cats[pr_selected_cat];
    cat_name = cat->cat_name;
    for (unsigned s = 0; s < PR_RADIO_PER_PAGE; s++) {
      if (target == pr_row_objs[s] || (pr_row_objs[s] && target == child(pr_row_objs[s], 0)) || user_data == s) {
        unsigned idx = (unsigned)pr_radio_page * PR_RADIO_PER_PAGE + s;
        if (idx < cat->count) {
          st_name = cat->stations[idx].name;
          st_url = cat->stations[idx].url;
        }
        break;
      }
    }
  } else if (pr_selected_cat >= 0 && pr_selected_cat < (int)PR_RADIO_CAT_COUNT) {
    const PrRadioCat *cat = &pr_radio_cats[pr_selected_cat];
    cat_name = cat->cat_name;
    for (unsigned s = 0; s < PR_RADIO_PER_PAGE; s++) {
      if (target == pr_row_objs[s] || (pr_row_objs[s] && target == child(pr_row_objs[s], 0)) || user_data == s) {
        unsigned idx = (unsigned)pr_radio_page * PR_RADIO_PER_PAGE + s;
        if (idx < cat->count) {
          st_name = cat->stations[idx].name;
          st_url = cat->stations[idx].url;
        }
        break;
      }
    }
  }

  if (st_name && st_url) {
    pr_send_play("radio", st_name, st_url, cat_name);
    if (pr_status_label) lv_text(pr_status_label, "正在连接电台…");
  }
}

static void pr_setup_row(LvObj *row, int height, int hide_arrow, int marquee) {
  (void)marquee;
  if (!row) return;
  lv_size(row, 360, height);
  protect_tap(row);
  LvObj *lbl = child(row, 0);
  if (lbl) {
    lv_clear_flag(lbl, DISC_LV_CLICKABLE | DISC_LV_SCROLLABLE);
    lv_add_flag(lbl, 0x4000);
    /* Set mode 4 (clip) to prevent any vertical or circular scrolling */
    FN(0x52f4f0, void (*)(LvObj *, int))(lbl, 4);
    lv_size(lbl, 246, height - 6);
  }
  LvObj *arrow = child(row, 1);
  if (arrow) {
    lv_clear_flag(arrow, DISC_LV_CLICKABLE | DISC_LV_SCROLLABLE);
    lv_add_flag(arrow, 0x4000);
    if (hide_arrow) {
      lv_add_flag(arrow, DISC_LV_HIDDEN);
    }
  }
}

static void pr_font(LvObj *o, unsigned size) {
  void **f = FN(0x48831c, void **(*)(unsigned))(size);
  if (f && *f) lv_add_style(o, *f, 0);
}

/* Side navigation buttons docked on left and right spaces of the circular screen */
static void pr_create_nav_buttons(int page, unsigned total_pages, void (*prev_cb)(void *), void (*next_cb)(void *)) {
  if (!pr_page) return;

  char page_str[32];
  snprintf(page_str, sizeof(page_str), "%d/%d", page + 1, total_pages);

  /* Left button: Previous page - docked at x=4, width=36 */
  pr_prev_btn = lv_btn(pr_page);
  if (pr_prev_btn) {
    lv_size(pr_prev_btn, 36, 52);
    lv_align(pr_prev_btn, 1, 4, 154);
    lv_radius(pr_prev_btn, 10, 0);
    pad_top(pr_prev_btn, 0, 0);
    pad_bottom(pr_prev_btn, 0, 0);
    pad_left(pr_prev_btn, 0, 0);
    pad_right(pr_prev_btn, 0, 0);
    protect_tap(pr_prev_btn);
    lv_add_cb(pr_prev_btn, prev_cb, 7, 0);

    /* Arrow icon */
    LvObj *arr = lv_label(pr_prev_btn);
    if (arr) {
      pr_font(arr, 16);
      lv_size(arr, 36, 20);
      lv_align(arr, 1, 0, 6);
      lv_text(arr, "◀");
      FN(0x4f6e50, void (*)(LvObj *, int, unsigned))(arr, 2, 0);
      lv_clear_flag(arr, DISC_LV_CLICKABLE | DISC_LV_SCROLLABLE);
      lv_color(arr, page > 0 ? 0xff54baff : 0xff334150, 0);
    }

    /* Page number label */
    LvObj *lbl = lv_label(pr_prev_btn);
    if (lbl) {
      pr_font(lbl, 13);
      lv_size(lbl, 36, 16);
      lv_align(lbl, 1, 0, 28);
      lv_text(lbl, page_str);
      FN(0x4f6e50, void (*)(LvObj *, int, unsigned))(lbl, 2, 0);
      lv_clear_flag(lbl, DISC_LV_CLICKABLE | DISC_LV_SCROLLABLE);
      lv_color(lbl, page > 0 ? 0xffd0e8ff : 0xff667788, 0);
    }

    if (page > 0) {
      lv_bg(pr_prev_btn, 0xff162436, 0);
      bg_opa(pr_prev_btn, 220, 0);
      lv_border(pr_prev_btn, 1, 0);
    } else {
      lv_bg(pr_prev_btn, 0xff0d131a, 0);
      bg_opa(pr_prev_btn, 130, 0);
      lv_border(pr_prev_btn, 0, 0);
    }
  }

  /* Right button: Next page - docked at x=320, width=36 */
  pr_next_btn = lv_btn(pr_page);
  if (pr_next_btn) {
    int has_next = ((unsigned)page < total_pages - 1);
    lv_size(pr_next_btn, 36, 52);
    lv_align(pr_next_btn, 1, 320, 154);
    lv_radius(pr_next_btn, 10, 0);
    pad_top(pr_next_btn, 0, 0);
    pad_bottom(pr_next_btn, 0, 0);
    pad_left(pr_next_btn, 0, 0);
    pad_right(pr_next_btn, 0, 0);
    protect_tap(pr_next_btn);
    lv_add_cb(pr_next_btn, next_cb, 7, 0);

    /* Arrow icon */
    LvObj *arr = lv_label(pr_next_btn);
    if (arr) {
      pr_font(arr, 16);
      lv_size(arr, 36, 20);
      lv_align(arr, 1, 0, 6);
      lv_text(arr, "▶");
      FN(0x4f6e50, void (*)(LvObj *, int, unsigned))(arr, 2, 0);
      lv_clear_flag(arr, DISC_LV_CLICKABLE | DISC_LV_SCROLLABLE);
      lv_color(arr, has_next ? 0xff54baff : 0xff334150, 0);
    }

    /* Page number label */
    LvObj *lbl = lv_label(pr_next_btn);
    if (lbl) {
      pr_font(lbl, 13);
      lv_size(lbl, 36, 16);
      lv_align(lbl, 1, 0, 28);
      lv_text(lbl, page_str);
      FN(0x4f6e50, void (*)(LvObj *, int, unsigned))(lbl, 2, 0);
      lv_clear_flag(lbl, DISC_LV_CLICKABLE | DISC_LV_SCROLLABLE);
      lv_color(lbl, has_next ? 0xffd0e8ff : 0xff667788, 0);
    }

    if (has_next) {
      lv_bg(pr_next_btn, 0xff162436, 0);
      bg_opa(pr_next_btn, 220, 0);
      lv_border(pr_next_btn, 1, 0);
    } else {
      lv_bg(pr_next_btn, 0xff0d131a, 0);
      bg_opa(pr_next_btn, 130, 0);
      lv_border(pr_next_btn, 0, 0);
    }
  }
}

static void pr_make(int view) {
  pr_clear();
  pr_current_view = view;
  pr_page = lv_create(0);
  if (!pr_page) return;

  screen_register(pr_page);
  lv_add_cb(pr_page, pr_deleted, DISC_LV_DELETE, 0);

  /* Swipe gesture returns to previous level */
  lv_add_cb(pr_page, pr_swipe, 1, 0);
  lv_add_cb(pr_page, pr_swipe, 2, 0);
  lv_add_cb(pr_page, pr_swipe, 8, 0);

  lv_size(pr_page, 360, 360);
  lv_border(pr_page, 0, 0);
  lv_bg(pr_page, 0xff080c12, 0);

  pad_top(pr_page, 0, 0);
  pad_bottom(pr_page, 0, 0);
  pad_left(pr_page, 0, 0);
  pad_right(pr_page, 0, 0);

  /* Scrollable flex container for list content */
  LvObj *pr_list = lv_create(pr_page);
  if (!pr_list) return;
  lv_size(pr_list, 360, 360);
  lv_border(pr_list, 0, 0);
  bg_opa(pr_list, 0, 0);
  pad_top(pr_list, 0, 0);
  pad_bottom(pr_list, 0, 0);
  pad_left(pr_list, 0, 0);
  pad_right(pr_list, 0, 0);
  FN(0x4f6b30, void (*)(LvObj *, int, unsigned))(pr_list, 2, 0);

  /* Flex Column Flow for Vertical Scrolling with compact 2px gap */
  FN(0x4f0268, void (*)(LvObj *, int))(pr_list, 0);
  FN(0x4f02c8, void (*)(LvObj *, int))(pr_list, 12);
  FN(0x5030a0, void (*)(LvObj *, int))(pr_list, 1);
  FN(0x5030e4, void (*)(LvObj *, int, int, int))(pr_list, 0, 0, 2);

  /* Forward swipe events from pr_list to handle back gesture */
  lv_add_cb(pr_list, pr_swipe, 1, 0);
  lv_add_cb(pr_list, pr_swipe, 2, 0);
  lv_add_cb(pr_list, pr_swipe, 8, 0);

  /* Layout offsets: pad_left 42 shifts text close to left button (x=4..40), ending at 288, leaving 32px gap before right button (x=320) */
  int cat_layout[3] = {42, 246, 0};
  int item_layout[3] = {42, 246, 0};

  /* 1. Header Title */
  char title_buf[64];
  const char *title_text = "流媒体";
  if (view == PR_VIEW_PODCAST_FEEDS) {
    title_text = "播客订阅";
  } else if (view == PR_VIEW_PODCAST_EPS) {
    pr_reload_dynamic_episodes();
    const char *raw_name = "播客节目";
    unsigned total_eps = 0;
    if (pr_dyn_feed_count > 0 && pr_selected_feed >= 0 && pr_selected_feed < (int)pr_dyn_feed_count) {
      raw_name = pr_dyn_feeds[pr_selected_feed].feed_title;
      total_eps = pr_dyn_feeds[pr_selected_feed].ep_count;
    } else if (pr_selected_feed >= 0 && pr_selected_feed < (int)PR_PODCAST_COUNT) {
      raw_name = pr_podcasts[pr_selected_feed].feed_title;
      total_eps = pr_podcasts[pr_selected_feed].ep_count;
    }
    unsigned total_pages = (total_eps + PR_EP_PER_PAGE - 1) / PR_EP_PER_PAGE;
    if (total_pages == 0) total_pages = 1;
    if (total_pages > 1) {
      snprintf(title_buf, sizeof(title_buf), "%s (%d/%d)", raw_name, pr_ep_page + 1, total_pages);
      title_text = title_buf;
    } else {
      title_text = raw_name;
    }
  } else if (view == PR_VIEW_RADIO_CATS) {
    title_text = "网络收音机";
  } else if (view == PR_VIEW_RADIO_STATIONS) {
    pr_reload_dynamic_radios();
    const char *raw_name = "电台列表";
    unsigned total_stations = 0;
    if (pr_dyn_radio_cat_count > 0 && pr_selected_cat >= 0 && pr_selected_cat < (int)pr_dyn_radio_cat_count) {
      raw_name = pr_dyn_radio_cats[pr_selected_cat].cat_name;
      total_stations = pr_dyn_radio_cats[pr_selected_cat].count;
    } else if (pr_selected_cat >= 0 && pr_selected_cat < (int)PR_RADIO_CAT_COUNT) {
      raw_name = pr_radio_cats[pr_selected_cat].cat_name;
      total_stations = pr_radio_cats[pr_selected_cat].count;
    }
    unsigned total_pages = (total_stations + PR_RADIO_PER_PAGE - 1) / PR_RADIO_PER_PAGE;
    if (total_pages == 0) total_pages = 1;
    if (total_pages > 1) {
      snprintf(title_buf, sizeof(title_buf), "%s (%d/%d)", raw_name, pr_radio_page + 1, total_pages);
      title_text = title_buf;
    } else {
      title_text = raw_name;
    }
  }

  native_title(pr_list, title_text, 0xb4fb);
  LvObj *title_obj = child(pr_list, 0);
  if (title_obj) {
    lv_color(title_obj, 0xff54baff, 0); /* Cyan accent */
    lv_clear_flag(title_obj, DISC_LV_CLICKABLE | DISC_LV_SCROLLABLE);
    lv_add_flag(title_obj, 0x4000);
  }

  /* 2. Populate View Rows */
  if (view == PR_VIEW_PODCAST_FEEDS) {
    pr_reload_dynamic_episodes();
    if (pr_dyn_feed_count > 0) {
      for (unsigned i = 0; i < pr_dyn_feed_count && i < 16; i++) {
        unsigned n = child_count(pr_list);
        original_row(pr_list, pr_dyn_feeds[i].feed_title, i, pr_feed_click, cat_layout);
        pr_row_objs[i] = child(pr_list, (int)n);
        pr_setup_row(pr_row_objs[i], 38, 0, 0);
      }
    } else {
      for (unsigned i = 0; i < PR_PODCAST_COUNT; i++) {
        unsigned n = child_count(pr_list);
        original_row(pr_list, pr_podcasts[i].feed_title, i, pr_feed_click, cat_layout);
        pr_row_objs[i] = child(pr_list, (int)n);
        pr_setup_row(pr_row_objs[i], 38, 0, 0);
      }
    }
  } else if (view == PR_VIEW_PODCAST_EPS) {
    pr_reload_dynamic_episodes();
    unsigned total_eps = 0;
    const PrDynEp *dyn_list = 0;
    const PrEpisode *built_list = 0;

    if (pr_dyn_feed_count > 0 && pr_selected_feed >= 0 && pr_selected_feed < (int)pr_dyn_feed_count) {
      total_eps = pr_dyn_feeds[pr_selected_feed].ep_count;
      dyn_list = pr_dyn_feeds[pr_selected_feed].eps;
    } else if (pr_selected_feed >= 0 && pr_selected_feed < (int)PR_PODCAST_COUNT) {
      total_eps = pr_podcasts[pr_selected_feed].ep_count;
      built_list = pr_podcasts[pr_selected_feed].episodes;
    }

    unsigned total_pages = (total_eps + PR_EP_PER_PAGE - 1) / PR_EP_PER_PAGE;
    if (total_pages == 0) total_pages = 1;
    if ((unsigned)pr_ep_page >= total_pages) pr_ep_page = (int)total_pages - 1;

    unsigned start_idx = (unsigned)pr_ep_page * PR_EP_PER_PAGE;
    unsigned end_idx = start_idx + PR_EP_PER_PAGE;
    if (end_idx > total_eps) end_idx = total_eps;

    /* Populate episode rows for current page - 6 episodes comfortably visible on screen */
    for (unsigned i = start_idx; i < end_idx; i++) {
      unsigned slot = i - start_idx;
      const char *ep_text = "";
      if (dyn_list) ep_text = dyn_list[i].title;
      else if (built_list) ep_text = built_list[i].title;

      unsigned n = child_count(pr_list);
      original_row(pr_list, ep_text, slot, pr_ep_click, item_layout);
      pr_ep_rows[slot] = child(pr_list, (int)n);
      pr_setup_row(pr_ep_rows[slot], 38, 1, 0);
    }

    /* Side navigation icon buttons with (x/y) page indication */
    if (total_pages > 1) {
      pr_create_nav_buttons(pr_ep_page, total_pages, pr_prev_page_click, pr_next_page_click);
    }
  } else if (view == PR_VIEW_RADIO_CATS) {
    pr_reload_dynamic_radios();
    if (pr_dyn_radio_cat_count > 0) {
      for (unsigned i = 0; i < pr_dyn_radio_cat_count && i < 16; i++) {
        unsigned n = child_count(pr_list);
        original_row(pr_list, pr_dyn_radio_cats[i].cat_name, i, pr_cat_click, cat_layout);
        pr_row_objs[i] = child(pr_list, (int)n);
        pr_setup_row(pr_row_objs[i], 38, 0, 0);
      }
    } else {
      for (unsigned i = 0; i < PR_RADIO_CAT_COUNT; i++) {
        unsigned n = child_count(pr_list);
        original_row(pr_list, pr_radio_cats[i].cat_name, i, pr_cat_click, cat_layout);
        pr_row_objs[i] = child(pr_list, (int)n);
        pr_setup_row(pr_row_objs[i], 38, 0, 0);
      }
    }
  } else if (view == PR_VIEW_RADIO_STATIONS) {
    pr_reload_dynamic_radios();
    unsigned total_stations = 0;
    const PrDynStation *dyn_st = 0;
    const PrStation *built_st = 0;

    if (pr_dyn_radio_cat_count > 0 && pr_selected_cat >= 0 && pr_selected_cat < (int)pr_dyn_radio_cat_count) {
      total_stations = pr_dyn_radio_cats[pr_selected_cat].count;
      dyn_st = pr_dyn_radio_cats[pr_selected_cat].stations;
    } else if (pr_selected_cat >= 0 && pr_selected_cat < (int)PR_RADIO_CAT_COUNT) {
      total_stations = pr_radio_cats[pr_selected_cat].count;
      built_st = pr_radio_cats[pr_selected_cat].stations;
    }

    unsigned total_pages = (total_stations + PR_RADIO_PER_PAGE - 1) / PR_RADIO_PER_PAGE;
    if (total_pages == 0) total_pages = 1;
    if ((unsigned)pr_radio_page >= total_pages) pr_radio_page = (int)total_pages - 1;

    unsigned start_idx = (unsigned)pr_radio_page * PR_RADIO_PER_PAGE;
    unsigned end_idx = start_idx + PR_RADIO_PER_PAGE;
    if (end_idx > total_stations) end_idx = total_stations;

    /* Populate station rows for current page - 6 stations comfortably visible on screen */
    for (unsigned i = start_idx; i < end_idx; i++) {
      unsigned slot = i - start_idx;
      const char *st_text = "";
      if (dyn_st) st_text = dyn_st[i].name;
      else if (built_st) st_text = built_st[i].name;

      unsigned n = child_count(pr_list);
      original_row(pr_list, st_text, slot, pr_station_click, item_layout);
      pr_row_objs[slot] = child(pr_list, (int)n);
      pr_setup_row(pr_row_objs[slot], 38, 1, 0);
    }

    /* Side navigation icon buttons with (x/y) page indication */
    if (total_pages > 1) {
      pr_create_nav_buttons(pr_radio_page, total_pages, pr_prev_radio_click, pr_next_radio_click);
    }
  }

  /* Bottom spacer so scrolling can pull all content comfortably above bottom curve */
  LvObj *footer = lv_create(pr_list);
  if (footer) {
    lv_size(footer, 360, 16);
    lv_border(footer, 0, 0);
    bg_opa(footer, 0, 0);
    pad_top(footer, 0, 0);
    pad_bottom(footer, 0, 0);
    lv_clear_flag(footer, DISC_LV_CLICKABLE | DISC_LV_SCROLLABLE);
    lv_add_flag(footer, 0x4000);
  }

  screen_load(pr_page);
}

/* Polling hook invoked inside main poll() */
static void pr_poll(void) {
  LvObj *media = LV_GLOBAL(0x8de1b0);
  if (media && lv_valid(media)) {
    const char *names[2] = {"播客", "网络收音机"};
    for (unsigned i = 0; i < 2; i++) {
      if (!pr_menu_rows[i]) {
        unsigned n = child_count(media);
        original_row(media, names[i], i + 3, pr_entry_click, 0);
        if (child_count(media) > n) {
          pr_menu_rows[i] = child(media, -1);
          protect_tap(pr_menu_rows[i]);
          lv_add_cb(pr_menu_rows[i], pr_deleted, DISC_LV_DELETE, 0);
        }
      }
    }

    /* Position 播客 & 网络收音机 directly below DLNA and above 网易云音乐 */
    if (pr_menu_rows[0] && pr_menu_rows[1]) {
      int dlna_idx = -1;
      int netease_idx = -1;
      unsigned m_count = child_count(media);
      for (unsigned k = 0; k < m_count; k++) {
        LvObj *c = child(media, (int)k);
        if (c == dlna_menu_row) dlna_idx = (int)k;
        if (c == online_rows[0]) netease_idx = (int)k;
      }
      int target_pos = -1;
      if (dlna_idx >= 0) target_pos = dlna_idx + 1;
      else if (netease_idx >= 0) target_pos = netease_idx;

      if (target_pos >= 0) {
        if (child(media, target_pos) != pr_menu_rows[0]) {
          move_index(pr_menu_rows[0], target_pos);
        }
        if (child(media, target_pos + 1) != pr_menu_rows[1]) {
          move_index(pr_menu_rows[1], target_pos + 1);
        }
      }
    }
  }

  if (pr_open_pending) {
    pr_open_pending = 0;
    pr_ep_page = 0;
    pr_radio_page = 0;
    pr_make(pr_requested_view);
  }

  if (pr_back_pending) {
    pr_back_pending = 0;
    if (pr_current_view == PR_VIEW_PODCAST_EPS) {
      pr_ep_page = 0;
      pr_make(PR_VIEW_PODCAST_FEEDS);
    } else if (pr_current_view == PR_VIEW_RADIO_STATIONS) {
      pr_radio_page = 0;
      pr_make(PR_VIEW_RADIO_CATS);
    } else if (pr_current_view != PR_VIEW_NONE) {
      pr_clear();
      if (media && lv_valid(media)) screen_load(media);
    }
  }
}

#endif /* DISC_PODCAST_RADIO_UI_H */
