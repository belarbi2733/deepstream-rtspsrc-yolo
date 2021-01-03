#ifndef __NVGSTDS_APP_H__
#define __NVGSTDS_APP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <gst/gst.h>
#include <stdio.h>
#include <sys/time.h>
#include "../common/includes/gst-nvdssr.h"
#include "../common/includes/deepstream_common.h"
#include "../common/includes/nvdsgstutils.h"

#define NVGSTDS_ERR_MSG_V(msg, ...) \
    g_print("** ERROR: <%s:%d>: " msg "\n", __func__, __LINE__, ##__VA_ARGS__)

#define NVGSTDS_INFO_MSG_V(msg, ...) \
    g_print("** INFO: <%s:%d>: " msg "\n", __func__, __LINE__, ##__VA_ARGS__)

#define NVGSTDS_WARN_MSG_V(msg, ...) \
    g_print("** WARN: <%s:%d>: " msg "\n", __func__, __LINE__, ##__VA_ARGS__)

#define MAX_SOURCE_BINS 1024
#define SOURCE_RESET_INTERVAL_SEC 30

typedef struct _AppCtx AppCtx;

typedef enum
{
  NV_DS_SOURCE_CAMERA_V4L2 = 1,
  NV_DS_SOURCE_URI,
  NV_DS_SOURCE_URI_MULTIPLE,
  NV_DS_SOURCE_RTSP,
  NV_DS_SOURCE_CAMERA_CSI,
} NvDsSourceType;


typedef struct
{
  GstElement *bin;
  GstElement *src_elem;
  GstElement *cap_filter;
  GstElement *cap_filter1;
  GstElement *depay;
  GstElement *parser;
  GstElement *enc_que;
  GstElement *dec_que;
  GstElement *decodebin;
  GstElement *enc_filter;
  GstElement *encbin_que;
  GstElement *tee;
  GstElement *tee_rtsp_pre_decode;
  GstElement *fakesink_queue;
  GstElement *fakesink;
  GstElement *nvvidconv;

  gboolean do_record;
  guint64 pre_event_rec;
  GMutex bin_lock;
  guint bin_id;
  gint rtsp_reconnect_interval_sec;
  struct timeval last_buffer_time;
  struct timeval last_reconnect_time;
  gulong src_buffer_probe;
  gulong rtspsrc_monitor_probe;
  gpointer bbox_meta;
  GstBuffer *inbuf;
  gchar *location;
  gchar *file;
  gchar *direction;
  gint latency;
  gboolean got_key_frame;
  gboolean eos_done;
  gboolean reset_done;
  gboolean live_source;
  gboolean reconfiguring;
  gboolean async_state_watch_running;
  // NvDsDewarperBin dewarper_bin;
  gulong probe_id;
  guint64 accumulated_base;
  guint64 prev_accumulated_base;
  guint source_id;
  // NvDsSourceConfig *config;
  gpointer recordCtx;
} NvDsSrcBin;

typedef struct
{
  GstElement *bin;
  GstElement *streammux;
  GThread *reset_thread;
  NvDsSrcBin sub_bins[MAX_SOURCE_BINS];
  guint num_bins;
  guint num_fr_on;
  gboolean live_source;
  gulong nvstreammux_eosmonitor_probe;
} NvDsSrcParentBin;

typedef struct
{
  // gulong primary_bbox_buffer_probe_id;
  guint bus_id;
  GstElement *pipeline;
  NvDsSrcParentBin multi_src_bin;
  // NvDsInstanceBin instance_bins[MAX_SOURCE_BINS];
  // NvDsInstanceBin demux_instance_bins[MAX_SOURCE_BINS];
  // NvDsInstanceBin common_elements;
  GstElement *tiler_tee;
  // NvDsTiledDisplayBin tiled_display_bin;
  GstElement *demuxer;
  // NvDsDsExampleBin dsexample_bin;
  AppCtx *appCtx;
} NvDsPipeline;

struct _AppCtx
{
//   gboolean version;
//   gboolean cintr;
//   gboolean show_bbox_text;
//   gboolean seeking;
  gboolean quit;
//   gint person_class_id;
//   gint car_class_id;
  gint return_value;
//   guint index;
//   gint active_source_index;

  GMutex app_lock;
  GCond app_cond;

  NvDsPipeline pipeline;
//   NvDsConfig config;
//   NvDsConfig override_config;
//   NvDsInstanceData instance_data[MAX_SOURCE_BINS];
//   NvDsC2DContext *c2d_ctx[MAX_MESSAGE_CONSUMERS];
//   NvDsAppPerfStructInt perf_struct;
//   bbox_generated_callback bbox_generated_post_analytics_cb;
//   bbox_generated_callback all_bbox_generated_cb;
//   overlay_graphics_callback overlay_graphics_cb;
//   NvDsFrameLatencyInfo *latency_info;
//   GMutex latency_lock;
//   GThread *ota_handler_thread;
//   guint ota_inotify_fd;
//   guint ota_watch_desc;
};

#ifdef __cplusplus
}
#endif

#endif