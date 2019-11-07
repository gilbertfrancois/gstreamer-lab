#include <gst/gst.h>

int main(int argc, char *argv[]) {
  GstElement *pipeline;
  GstBus *bus;
  GstMessage *msg;
  GMainLoop *loop;

  // Initialize GStreamer
  gst_init(&argc, &argv);
  loop = g_main_loop_new(NULL, FALSE);

  // Build the pipeline
  pipeline = gst_parse_launch("videotestsrc ! videoconvert ! glimagesink", NULL);

  // start playing
  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  // Wait until error or EOS (end of stream)
  bus = gst_element_get_bus(pipeline);
  g_main_loop_run(loop);
  msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

  // Free resources
  if (msg != NULL) {
    gst_message_unref(msg);
  }
  gst_object_unref(bus);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);
}
