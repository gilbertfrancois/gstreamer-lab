#ifndef __GST_MYVIDEO_H__
#define __GST_MYVIDEO_H__

#include <gst/gst.h>
#include "boilerplate.h"

G_BEGIN_DECLS

#define GST_TYPE_MYVIDEO \
  (gst_my_video_get_type())
#define GST_MYVIDEO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MYVIDEO,GstMyVideo))

typedef struct _GstMyVideo GstMyVideo;
typedef struct _GstMyVideoClass GstMyVideoClass;

struct _GstMyVideo {
  GstVideoFilter element;
  gboolean silent;
  int count; // Global frame counter
  int probe; // Counter to compute FPS, reset every 16 frames.
  double started;
};

struct _GstMyVideoClass {
  GstVideoFilterClass parent_class;
};

GType gst_my_video_get_type(void);

/* The only boolean property we can set/get */
enum {
  PROP_SILENT = 1 // zero not allowed
};

/* We still measure FPS */
static double measure_fps(GstMyVideo *filter, guint count);

/* Various functions of the filter we provide */
static void gst_my_video_set_property(GObject *object, guint prop_id,
                                      const GValue *value, GParamSpec *pspec);

static void gst_my_video_get_property(GObject *object, guint prop_id,
                                      GValue *value, GParamSpec *pspec);

GST_DEBUG_CATEGORY_STATIC (gst_my_video_debug);

G_DEFINE_TYPE (GstMyVideo, gst_my_video, GST_TYPE_VIDEO_FILTER);

G_END_DECLS

#endif /* __GST_MYVIDEO_H__ */
