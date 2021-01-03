#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <gst/rtsp/gstrtsptransport.h>
#include <gst/rtp/gstrtcpbuffer.h>
#include <glib.h>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "cuda_runtime_api.h"
#include "gstnvdsmeta.h"
#include "gstnvdsinfer.h"
#include "nvdsinfer_custom_impl.h"
#include "nvds_version.h"
#include "main.h"

#define NVINFER_PLUGIN "nvinfer"
#define INFER_PGIE_CONFIG_FILE "config/pgie_config.txt"

#define MUXER_OUTPUT_WIDTH 1920
#define MUXER_OUTPUT_HEIGHT 1080
#define MUXER_BATCH_TIMEOUT_USEC 40000
#define MEMORY_FEATURES "memory:NVMM"
#define TILED_OUTPUT_WIDTH 1920
#define TILED_OUTPUT_HEIGHT 1080
#define PGIE_CLASS_ID_0 0
#define PGIE_CLASS_ID_1 1
#define PGIE_CLASS_ID_2 2
#define PGIE_CLASS_ID_3 3
#define PGIE_CLASS_ID_4 4
#define PGIE_CLASS_ID_5 5
#define PGIE_CLASS_ID_6 6
#define PGIE_CLASS_ID_7 7
#define PGIE_CLASS_ID_8 8
#define PGIE_CLASS_ID_9 9
#define PGIE_DETECTED_CLASS_NUM 10
#define MAX_SINK_BINS (1024)
#define MAX_INSTANCES 128
#define MAX_DISPLAY_LEN 64

AppCtx *appCtx[MAX_INSTANCES];

GMainLoop *loop = NULL;
static gboolean install_mux_eosmonitor_probe = FALSE;
gint frame_number = 0;
const gchar pgie_classes_str[PGIE_DETECTED_CLASS_NUM][32] =
    { "ibokisago",
       "karasu", 
       "hototogisu", 
       "aramujiro", 
       "mate", 
       "tsumeta", 
       "baka", 
       "shiofuki", 
       "kagami", 
       "asari" };

unsigned int nvds_lib_major_version = NVDS_VERSION_MAJOR;
unsigned int nvds_lib_minor_version = NVDS_VERSION_MINOR;
static GstRTSPServer *server [MAX_SINK_BINS];
static guint server_count = 0;
static GMutex server_cnt_lock;
guint num_sources = 0;

static GstPadProbeReturn
osd_sink_pad_buffer_probe (GstPad * pad, GstPadProbeInfo * info, gpointer u_data)
{
  GstBuffer *buf = (GstBuffer *) info->data;
  guint num_rects = 0;
  NvDsObjectMeta *obj_meta = NULL;
  guint count_0 = 0;
  guint count_1 = 0;
  guint count_2 = 0;
  guint count_3 = 0;
  guint count_4 = 0;
  guint count_5 = 0;
  guint count_6 = 0;
  guint count_7 = 0;
  guint count_8 = 0;
  guint count_9 = 0;
  guint truck_count = 0;
  NvDsMetaList *l_frame = NULL;
  NvDsMetaList *l_obj = NULL;
  NvDsMetaList *l_cls = NULL;
  NvDsDisplayMeta *display_meta3 = NULL;
  NvDsBatchMeta *batch_meta = gst_buffer_get_nvds_batch_meta (buf);

  for (l_frame = batch_meta->frame_meta_list; l_frame != NULL;
      l_frame = l_frame->next) {
    NvDsFrameMeta *frame_meta = (NvDsFrameMeta *) (l_frame->data);
    int offset = 0;
    for (l_obj = frame_meta->obj_meta_list; l_obj != NULL; l_obj = l_obj->next) {
      obj_meta = (NvDsObjectMeta *) (l_obj->data);
      display_meta3 = nvds_acquire_display_meta_from_pool (batch_meta);
      NvOSD_TextParams *txt_params3 = &display_meta3->text_params[0];
      for (l_cls = obj_meta->classifier_meta_list; l_cls != NULL; l_cls = l_cls->next) {
        NvDsClassifierMeta *cls_meta = (NvDsClassifierMeta *) (l_cls->data);
      }
      switch (obj_meta->class_id) {
        case PGIE_CLASS_ID_0:
          count_0++;
          num_rects++;
          txt_params3->text_bg_clr.red = 0.0;
          txt_params3->text_bg_clr.green = 0.0;
          txt_params3->text_bg_clr.blue = 0.0;
          break;
        case PGIE_CLASS_ID_1:
          count_1++;
          num_rects++;
          txt_params3->text_bg_clr.red = 0.5;
          txt_params3->text_bg_clr.green = 0.0;
          txt_params3->text_bg_clr.blue = 0.0;
          break;
        case PGIE_CLASS_ID_2:
          count_2++;
          num_rects++;
          txt_params3->text_bg_clr.red = 0.0;
          txt_params3->text_bg_clr.green = 0.5;
          txt_params3->text_bg_clr.blue = 0.0;
          break;
        case PGIE_CLASS_ID_3:
          count_3++;
          num_rects++;
          txt_params3->text_bg_clr.red = 1.0;
          txt_params3->text_bg_clr.green = 1.0;
          txt_params3->text_bg_clr.blue = 1.0;
          break;
        case PGIE_CLASS_ID_4:
          count_4++;
          num_rects++;
          txt_params3->text_bg_clr.red = 0.0;
          txt_params3->text_bg_clr.green = 1.0;
          txt_params3->text_bg_clr.blue = 1.0;
          break;
        case PGIE_CLASS_ID_5:
          count_5++;
          num_rects++;
          txt_params3->text_bg_clr.red = 1.0;
          txt_params3->text_bg_clr.green = 0.0;
          txt_params3->text_bg_clr.blue = 1.0;
          break;
        case PGIE_CLASS_ID_6:
          count_6++;
          num_rects++;
          txt_params3->text_bg_clr.red = 1.0;
          txt_params3->text_bg_clr.green = 1.0;
          txt_params3->text_bg_clr.blue = 0.0;
          break;
        case PGIE_CLASS_ID_7:
          count_7++;
          num_rects++;
          txt_params3->text_bg_clr.red = 0.0;
          txt_params3->text_bg_clr.green = 0.0;
          txt_params3->text_bg_clr.blue = 1.0;
          break;
        case PGIE_CLASS_ID_8:
          count_8++;
          num_rects++;
          txt_params3->text_bg_clr.red = 0.0;
          txt_params3->text_bg_clr.green = 1.0;
          txt_params3->text_bg_clr.blue = 0.0;
          break;
        case PGIE_CLASS_ID_9:
          count_9++;
          num_rects++;
          txt_params3->text_bg_clr.red = 1.0;
          txt_params3->text_bg_clr.green = 0.0;
          txt_params3->text_bg_clr.blue = 0.0;
          break;
      }

      NvOSD_RectParams rect_params = obj_meta->rect_params;
      float left = rect_params.left;
      float top = rect_params.top;
      float width = rect_params.width;
      float height = rect_params.height;
      float confidence = obj_meta->confidence;

      std::string result;
      char result_0[32];
      char result_1[32];
      display_meta3->num_labels = 1;
      txt_params3->display_text = (gchar *) g_malloc0 (MAX_DISPLAY_LEN);

      snprintf(result_0, 32, "%s ", pgie_classes_str[obj_meta->class_id]);
      snprintf(result_1, 32, "%.0f％", confidence * 100);

      result.append(result_0);
      result.append(result_1);

      offset = snprintf (
        txt_params3->display_text, MAX_DISPLAY_LEN, "%s", result.c_str()
      );
      /* Now set the offsets where the string should appear */
      txt_params3->x_offset = left;
      txt_params3->y_offset = top-40;

      /* Font , font-color and font-size */
      txt_params3->font_params.font_name = (gchar *) "Serif";
      txt_params3->font_params.font_size = 20;
      txt_params3->font_params.font_color.red = 0.0;
      txt_params3->font_params.font_color.green = 0.0;
      txt_params3->font_params.font_color.blue = 0.0;
      txt_params3->font_params.font_color.alpha = 1.0;

      /* Text background color */
      txt_params3->set_bg_clr = 1;
      // txt_params3->text_bg_clr.red = 1.0;
      // txt_params3->text_bg_clr.green = 1.0;
      // txt_params3->text_bg_clr.blue = 1.0;
      txt_params3->text_bg_clr.alpha = 0.5;

      nvds_add_display_meta_to_frame (frame_meta, display_meta3);

    }
    if (num_rects > 0) {
      g_print ("Frame Number = %d Number of objects = %d "
          "ID_0: %d ID_1: %d ID_2: %d ID_3: %d ID_4: %d ID_5: %d ID_6: %d ID_7: %d ID_8: %d ID_9: %d \n",
          frame_meta->frame_num, num_rects, count_0, count_1, count_2, count_3, count_4, count_5, count_6, count_7, count_8, count_9);  
    }
  }

  frame_number++;
  return GST_PAD_PROBE_OK;
}

