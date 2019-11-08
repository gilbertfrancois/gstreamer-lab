#include <gst/gst.h>

#include "gstmyvideo.h"
#include <sys/time.h>

/* Here we define our expected video format, somewhat on voluntary basis.
 * In production, we probably should use something that needs not be converted. */

/* The sink pad that expects our video format.*/
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
  GST_PAD_SINK, GST_PAD_ALWAYS,GST_STATIC_CAPS("ANY")
);

/* The source pad that can swallow anything supplied */
static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
  GST_PAD_SRC, GST_PAD_ALWAYS,GST_STATIC_CAPS("ANY")
);

G_DEFINE_TYPE (GstMyVideo, gst_my_video, GST_TYPE_ELEMENT);

/* Various functions of the filter we provide */
static void gst_my_video_set_property(GObject *object, guint prop_id,
                                       const GValue *value, GParamSpec *pspec);
static void gst_my_video_get_property(GObject *object, guint prop_id,
                                       GValue *value, GParamSpec *pspec);
static gboolean gst_my_video_sink_event(GstPad *pad, GstObject *parent, GstEvent *event);
static GstFlowReturn gst_my_video_chain(GstPad *pad, GstObject *parent, GstBuffer *buf);

GST_DEBUG_CATEGORY_STATIC (gst_my_video_debug);

/* This function does the actual processing (nothing in our case).
 * Let's do something useful: make it to measure the frames per second rate. */
static GstFlowReturn gst_my_video_chain(GstPad *pad, GstObject *parent, GstBuffer *buf) {
  GstMyVideo *filter= GST_MYVIDEO (parent);
  struct timeval tp;

  if ( (filter->count & 0xF) == 0 ) { // Reset every 16 frames
    // Reset the start of the time when first frame arrives.
    filter->probe = 0;
    gettimeofday(&tp, NULL);
    filter->started = (double) tp.tv_sec + tp.tv_usec / 1000000.0;
  }
  if (!filter->silent && filter->probe > 3) { // Start showing after 3 samples
    gettimeofday(&tp, NULL);
    double ms = (double) tp.tv_sec + tp.tv_usec / 1000000.0;
    g_print("%d frame. Passing %.2f frames per second.\n", filter->count,
            filter->probe / (ms - filter->started));
  }
  filter->probe++;
  filter->count++;

  /* just push out the incoming buffer without touching it */
  return gst_pad_push(filter->srcpad, buf);
}

/* Initialize the class of the plugin */
static void gst_my_video_class_init(GstMyVideoClass *klass) {
  g_print("My Video have initialized its class\n");
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;

  gobject_class->set_property = gst_my_video_set_property;
  gobject_class->get_property = gst_my_video_get_property;

  g_object_class_install_property(gobject_class, PROP_SILENT,
                                  g_param_spec_boolean("silent", "Silent", "Produce verbose output ?",
                                                       FALSE, G_PARAM_READWRITE));

  gst_element_class_set_details_simple(gstelement_class,
                                       "MyVideo",
                                       "Lab plugin",
                                       "GStreamer Lab plugin",
                                       "audrius<audrius@deep-impact.com>");

  gst_element_class_add_pad_template(gstelement_class,
                                     gst_static_pad_template_get(&src_factory));
  gst_element_class_add_pad_template(gstelement_class,
                                     gst_static_pad_template_get(&sink_factory));
}

/* Initialize he plugin */
static void gst_my_video_init(GstMyVideo *filter) {
  g_print("I have initialized my instance\n");
  filter->sinkpad = gst_pad_new_from_static_template(&sink_factory, "sink");
  gst_pad_set_event_function (filter->sinkpad,
                              GST_DEBUG_FUNCPTR(gst_my_video_sink_event));
  gst_pad_set_chain_function (filter->sinkpad,
                              GST_DEBUG_FUNCPTR(gst_my_video_chain));
  GST_PAD_SET_PROXY_CAPS (filter->sinkpad);
  gst_element_add_pad(GST_ELEMENT (filter), filter->sinkpad);

  filter->srcpad = gst_pad_new_from_static_template(&src_factory, "src");
  GST_PAD_SET_PROXY_CAPS (filter->srcpad);
  gst_element_add_pad(GST_ELEMENT (filter), filter->srcpad);

  filter->silent = FALSE;
  filter->count = filter->probe = 0;
}

/* Some property setter */
static void gst_my_video_set_property(GObject *object, guint prop_id,
                           const GValue *value, GParamSpec *pspec) {
  GstMyVideo *filter = GST_MYVIDEO (object);

  switch (prop_id) {
    case PROP_SILENT:
      filter->silent = g_value_get_boolean(value);
      g_print("I have set my property to %d\n", filter->silent);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/* Some property getter */
static void gst_my_video_get_property(GObject *object, guint prop_id,
                           GValue *value, GParamSpec *pspec) {
  GstMyVideo *filter = GST_MYVIDEO (object);

  switch (prop_id) {
    case PROP_SILENT:
      g_print("I have been asked about my property and would say %d\n", filter->silent);
      g_value_set_boolean(value, filter->silent);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/* This function handles sink events */
static gboolean gst_my_video_sink_event(GstPad *pad, GstObject *parent, GstEvent *event) {
  g_print("Received %s event", GST_EVENT_TYPE_NAME(event));

  GstMyVideo *filter = GST_MYVIDEO (parent);
  gboolean ret;

  GST_LOG_OBJECT (filter, "Received %s event: %"
      GST_PTR_FORMAT,
                  GST_EVENT_TYPE_NAME(event), event);

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_CAPS: {
      GstCaps *caps;

      gst_event_parse_caps(event, &caps);
      /* do something with the caps */

      /* and forward */
      ret = gst_pad_event_default(pad, parent, event);
      break;
    }
    default:
      ret = gst_pad_event_default(pad, parent, event);
      break;
  }
  return ret;
}

/* Entry point to initialize the plug-in */
static gboolean myvideo_init(GstPlugin *myvideo) {
  GST_DEBUG_CATEGORY_INIT (gst_my_video_debug, DI_PLUGIN_NAME,
                           0, "Template myvideo");
  g_print("I have initialized my plugin\n");
  return gst_element_register(myvideo, DI_PLUGIN_NAME, GST_RANK_NONE,
                              GST_TYPE_MYVIDEO);
}

/* gstreamer looks for this structure to register myvideos
 *
 * exchange the string 'Template myvideo' with your myvideo description
 */
GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    myvideo,
    "GStreamer Lab Plugin",
    myvideo_init,
    PACKAGE_VERSION,
    GST_LICENSE,
    GST_PACKAGE_NAME,
    GST_PACKAGE_ORIGIN
)
