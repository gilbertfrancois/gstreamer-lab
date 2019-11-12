#ifndef __GST_NVIDEO_H__
#define __GST_NVIDEO_H__

#include <gst/gst.h>
#include "boilerplate.h"

G_BEGIN_DECLS

#define GST_TYPE_NVIDEO \
  (gst_n_video_get_type())
#define GST_NVIDEO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_NVIDEO,GstNVideo))

typedef struct _GstNVideo GstNVideo;
typedef struct _GstNVideoClass GstNVideoClass;

struct _GstNVideo {
  GstVideoFilter element;
  gboolean silent;
  int count; // Global frame counter
  int probe; // Counter to compute FPS, reset every 16 frames.
  double started;
};

struct _GstNVideoClass {
  GstVideoFilterClass parent_class;
};

GType gst_my_video_get_type(void);

/* The only boolean property we can set/get */
enum {
  PROP_SILENT = 1 // zero not allowed
};

/* We still measure FPS */
static double measure_fps(GstNVideo *filter, guint count);

/* Various functions of the filter we provide */
static void gst_n_video_set_property(GObject *object, guint prop_id,
                                      const GValue *value, GParamSpec *pspec);

static void gst_n_video_get_property(GObject *object, guint prop_id,
                                      GValue *value, GParamSpec *pspec);

GST_DEBUG_CATEGORY_STATIC (gst_n_video_debug);

G_DEFINE_TYPE (GstNVideo, gst_n_video, GST_TYPE_VIDEO_FILTER);

G_END_DECLS

#endif /* __GST_NVIDEO_H__ */