gboolean
link_element_to_streammux_sink_pad (GstElement *streammux, GstElement *elem,
    gint index)
{
  gboolean ret = FALSE;
  GstPad *mux_sink_pad = NULL;
  GstPad *src_pad = NULL;
  gchar pad_name[16];

  if (index >= 0) {
    g_snprintf (pad_name, 16, "sink_%u", index);
    pad_name[15] = '\0';
  } else {
    strcpy (pad_name, "sink_%u");
  }

  mux_sink_pad = gst_element_get_request_pad (streammux, pad_name);
  if (!mux_sink_pad) {
    NVGSTDS_ERR_MSG_V ("Failed to get sink pad from streammux");
    goto done;
  }

  src_pad = gst_element_get_static_pad (elem, "src");
  if (!src_pad) {
    NVGSTDS_ERR_MSG_V ("Failed to get src pad from '%s'",
                        GST_ELEMENT_NAME (elem));
    goto done;
  }

  if (gst_pad_link (src_pad, mux_sink_pad) != GST_PAD_LINK_OK) {
    NVGSTDS_ERR_MSG_V ("Failed to link '%s' and '%s'", GST_ELEMENT_NAME (streammux),
        GST_ELEMENT_NAME (elem));
    goto done;
  }

  ret = TRUE;

done:
  if (mux_sink_pad) {
    gst_object_unref (mux_sink_pad);
  }
  if (src_pad) {
    gst_object_unref (src_pad);
  }
  return ret;
}

gboolean
link_element_to_tee_src_pad (GstElement *tee, GstElement *sinkelem)
{
  gboolean ret = FALSE;
  GstPad *tee_src_pad = NULL;
  GstPad *sinkpad = NULL;
  GstPadTemplate *padtemplate = NULL;

  padtemplate = (GstPadTemplate *)
      gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS (tee),
      "src_%u");
  tee_src_pad = gst_element_request_pad (tee, padtemplate, NULL, NULL);
  if (!tee_src_pad) {
    NVGSTDS_ERR_MSG_V ("Failed to get src pad from tee");
    goto done;
  }

  sinkpad = gst_element_get_static_pad (sinkelem, "sink");
  if (!sinkpad) {
    NVGSTDS_ERR_MSG_V ("Failed to get sink pad from '%s'",
        GST_ELEMENT_NAME (sinkelem));
    goto done;
  }

  if (gst_pad_link (tee_src_pad, sinkpad) != GST_PAD_LINK_OK) {
    NVGSTDS_ERR_MSG_V ("Failed to link '%s' and '%s'", GST_ELEMENT_NAME (tee),
        GST_ELEMENT_NAME (sinkelem));
    goto done;
  }

  ret = TRUE;

done:
  if (tee_src_pad) {
    gst_object_unref (tee_src_pad);
  }
  if (sinkpad) {
    gst_object_unref (sinkpad);
  }
  return ret;
}

/**
 * Function called at regular interval when source bin is
 * changing state async. This function watches the state of
 * the source bin and sets it to PLAYING if the state of source
 * bin stops at PAUSED when changing state ASYNC.
 */
