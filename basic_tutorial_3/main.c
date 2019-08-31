#include <gst/gst.h>

typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *audioconvert;
    GstElement *videoconvert;
    GstElement *audiosink;
    GstElement *videosink;
} CustomData;

// Handler for the pad-added signal
static void pad_added_handler(GstElement *src, GstPad *pad, CustomData *data);

int main(int argc, char *argv[]) {
    CustomData data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;

    // Initialize gstreamer
    gst_init(&argc, &argv);

    // Create the elements
    data.source = gst_element_factory_make("uridecodebin", "source");
    data.audioconvert = gst_element_factory_make("audioconvert", "audioconvert");
    data.videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    data.audiosink = gst_element_factory_make("autoaudiosink", "audiosink");
    data.videosink = gst_element_factory_make("autovideosink", "videosink");

    // Create empty pipeline
    data.pipeline = gst_pipeline_new("test-pipeline");

    if (!data.pipeline || !data.source || !data.audioconvert || !data.videoconvert || !data.audiosink) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    // Build the pipeline.
    // Note that we are not linking the source at this point.
    gst_bin_add(GST_BIN (data.pipeline), data.source);
    gst_bin_add(GST_BIN (data.pipeline), data.audioconvert);
    gst_bin_add(GST_BIN (data.pipeline), data.videoconvert);
    gst_bin_add(GST_BIN (data.pipeline), data.audiosink);
    gst_bin_add(GST_BIN (data.pipeline), data.videosink);

    if (!gst_element_link(data.audioconvert, data.audiosink)) {
        g_printerr("Audio elements could not be linked.\n");
        return -1;
    }
    if (!gst_element_link(data.videoconvert, data.videosink)) {
        g_printerr("Video elements could not be linked.\n");
        return -1;
    }

    // Set the uri property of the source. 
    // The properties are a list of key-value pairs, terminated by a NULL.
    g_object_set(data.source, "uri", "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", NULL);

    // Connect to the signal pads
    g_signal_connect(data.source, "pad-added", G_CALLBACK(pad_added_handler), &data);

    // Start playing
    ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline oth the playing state.\n");
        return -1;
    }

    // Listen to the bus
    bus = gst_element_get_bus(data.pipeline);
    do {
        msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
                GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
        // parse message
        if (msg != NULL) {
            GError *err;
            gchar *debug_info;

            switch(GST_MESSAGE_TYPE (msg)) {
                case GST_MESSAGE_ERROR:
                    gst_message_parse_error (msg, &err, &debug_info);
                    g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
                    g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
                    g_clear_error (&err);
                    g_free (debug_info);
                    terminate = TRUE;
                    break;
                case GST_MESSAGE_EOS:
                    g_print("End of stream.\n");
                    terminate = TRUE;
                    break;
                case GST_MESSAGE_STATE_CHANGED:
                    // We are only interested in the state change of the pipeline
                    if (GST_MESSAGE_SRC(msg) == GST_OBJECT(data.pipeline)) {
                        GstState old_state, new_state, pending_state;
                        gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
                        g_print("Pipeline state changed from %s to %s:\n",
                                gst_element_state_get_name(old_state),
                                gst_element_state_get_name(new_state));
                    }
                    break;
                default:
                    g_printerr("Unexpected message received\n");
                    break;
            }
        }
        gst_message_unref(msg);
    } while(!terminate);

    // Free the resources
    gst_object_unref(bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
    return 0;
}

static void pad_added_handler(GstElement *src, GstPad *new_pad, CustomData *data) {

    g_print("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));

    GstPad       *sink_pad       = NULL;
    GstCaps      *new_pad_caps   = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar  *new_pad_type   = NULL;

    // Check the new pad's type
    new_pad_caps   = gst_pad_get_current_caps(new_pad);
    new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    new_pad_type   = gst_structure_get_name(new_pad_struct);
    g_print("Found pad of type '%s'\n", new_pad_type);

    // Create a audiosink pad to the correct reference
    if (g_str_has_prefix(new_pad_type, "video/x-raw")) {
        sink_pad = gst_element_get_static_pad(data->videoconvert, "sink");
    }
    else if (g_str_has_prefix(new_pad_type, "audio/x-raw")) {
        sink_pad = gst_element_get_static_pad(data->audioconvert, "sink");
    }
    else {
        g_printerr("Something went wrong during linking.\n");
    }

    // Check if the pad is already linked 
    if (gst_pad_is_linked(sink_pad)) {
        g_print("audiosink pad is already linked.\n");
    }
    else { 
        // Attempt to link
        GstPadLinkReturn ret = gst_pad_link(new_pad, sink_pad);
        if (GST_PAD_LINK_FAILED(ret)) {
            g_print("Type is '%s' but link failed.\n", new_pad_type);
        }
        else {
            g_print("Link successful (type '%s').\n", new_pad_type);
        }
    }

    // Unreference 
    if (new_pad_caps != NULL) {
        gst_caps_unref(new_pad_caps);
    }
    if (sink_pad != NULL) {
        gst_object_unref(sink_pad);
    }

}




