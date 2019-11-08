#include <iostream>

#include <gst/gst.h>
#include <glib.h>

/** Unlike the plugin, the main program is written in C++ */
int main(int argc, char *argv[]) {
  gst_init(&argc, &argv);

  auto registry = gst_registry_get();

  // Scan the current folder where the program starts for our .so library.
  // We could also set GST_PLUGIN_PATH system property instead.
  gst_registry_scan_path(registry, ".");

  GstElement *pipeline = gst_pipeline_new("videotest-pipeline");
  GMainLoop *loop = g_main_loop_new(nullptr, false);

  auto deep_plugin = gst_element_factory_make("myfilter", "myfilter");
  auto videotestsrcm = gst_element_factory_make("videotestsrc", "testsource");
  auto videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
  auto autovideosinkm = gst_element_factory_make("glimagesink", "videosink");

  if (!deep_plugin) {
    g_printerr("Our plugin cannot be loaded. Be sure libmyfilter.so is in the folder where the program starts\n");
    return -2;
  }
  if (!pipeline || !videotestsrcm || !autovideosinkm || !deep_plugin) {
    g_printerr("Our plugin ok but some others could not be created. Exiting.\n");
    return -1;
  }

  // Set up the pipeline

  // Add all elements into the pipeline
  gst_bin_add_many(GST_BIN (pipeline),
                   videotestsrcm, deep_plugin, videoconvert, autovideosinkm, nullptr);

  // Link videotestsrcm > deep_plugin > videoconvert > autovideosinkm
  gst_element_link(videotestsrcm, deep_plugin);
  gst_element_link(deep_plugin, videoconvert);
  gst_element_link(videoconvert, autovideosinkm);

  // Set the pipeline to "playing" state
  g_print("Now set pipeline in state playing");
  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  // Iterate
  g_print("Running...\n");
  g_main_loop_run(loop);

  g_print("Returned, stopping playback\n");
  gst_element_set_state(pipeline, GST_STATE_NULL);

  gst_object_unref(GST_OBJECT (pipeline));
  g_main_loop_unref(loop);

  return 0;
}