static gboolean
watch_source_async_state_change (gpointer data)
{
  NvDsSrcBin *src_bin = (NvDsSrcBin *) data;
  GstState state, pending;
  GstStateChangeReturn ret;

  ret = gst_element_get_state (src_bin->bin, &state, &pending, 0);

  // Bin is still changing state ASYNC. Wait for some more time.
  if (ret == GST_STATE_CHANGE_ASYNC)
    return TRUE;

  // Bin state change failed / failed to get state
  if (ret == GST_STATE_CHANGE_FAILURE) {
    src_bin->async_state_watch_running = FALSE;
    return FALSE;
  }

  // Bin successfully changed state to PLAYING. Stop watching state
  if (state == GST_STATE_PLAYING) {
    src_bin->reconfiguring = FALSE;
    src_bin->async_state_watch_running = FALSE;
    return FALSE;
  }

  // Bin has stopped ASYNC state change but has not gone into
  // PLAYING. Expliclity set state to PLAYING and keep watching
  // state
  gst_element_set_state (src_bin->bin, GST_STATE_PLAYING);

  return TRUE;
}

/**
 * Probe function to monitor data output from rtspsrc.
 */
static GstPadProbeReturn
rtspsrc_monitor_probe_func (GstPad * pad, GstPadProbeInfo * info,
    gpointer u_data)
{
  NvDsSrcBin *bin = (NvDsSrcBin *) u_data;
  if (info->type & GST_PAD_PROBE_TYPE_BUFFER) {
    g_mutex_lock(&bin->bin_lock);
    gettimeofday (&bin->last_buffer_time, NULL);
    g_mutex_unlock(&bin->bin_lock);
  }
  return GST_PAD_PROBE_OK;
}

gboolean
reset_source_pipeline (gpointer data)
{
  NvDsSrcBin *src_bin = (NvDsSrcBin *) data;
  GstState state, pending;
  GstStateChangeReturn ret;

  g_mutex_lock(&src_bin->bin_lock);
  gettimeofday (&src_bin->last_buffer_time, NULL);
  gettimeofday (&src_bin->last_reconnect_time, NULL);
  g_mutex_unlock(&src_bin->bin_lock);

  if (gst_element_set_state (src_bin->bin,
          GST_STATE_NULL) == GST_STATE_CHANGE_FAILURE) {
    GST_ERROR_OBJECT (src_bin->bin, "Can't set source bin to NULL");
    return FALSE;
  }
  NVGSTDS_INFO_MSG_V ("Resetting source %d", src_bin->bin_id);

  // GST_CAT_INFO (NVDS_APP, "Reset source pipeline %s %p\n,", __func__, src_bin);
  if (!gst_element_sync_state_with_parent (src_bin->bin)) {
    GST_ERROR_OBJECT (src_bin->bin, "Couldn't sync state with parent");
  }

  ret = gst_element_get_state (src_bin->bin, &state, &pending, 0);

  if (ret == GST_STATE_CHANGE_ASYNC || ret == GST_STATE_CHANGE_NO_PREROLL) {
    if (!src_bin->async_state_watch_running)
      g_timeout_add (20, watch_source_async_state_change, src_bin);
    src_bin->async_state_watch_running = TRUE;
    src_bin->reconfiguring = TRUE;
  } else if (ret == GST_STATE_CHANGE_SUCCESS && state == GST_STATE_PLAYING) {
    src_bin->reconfiguring = FALSE;
  }
  return FALSE;
}

/**
 * Function called at regular interval to check if NV_DS_SOURCE_RTSP type
 * source in the pipeline is down / disconnected. This function try to
 * reconnect the source by resetting that source pipeline.
 */
static gboolean
watch_source_status (gpointer data)
{
  NvDsSrcBin *src_bin = (NvDsSrcBin *) data;
  struct timeval current_time;
  gettimeofday (&current_time, NULL);
  static struct timeval last_reset_time_global = {0, 0};
  gdouble time_diff_msec_since_last_reset =
      1000.0 * (current_time.tv_sec - last_reset_time_global.tv_sec) +
      (current_time.tv_usec - last_reset_time_global.tv_usec) / 1000.0;

  if (src_bin->reconfiguring) {
    guint time_since_last_reconnect_sec =
        current_time.tv_sec - src_bin->last_reconnect_time.tv_sec;
    if (time_since_last_reconnect_sec >= SOURCE_RESET_INTERVAL_SEC) {
      if (time_diff_msec_since_last_reset > 3000) {
        last_reset_time_global = current_time;
        // source is still not up, reconfigure it again.
        reset_source_pipeline (src_bin);
      }
    }
  } else {
    gint time_since_last_buf_sec = 0;
    g_mutex_lock (&src_bin->bin_lock);
    if (src_bin->last_buffer_time.tv_sec != 0) {
      time_since_last_buf_sec =
          current_time.tv_sec - src_bin->last_buffer_time.tv_sec;
    }
    g_mutex_unlock (&src_bin->bin_lock);

    // Reset source bin if no buffers are received in the last
    // `rtsp_reconnect_interval_sec` seconds.
    if (src_bin->rtsp_reconnect_interval_sec > 0 &&
            time_since_last_buf_sec >= src_bin->rtsp_reconnect_interval_sec) {
      if (time_diff_msec_since_last_reset > 3000) {
        last_reset_time_global = current_time;

        NVGSTDS_WARN_MSG_V ("No data from source %d since last %u sec. Trying reconnection",
              src_bin->bin_id, time_since_last_buf_sec);
        reset_source_pipeline (src_bin);
      }
    }

  }
  return TRUE;
}

/**
 * callback function to receive messages from components
 * in the pipeline.
 */
