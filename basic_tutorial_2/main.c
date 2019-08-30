#include <gst/gst.h>

int main(int argc, char *argv[]) {
    GstElement *pipeline, *videosrc, *vertigotv, *autovideosink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    // Initialize gstreamer
    gst_init(&argc, &argv);

    // Create the elements
    videosrc = gst_element_factory_make("videotestsrc", "video_src");
    vertigotv = gst_element_factory_make("vertigotv", "filter_effect");
    autovideosink = gst_element_factory_make("autovideosink", "display_window");

    // Create empty pipeline
    pipeline = gst_pipeline_new("test-pipeline");

    if (!pipeline || !videosrc || !vertigotv || !autovideosink) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    // Build the pipeline
    gst_bin_add(GST_BIN (pipeline), videosrc);
    gst_bin_add(GST_BIN (pipeline), vertigotv);
    gst_bin_add(GST_BIN (pipeline), autovideosink);
    gst_element_link(videosrc, vertigotv);
    gst_element_link(vertigotv, autovideosink);

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
//    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS | GST_MESSAGE_ERROR);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR);

    // Parse the message
    if (msg != NULL) {
        GError *err;
        gchar *debug_info;

        switch(GST_MESSAGE_TYPE (msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
                g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
                g_clear_error (&err);
                g_free (debug_info);
                break;
            case GST_MESSAGE_EOS:
                g_print ("End-Of-Stream reached.\n");
                break;
            default:
                g_printerr ("Unexpected message received.\n");
                break;
        }
        gst_message_unref(msg);
    }

    // Free resources 
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}






