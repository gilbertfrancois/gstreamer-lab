#include <gst/gst.h>

typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *convert;
    GstElement *sink;
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
    data.convert = gst_element_factory_make("audioconvert", "convert");
    data.sink = gst_element_factory_make("autoaudiosink", "sink");

    // Create empty pipeline
    data.pipeline = gst_pipeline_new("test-pipeline");

    if (!data.pipeline || !data.source || !data.convert || !data.sink) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    // Build the pipeline.
    // Note that we are not linking the source at this point.
    gst_bin_add(GST_BIN (data.pipeline), data.source);
    gst_bin_add(GST_BIN (data.pipeline), data.convert);
    gst_bin_add(GST_BIN (data.pipeline), data.sink);
    if (!gst_element_link(data.convert, data.sink)) {
        g_printerr("Elements could not be linked.\n");
        return -1;
    }

    // Set the uri property of the source. 
    // The properties are a list of key-value pairs, terminated by a NULL.
    g_object_set(data.source, "uri", "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", NULL);
    
    // Connect to the signal paths
    g_signal_connect(data.source, "pad-added", G_CALLBACK(pad_added_handler), &data);
    
    g_print("hier\n");
    // Start playing
    ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline oth the playing state.\n");
        return -1;
    }
    
    // TODO: Bus loop here...

    // Free the resources
    gst_object_unref(bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
    return 0;
}

static void pad_added_handler(GstElement *src, GstPad *new_pad, CustomData *data) {
    GstPad *sink_pad = gst_element_get_static_pad(data->convert, "sink");
    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar *new_pad_type = NULL;

    g_print("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));

    // If the converter is already linked, do nothing.
    if (gst_pad_is_linked(sink_pad)) {
        g_print("Sink pad is already linked.\n");
        goto exit;
    }

    // Check the new pad's type
    new_pad_caps  = gst_pad_get_current_caps(new_pad);
    new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    new_pad_type = gst_structure_get_name(new_pad_struct);
    if (!g_str_has_prefix(new_pad_type, "audio/x-raw")) {
        g_print("It has type '%s' which is not audio, Ignoring...\n", new_pad_type);
        goto exit;
    }

    // Attempt to link
    ret = gst_pad_link(new_pad, sink_pad);
    if (GST_PAD_LINK_FAILED(ret)) {
        g_print("Type is '%s' but link failed.\n", new_pad_type);
    }
    else {
        g_print("Link successful (type '%s').\n", new_pad_type);
    }

exit:
    // Unreference the new pad's caps, if we got them
    if (new_pad_caps != NULL) {
        gst_caps_unref(new_pad_caps);
    }

    // Unreference the sink pad
    gst_object_unref(sink_pad);
}