static gboolean
bus_callback (GstBus * bus, GstMessage * message, gpointer data)
{
  AppCtx *appCtx = (AppCtx *) data;
  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_INFO:{
      GError *error = NULL;
      gchar *debuginfo = NULL;
      gst_message_parse_info (message, &error, &debuginfo);
      g_printerr ("INFO from %s: %s\n",
          GST_OBJECT_NAME (message->src), error->message);
      if (debuginfo) {
        g_printerr ("Debug info: %s\n", debuginfo);
      }
      g_error_free (error);
      g_free (debuginfo);
      break;
    }
    case GST_MESSAGE_WARNING:{
      GError *error = NULL;
      gchar *debuginfo = NULL;
      gst_message_parse_warning (message, &error, &debuginfo);
      g_printerr ("WARNING from %s: %s\n",
          GST_OBJECT_NAME (message->src), error->message);
      if (debuginfo) {
        g_printerr ("Debug info: %s\n", debuginfo);
      }
      g_error_free (error);
      g_free (debuginfo);
      break;
    }
    case GST_MESSAGE_ERROR:{
      GError *error = NULL;
      gchar *debuginfo = NULL;
      guint i = 0;
      gst_message_parse_error (message, &error, &debuginfo);
      g_printerr ("ERROR from %s: %s\n",
          GST_OBJECT_NAME (message->src), error->message);
      if (debuginfo) {
        g_printerr ("Debug info: %s\n", debuginfo);
      }
      NvDsSrcParentBin *bin = &appCtx->pipeline.multi_src_bin;
      GstElement *msg_src_elem = (GstElement *) GST_MESSAGE_SRC (message);
      gboolean bin_found = FALSE;
      /* Find the source bin which generated the error. */
      while (msg_src_elem && !bin_found) {
        for (i = 0; i < bin->num_bins && !bin_found; i++) {
          printf("msg_src_elem: %s\n", gst_element_get_name(msg_src_elem));

          if (!bin->sub_bins[i].src_elem) {
            printf("bin->sub_bins[i].src_elem is empty\n");
            goto done;
          }
          if (bin->sub_bins[i].src_elem == msg_src_elem ||
                  bin->sub_bins[i].bin == msg_src_elem) {
            bin_found = TRUE;
            printf("bin to reset is found\n");
            break;
          }
        }
        msg_src_elem = GST_ELEMENT_PARENT (msg_src_elem);
      }

      if (i != bin->num_bins) {
        // Error from one of RTSP source.
        NvDsSrcBin *subBin = &bin->sub_bins[i];
        if (!subBin->reconfiguring ||
            g_strrstr(debuginfo, "500 (Internal Server Error)")) {
          subBin->reconfiguring = TRUE;
          printf("trying to call reset_source_pipeline\n");
          g_timeout_add (0, reset_source_pipeline, subBin);
        }
        g_error_free (error);
        g_free (debuginfo);
        return TRUE;
      }

    done:
      g_error_free (error);
      g_free (debuginfo);
      appCtx->return_value = -1;
      appCtx->quit = TRUE;
      break;
    }
    case GST_MESSAGE_STATE_CHANGED:{
      GstState oldstate, newstate;
      gst_message_parse_state_changed (message, &oldstate, &newstate, NULL);
      if (GST_ELEMENT (GST_MESSAGE_SRC (message)) == appCtx->pipeline.pipeline) {
        switch (newstate) {
          case GST_STATE_PLAYING:
            NVGSTDS_INFO_MSG_V ("Pipeline running\n");
            GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (appCtx->
                    pipeline.pipeline), GST_DEBUG_GRAPH_SHOW_ALL,
                "ds-app-playing");
            break;
          case GST_STATE_PAUSED:
            if (oldstate == GST_STATE_PLAYING) {
              NVGSTDS_INFO_MSG_V ("Pipeline paused\n");
            }
            break;
          case GST_STATE_READY:
            GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (appCtx->pipeline.
                    pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "ds-app-ready");
            if (oldstate == GST_STATE_NULL) {
              NVGSTDS_INFO_MSG_V ("Pipeline ready\n");
            } else {
              NVGSTDS_INFO_MSG_V ("Pipeline stopped\n");
            }
            break;
          case GST_STATE_NULL:
            g_mutex_lock (&appCtx->app_lock);
            g_cond_broadcast (&appCtx->app_cond);
            g_mutex_unlock (&appCtx->app_lock);
            break;
          default:
            break;
        }
      }
      break;
    }
    case GST_MESSAGE_EOS:{
      /*
       * In normal scenario, this would use g_main_loop_quit() to exit the
       * loop and release the resources. Since this application might be
       * running multiple pipelines through configuration files, it should wait
       * till all pipelines are done.
       */
      NVGSTDS_INFO_MSG_V ("Received EOS. Exiting ...\n");
      appCtx->quit = TRUE;
      return FALSE;
      break;
    }
    default:
      break;
  }
  return TRUE;
}


/* Returning FALSE from this callback will make rtspsrc ignore the stream.
 * Ignore audio and add the proper depay element based on codec. */
static gboolean
cb_rtspsrc_select_stream (GstElement *rtspsrc, guint num, GstCaps *caps,
        gpointer user_data)
{
  GstStructure *str = gst_caps_get_structure (caps, 0);
  const gchar *media = gst_structure_get_string (str, "media");
  const gchar *encoding_name = gst_structure_get_string (str, "encoding-name");
  gchar elem_name[50];
  NvDsSrcBin *bin = (NvDsSrcBin *) user_data;
  gboolean ret = FALSE;

  gboolean is_video = (!g_strcmp0 (media, "video"));

  if (!is_video)
    return FALSE;

  /* Create and add depay element only if it is not created yet. */
  if (!bin->depay) {
    g_snprintf (elem_name, sizeof (elem_name), "depay_elem%d", bin->bin_id);

    /* Add the proper depay element based on codec. */
    if (!g_strcmp0 (encoding_name, "H264")) {
      bin->depay = gst_element_factory_make ("rtph264depay", elem_name);
      g_snprintf (elem_name, sizeof (elem_name), "h264parse_elem%d", bin->bin_id);
      bin->parser = gst_element_factory_make ("h264parse", elem_name);
    } else if (!g_strcmp0 (encoding_name, "H265")) {
      printf("passes rtph265depay\n");
      bin->depay = gst_element_factory_make ("rtph265depay", elem_name);
      g_snprintf (elem_name, sizeof (elem_name), "h265parse_elem%d", bin->bin_id);
      bin->parser = gst_element_factory_make ("h265parse", elem_name);
    } else {
      NVGSTDS_WARN_MSG_V ("%s not supported", encoding_name);
      return FALSE;
    }

    if (!bin->depay) {
      NVGSTDS_ERR_MSG_V ("Failed to create '%s'", elem_name);
      return FALSE;
    }
    gst_bin_add_many (GST_BIN (bin->bin), bin->depay, bin->parser, NULL);

    NVGSTDS_LINK_ELEMENT (bin->depay, bin->parser);
    NVGSTDS_LINK_ELEMENT (bin->parser, bin->tee_rtsp_pre_decode);

    if (!gst_element_sync_state_with_parent (bin->depay)) {
      NVGSTDS_ERR_MSG_V ("'%s' failed to sync state with parent", elem_name);
      return FALSE;
    }
    gst_element_sync_state_with_parent (bin->parser);
  }

  ret = TRUE;
done:
  return ret;
}

