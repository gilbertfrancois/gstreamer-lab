#include <gst/gst.h>
#include <glib.h>
#include <sys/time.h>


static GstPadProbeReturn
sink_bin_buf_probe(GstPad *pad, GstPadProbeInfo *info, gpointer u_data) {
    static struct timeval last_fps_time;
    static struct timeval curr_fps_time;
    static int numbuff = 0;
    const int FPS_DISPLAY_FREQ = 10;
    if ((numbuff++ % FPS_DISPLAY_FREQ) == 0) {
        if (last_fps_time.tv_sec == 0 && last_fps_time.tv_usec == 0) {
            gettimeofday(&last_fps_time, NULL);
        }
        else {
            gettimeofday(&curr_fps_time, NULL);
            gdouble elapsedtime = 
                (curr_fps_time.tv_sec + curr_fps_time.tv_usec / 1000000.0) -
                (last_fps_time.tv_sec + last_fps_time.tv_usec / 1000000.0);
            g_print("%.2f fps\n", (FPS_DISPLAY_FREQ / elapsedtime));
            last_fps_time = curr_fps_time;
        }
    }
    return GST_PAD_PROBE_OK;
}


int main(int argc, char *argv[]) {
    GstElement *pipeline;
    GstElement *videosrc;
    GstElement *vertigotv;
    GstElement *videoconvert;
    GstElement *videosink;

    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    GstPad *vertigotv_sink_pad;
    gulong probe_id = -1;

    // Initialize gstreamer
    gst_init(&argc, &argv);

    // Create the elements
    videosrc = gst_element_factory_make("videotestsrc", "video_src");
    vertigotv = gst_element_factory_make("vertigotv", "filter_effect");
    videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    /* videosink = gst_element_factory_make("osxvideosink", "display_window"); */
    /* videosink = gst_element_factory_make("autovideosink", "display_window"); */
    /* videosink = gst_element_factory_make("xvimagesink", "display_window"); */
    videosink = gst_element_factory_make("glimagesink", "display_window");

    vertigotv_sink_pad = gst_element_get_static_pad(vertigotv, "sink");
    if (!vertigotv_sink_pad) {
        g_print("Unable to get vertigotv sink pad.");
    }
    else {
        probe_id = gst_pad_add_probe(vertigotv_sink_pad, GST_PAD_PROBE_TYPE_BUFFER, sink_bin_buf_probe, NULL, NULL);
        g_print("Probe added with ID: %ld.\n", probe_id);
    }

    // Create empty pipeline
    pipeline = gst_pipeline_new("test-pipeline");

    if (!pipeline || !videosrc || !vertigotv || !videoconvert || !videosink) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    // Build the pipeline
    gst_bin_add_many(GST_BIN(pipeline), videosrc, vertigotv, videoconvert, videosink, NULL);
    gst_element_link_many(videosrc, vertigotv, videoconvert, videosink, NULL);

    // Modify the videosrc's properties
    g_object_set(videosrc, "pattern", 0, NULL);

    // Start playing
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state\n.");
        gst_object_unref(pipeline);
        return -1;
    }

    // Wait until error or EOS
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, (GstMessageType) (GST_MESSAGE_EOS | GST_MESSAGE_ERROR));
//    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS);
//
    // Parse the message
    if (msg != NULL) {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE (msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
                g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
                g_clear_error(&err);
                g_free(debug_info);
                break;
            case GST_MESSAGE_EOS:
                g_print("End-Of-Stream reached.\n");
                break;
            default:
                g_printerr("Unexpected message received.\n");
                break;
        }
        gst_message_unref(msg);
    }

    // Free resources 
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    if (vertigotv_sink_pad != NULL) {
        gst_object_unref(vertigotv_sink_pad);
    }
    gst_object_unref(pipeline);

    return 0;
}