static void
cb_newpad2 (GstElement * decodebin, GstPad * pad, gpointer data)
{
  GstCaps *caps = gst_pad_query_caps (pad, NULL);
  const GstStructure *str = gst_caps_get_structure (caps, 0);
  const gchar *name = gst_structure_get_name (str);
  gint input_width = 1920;
  gint input_height = 1080;
  gint input_fps_n = 1;
  gint input_fps_d = 1;
  if (!strncmp (name, "video", 5)) {
    NvDsSrcBin *bin = (NvDsSrcBin *) data;
    GstPad *sinkpad = gst_element_get_static_pad (bin->cap_filter, "sink");
    if (gst_pad_link (pad, sinkpad) != GST_PAD_LINK_OK) {

      NVGSTDS_ERR_MSG_V ("Failed to link decodebin to pipeline");
    } else {
      gst_structure_get_int (str, "width", &input_width);
      gst_structure_get_int (str, "height", &input_height);
      gst_structure_get_fraction (str, "framerate", &input_fps_n, &input_fps_d);
    }
    gst_object_unref (sinkpad);
  }
  gst_caps_unref (caps);
}

static void
cb_newpad3 (GstElement * decodebin, GstPad * pad, gpointer data)
{
  GstCaps *caps = gst_pad_query_caps (pad, NULL);
  const GstStructure *str = gst_caps_get_structure (caps, 0);
  const gchar *name = gst_structure_get_name (str);
  if (g_strrstr (name, "x-rtp")) {
    NvDsSrcBin *bin = (NvDsSrcBin *) data;
    GstPad *sinkpad = gst_element_get_static_pad (bin->depay, "sink");
    if (gst_pad_link (pad, sinkpad) != GST_PAD_LINK_OK) {

      NVGSTDS_ERR_MSG_V ("Failed to link depay loader to rtsp src");
    }
    gst_object_unref (sinkpad);
  }
  gst_caps_unref (caps);
}

static void
cb_newpad (GstElement * decodebin, GstPad * decoder_src_pad, gpointer data)
{
  g_print ("In cb_newpad\n");
  GstCaps *caps = gst_pad_get_current_caps (decoder_src_pad);
  const GstStructure *str = gst_caps_get_structure (caps, 0);
  const gchar *name = gst_structure_get_name (str);
  GstElement *source_bin = (GstElement *) data;
  GstCapsFeatures *features = gst_caps_get_features (caps, 0);

  if (!strncmp (name, "video", 5)) {
    if (gst_caps_features_contains (features, MEMORY_FEATURES)) {
      GstPad *bin_ghost_pad = gst_element_get_static_pad (source_bin, "src");
      if (!gst_ghost_pad_set_target (GST_GHOST_PAD (bin_ghost_pad),
              decoder_src_pad)) {
        g_printerr ("Failed to link decoder src pad to source bin ghost pad\n");
      }
      gst_object_unref (bin_ghost_pad);
    } else {
      g_printerr ("Error: Decodebin did not pick nvidia decoder plugin.\n");
    }
  }
}

static void
decodebin_child_added (GstChildProxy * child_proxy, GObject * object,
    gchar * name, gpointer user_data)
{
  NvDsSrcBin *bin = (NvDsSrcBin *) user_data;

  if (g_strrstr (name, "decodebin") == name) {
    g_signal_connect (G_OBJECT (object), "child-added",
        G_CALLBACK (decodebin_child_added), user_data);
  }
  if ((g_strrstr (name, "h264parse") == name) ||
      (g_strrstr (name, "h265parse") == name)) {
      g_object_set (object, "config-interval", -1, NULL);
  }
  if (g_strrstr (name, "fakesink") == name) {
      g_object_set (object, "enable-last-sample", FALSE, NULL);
  }
  if (g_strrstr (name, "nvcuvid") == name) {
    g_object_set (object, "gpu-id", 0, NULL);
  }
  if (g_strstr_len (name, -1, "omx") == name) {
    g_object_set (object, "skip-frames", 2, NULL);
    g_object_set (object, "disable-dvfs", TRUE, NULL);
  }
  if (g_strstr_len (name, -1, "nvjpegdec") == name) {
    g_object_set (object, "DeepStream", TRUE, NULL);
  }
  if (g_strstr_len (name, -1, "nvv4l2decoder") == name) {
#ifdef __aarch64__
    g_object_set (object, "enable-max-performance", TRUE, NULL);
#endif
  }
done:
  return;
}

static gboolean
create_rtsp_src_bin (guint index, gchar * location, NvDsSrcBin * bin)
{
  NvDsSRContext *ctx = NULL;
  gboolean ret = FALSE;
  gchar elem_name[50];
  GstCaps *caps = NULL;
  GstCapsFeatures *feature = NULL;

  bin->rtsp_reconnect_interval_sec = 10;

  g_snprintf (elem_name, sizeof (elem_name), "src_elem%d", bin->bin_id);
  bin->src_elem = gst_element_factory_make ("rtspsrc", elem_name);
  if (!bin->src_elem) {
    NVGSTDS_ERR_MSG_V ("Failed to create '%s'", elem_name);
    goto done;
  }

  g_signal_connect (G_OBJECT(bin->src_elem), "select-stream",
                    G_CALLBACK(cb_rtspsrc_select_stream),
                    bin);

  g_object_set (G_OBJECT (bin->src_elem), "location", location, NULL);
  g_object_set (G_OBJECT (bin->src_elem), "latency", bin->latency, NULL);
  g_object_set (G_OBJECT (bin->src_elem), "drop-on-latency", TRUE, NULL);
  configure_source_for_ntp_sync (bin->src_elem);
  g_signal_connect (G_OBJECT (bin->src_elem), "pad-added",
      G_CALLBACK (cb_newpad3), bin);

  g_snprintf (elem_name, sizeof (elem_name), "tee_rtsp_elem%d", bin->bin_id);
  bin->tee_rtsp_pre_decode = gst_element_factory_make ("tee", elem_name);
  if (!bin->tee_rtsp_pre_decode) {
    NVGSTDS_ERR_MSG_V ("Failed to create '%s'", elem_name);
    goto done;
  }

  g_snprintf (elem_name, sizeof (elem_name), "dec_que%d", bin->bin_id);
  bin->dec_que = gst_element_factory_make ("queue", elem_name);
  if (!bin->dec_que) {
    NVGSTDS_ERR_MSG_V ("Failed to create '%s'", elem_name);
    goto done;
  }

  if (bin->rtsp_reconnect_interval_sec > 0) {
    printf("rtsp_reconnect_interval_sec: %u\n", bin->rtsp_reconnect_interval_sec);
    NVGSTDS_ELEM_ADD_PROBE (bin->rtspsrc_monitor_probe, bin->dec_que,
        "sink", rtspsrc_monitor_probe_func,
        GST_PAD_PROBE_TYPE_BUFFER,
        bin);
    install_mux_eosmonitor_probe = TRUE;
  }

  g_snprintf (elem_name, sizeof (elem_name), "decodebin_elem%d", bin->bin_id);
  bin->decodebin = gst_element_factory_make ("decodebin", elem_name);
  if (!bin->decodebin) {
    NVGSTDS_ERR_MSG_V ("Failed to create '%s'", elem_name);
    goto done;
  }

  g_signal_connect (G_OBJECT (bin->decodebin), "pad-added",
      G_CALLBACK (cb_newpad2), bin);
  g_signal_connect (G_OBJECT (bin->decodebin), "child-added",
      G_CALLBACK (decodebin_child_added), bin);


  g_snprintf (elem_name, sizeof (elem_name), "src_que%d", bin->bin_id);
  bin->cap_filter = gst_element_factory_make ("queue", elem_name);
  if (!bin->cap_filter) {
    NVGSTDS_ERR_MSG_V ("Failed to create '%s'", elem_name);
    goto done;
  }

  g_mutex_init (&bin->bin_lock);

  g_snprintf(elem_name, sizeof(elem_name), "nvvidconv_elem%d", bin->bin_id);
  bin->nvvidconv = gst_element_factory_make("nvvideoconvert", elem_name);
  if (!bin->nvvidconv)
  {
    NVGSTDS_ERR_MSG_V("Could not create element 'nvvidconv_elem'");
    goto done;
  }
  caps = gst_caps_new_empty_simple("video/x-raw");
  feature = gst_caps_features_new("memory:NVMM", NULL);
  gst_caps_set_features(caps, 0, feature);

  bin->cap_filter1 =
      gst_element_factory_make("capsfilter", "src_cap_filter_nvvidconv");
  if (!bin->cap_filter1)
  {
    NVGSTDS_ERR_MSG_V("Could not create 'queue'");
    goto done;
  }

  g_object_set(G_OBJECT(bin->cap_filter1), "caps", caps, NULL);
  gst_caps_unref(caps);

  gst_bin_add_many (GST_BIN(bin->bin), 
                    bin->src_elem, 
                    bin->tee_rtsp_pre_decode,
                    bin->dec_que, 
                    bin->decodebin, 
                    bin->cap_filter, 
                    bin->nvvidconv, 
                    bin->cap_filter1, 
                    NULL);

  link_element_to_tee_src_pad(bin->tee_rtsp_pre_decode, bin->dec_que);
  NVGSTDS_LINK_ELEMENT (bin->dec_que, bin->decodebin);

  if (ctx)
    link_element_to_tee_src_pad(bin->tee_rtsp_pre_decode, ctx->recordbin);
    
  NVGSTDS_LINK_ELEMENT (bin->cap_filter, bin->nvvidconv);
  NVGSTDS_LINK_ELEMENT (bin->nvvidconv, bin->cap_filter1);
  NVGSTDS_BIN_ADD_GHOST_PAD (bin->bin, bin->cap_filter1, "src");

  ret = TRUE;

  g_timeout_add (1000, watch_source_status, bin);

done:
  if (!ret) {
    NVGSTDS_ERR_MSG_V ("%s failed", __func__);
  }
  return ret;

}

/**
 * Probe function to drop EOS events from nvstreammux when RTSP sources
 * are being used so that app does not quit from EOS in case of RTSP
 * connection errors and tries to reconnect.
 */
static GstPadProbeReturn
nvstreammux_eosmonitor_probe_func (GstPad * pad, GstPadProbeInfo * info,
    gpointer u_data)
{
  if (info->type & GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM) {
    GstEvent *event = (GstEvent *) info->data;
    if (GST_EVENT_TYPE (event) == GST_EVENT_EOS)
      return GST_PAD_PROBE_DROP;
  }
  return GST_PAD_PROBE_OK;
}

static gboolean
start_rtsp_streaming (guint rtsp_port_num, guint updsink_port_num, guint64 udp_buffer_size)
{
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;
  char udpsrc_pipeline[512];
  char port_num_Str[64] = { 0 };

  if (udp_buffer_size == 0)
    udp_buffer_size = 512 * 1024;

  sprintf (udpsrc_pipeline,
      "( udpsrc name=pay0 port=%d buffer-size=%lu caps=\"application/x-rtp, media=video, "
      "clock-rate=90000, encoding-name=%s, payload=96 \" )",
      updsink_port_num, udp_buffer_size, "H264");

  sprintf (port_num_Str, "%d", rtsp_port_num);

  g_mutex_lock (&server_cnt_lock);

  server [server_count] = gst_rtsp_server_new ();
  g_object_set (server [server_count], "service", port_num_Str, NULL);

  mounts = gst_rtsp_server_get_mount_points (server [server_count]);

  factory = gst_rtsp_media_factory_new ();
  gst_rtsp_media_factory_set_launch (factory, udpsrc_pipeline);

  gst_rtsp_mount_points_add_factory (mounts, "/ds-test", factory);

  g_object_unref (mounts);

  gst_rtsp_server_attach (server [server_count], NULL);

  server_count++;

  g_mutex_unlock (&server_cnt_lock);

  g_print
      ("\n *** DeepStream: Launched RTSP Streaming at rtsp://localhost:%d/ds-test ***\n\n",
      rtsp_port_num);

  return TRUE;
}


static void
usage(const char *bin)
{
  g_printerr
    ("Usage: %s [-t infer-type]<elementary H264 file 1> ... <elementary H264 file n>\n",
      bin);
  g_printerr
    ("     -t infer-type: select form [infer, inferserver], infer by default\n");
}

int
main (int argc, char *argv[])
{
  printf("app start..\n");
  appCtx[0] = (AppCtx *) g_malloc0 (sizeof (AppCtx));
  NvDsPipeline *pipeline = &appCtx[0]->pipeline;
  GstElement *streammux = NULL, 
              *sink = NULL, 
              *pgie = NULL, 
              *nvosd = NULL, 
              *queue2 = NULL, 
              *nvvidconv2 = NULL, 
              *caps_filter = NULL, 
              *encoder = NULL, 
              *codecparser = NULL, 
              *rtppay = NULL, 
              *tiler = NULL;
  GstCaps *caps = NULL;
  GstBus *bus = NULL;
  guint i=0;
  guint tiler_rows, tiler_columns;
  GstPad *osd_sink_pad = NULL;
  guint pgie_batch_size;
  const char *infer_plugin = NVINFER_PLUGIN;
  gboolean ret = FALSE;

  /* define numsorces from argc */
  if (argc < 2) {
    g_printerr ("Usage: %s <uri>\n", argv[0]);
    return -1;
  }
  num_sources = argc - 1;
  /* define numsorces from argc */

  nvds_version(&nvds_lib_major_version, &nvds_lib_minor_version);

  gst_init (&argc, &argv);

  pipeline->pipeline = gst_pipeline_new ("pipeline");
  if (!pipeline->pipeline) {
    NVGSTDS_ERR_MSG_V ("Failed to create pipeline");
  }

  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline->pipeline));
  pipeline->bus_id = gst_bus_add_watch (bus, bus_callback, appCtx[0]);
  gst_object_unref (bus);

  pipeline->multi_src_bin.reset_thread = NULL;

  pipeline->multi_src_bin.bin = gst_bin_new ("multi_src_bin");
  if (!pipeline->multi_src_bin.bin) {
    NVGSTDS_ERR_MSG_V ("Failed to create element 'multi_src_bin'");
    goto done;
  }
  g_object_set (pipeline->multi_src_bin.bin, "message-forward", TRUE, NULL);

  pipeline->multi_src_bin.streammux = gst_element_factory_make ("nvstreammux", "src_bin_muxer");
  if (!pipeline->multi_src_bin.streammux) {
    NVGSTDS_ERR_MSG_V ("Failed to create element 'src_bin_muxer'");
    goto done;
  }
  gst_bin_add (GST_BIN (pipeline->multi_src_bin.bin), pipeline->multi_src_bin.streammux);
  
  /* create rtsp src bin */
  for (i = 0; i < num_sources; i++) {
    printf("%s: %u\n", "start createing source bins", i);
    GstPad *sinkpad, *srcpad;
    gchar elem_name[50];
    g_snprintf (elem_name, sizeof (elem_name), "src_sub_bin%d", i);
    pipeline->multi_src_bin.sub_bins[i].bin = gst_bin_new (elem_name);
    if (!pipeline->multi_src_bin.sub_bins[i].bin) {
      NVGSTDS_ERR_MSG_V ("Failed to create '%s'", elem_name);
      goto done;
    }

    pipeline->multi_src_bin.sub_bins[i].bin_id = pipeline->multi_src_bin.sub_bins[i].source_id = i;
    pipeline->multi_src_bin.live_source = TRUE;
    pipeline->multi_src_bin.sub_bins[i].eos_done = TRUE;
    pipeline->multi_src_bin.sub_bins[i].reset_done = TRUE;
  
    printf("argv[2]: %s\n", argv[i+1]);
    if(!create_rtsp_src_bin (i, argv[i + 1], &pipeline->multi_src_bin.sub_bins[i])){
      g_printerr ("Failed to create source bin. Exiting.\n");
    }
    gst_bin_add (GST_BIN (pipeline->multi_src_bin.bin), pipeline->multi_src_bin.sub_bins[i].bin);

    if (!link_element_to_streammux_sink_pad (pipeline->multi_src_bin.streammux,
            pipeline->multi_src_bin.sub_bins[i].bin, i)) {
      NVGSTDS_ERR_MSG_V ("source %d cannot be linked to mux's sink pad %p\n", i, pipeline->multi_src_bin.streammux);
      goto done;
    }
    pipeline->multi_src_bin.num_bins++;
    printf("pipeline->multi_src_bin.num_bins: %d\n", pipeline->multi_src_bin.num_bins);
  }
  /* create rtsp src bin */

  NVGSTDS_BIN_ADD_GHOST_PAD (pipeline->multi_src_bin.bin, pipeline->multi_src_bin.streammux, "src");

  if (install_mux_eosmonitor_probe) {
    NVGSTDS_ELEM_ADD_PROBE (pipeline->multi_src_bin.nvstreammux_eosmonitor_probe, pipeline->multi_src_bin.streammux,
        "src", nvstreammux_eosmonitor_probe_func,
        GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM,
        &pipeline->multi_src_bin);
  }

done:

  loop = g_main_loop_new (NULL, FALSE);

  pgie = gst_element_factory_make (infer_plugin, "primary-nvinference-engine");
  tiler = gst_element_factory_make ("nvmultistreamtiler", "nvtiler");
  nvosd = gst_element_factory_make ("nvdsosd", "nv-onscreendisplay");
  g_object_set (G_OBJECT (nvosd), "display-text", 1, NULL);
  g_object_set (G_OBJECT (nvosd), "display-clock", 1, NULL);
  g_object_set (G_OBJECT (nvosd), "display-bbox", 1, NULL);
  g_object_set (G_OBJECT (nvosd), "display-mask", 1, NULL);
  g_object_set (G_OBJECT (nvosd), "process-mode", 2, NULL);
  queue2 = gst_element_factory_make ("queue", "queue2");
  nvvidconv2 = gst_element_factory_make ("nvvideoconvert", "convertor2");
  guint gpu_id = 0;
  caps_filter = gst_element_factory_make ("capsfilter", "capsfilter");
  caps = gst_caps_from_string ("video/x-raw(memory:NVMM), format=I420");
  encoder = gst_element_factory_make ("nvv4l2h264enc", "encoder");
  guint profile = 0;
  guint bitrate = 1000000;
  guint iframeinterval = 60;
  codecparser = gst_element_factory_make ("h264parse", "h264-parser2");
  rtppay = gst_element_factory_make ("rtph264pay", "rtppay");
  sink = gst_element_factory_make ("udpsink", "udpsink");

  g_object_set (G_OBJECT (pipeline->multi_src_bin.streammux), "width", MUXER_OUTPUT_WIDTH, "height",
      MUXER_OUTPUT_HEIGHT, "batch-size", num_sources,
      "batched-push-timeout", MUXER_BATCH_TIMEOUT_USEC, NULL);
  g_object_get (G_OBJECT (pgie), "batch-size", &pgie_batch_size, NULL);
  if (pgie_batch_size != num_sources) {
    g_printerr
        ("WARNING: Overriding infer-config batch-size (%d) with number of sources (%d)\n",
        pgie_batch_size, num_sources);
    g_object_set (G_OBJECT (pgie), "batch-size", num_sources, NULL);
  }
  g_object_set (G_OBJECT (pgie), "batch-size", 1, NULL);
  g_object_set (G_OBJECT (pgie), "config-file-path", INFER_PGIE_CONFIG_FILE, NULL);
  g_object_set (G_OBJECT (nvvidconv2), "gpu-id", gpu_id, NULL);
  g_object_set (G_OBJECT (caps_filter), "caps", caps, NULL);
  g_object_set (G_OBJECT (encoder), "profile", profile, NULL);
  g_object_set (G_OBJECT (encoder), "iframeinterval", iframeinterval, NULL);
  g_object_set (G_OBJECT (encoder), "bitrate", bitrate, NULL);
  g_object_set (G_OBJECT (encoder), "preset-level", 1, NULL);
  g_object_set (G_OBJECT (encoder), "insert-sps-pps", 1, NULL);
  g_object_set (G_OBJECT (encoder), "bufapi-version", 1, NULL);
  g_object_set (G_OBJECT (sink), "host", "224.224.255.255", "port",
      5400, "async", FALSE, "sync", 0, NULL);

  if (!pgie) {
    g_printerr ("pgie could not be created. Exiting.\n");
    return -1;
  }
  if (!tiler) {
    g_printerr ("tiler could not be created. Exiting.\n");
    return -1;
  }
  if (!nvosd) {
    g_printerr ("nvosd could not be created. Exiting.\n");
    return -1;
  }
  if (!sink) {
    g_printerr ("sink could not be created. Exiting.\n");
    return -1;
  }
  if (!queue2 || !nvvidconv2 || !caps_filter || !encoder || !codecparser ) {
    g_printerr ("One element could not be created. Exiting.\n");
    return -1;
  }
  if (!rtppay) {
    g_printerr ("rtppay could not be created. Exiting.\n");
    return -1;
  }

  /* a tee after the tiler which shall be connected to sink(s) */
  pipeline->tiler_tee = gst_element_factory_make ("tee", "tiler_tee");
  if (!pipeline->tiler_tee) {
    NVGSTDS_ERR_MSG_V ("Failed to create element 'tiler_tee'");
  }
  /* Create demuxer only if tiled display is disabled. */
  pipeline->demuxer =
      gst_element_factory_make ("nvstreamdemux", "demuxer");
  if (!pipeline->demuxer) {
    NVGSTDS_ERR_MSG_V ("Failed to create element 'demuxer'");
  }

  /* setting tiler */ 
  tiler_rows = (guint) sqrt (num_sources);
  tiler_columns = (guint) ceil (1.0 * num_sources / tiler_rows);
  g_object_set (G_OBJECT (tiler), "rows", tiler_rows, "columns", tiler_columns,
      "width", TILED_OUTPUT_WIDTH, "height", TILED_OUTPUT_HEIGHT, NULL);

  gst_bin_add_many (GST_BIN (pipeline->pipeline),
      pipeline->multi_src_bin.bin, 
      pipeline->multi_src_bin.streammux,
      pgie, 
      tiler, 
      nvosd, 
      queue2, 
      nvvidconv2, 
      caps_filter, 
      encoder, 
      codecparser, 
      rtppay, 
      sink, 
      NULL);

  if (!gst_element_link_many (pipeline->multi_src_bin.bin, 
                              pgie, 
                              tiler, 
                              nvosd, 
                              queue2, 
                              nvvidconv2, 
                              caps_filter, 
                              encoder, 
                              codecparser, 
                              rtppay, 
                              sink, 
                              NULL)) {
    g_printerr ("Elements could not be linked. Exiting.\n");
    return -1;
  }

  osd_sink_pad = gst_element_get_static_pad (pgie, "src");
  if (!osd_sink_pad)
    g_print ("Unable to get sink pad\n");
  else
    gst_pad_add_probe (osd_sink_pad, GST_PAD_PROBE_TYPE_BUFFER,
        osd_sink_pad_buffer_probe, NULL, NULL);

  ret = TRUE;
  ret = start_rtsp_streaming (8554, 5400, 0);
  if (ret != TRUE) {
    g_print ("%s: start_rtsp_straming function failed\n", __func__);
  }

  g_print ("Now playing...\n");
  gst_element_set_state (pipeline->pipeline, GST_STATE_PLAYING);
  g_print ("Running...\n");
  g_main_loop_run (loop);
  g_print ("Returned, stopping playback\n");
  gst_element_set_state (pipeline->pipeline, GST_STATE_NULL);
  g_print ("Deleting pipeline\n");
  gst_object_unref (GST_OBJECT (pipeline->pipeline));
  g_source_remove (pipeline->bus_id);
  g_main_loop_unref (loop);
  return 0;
}
