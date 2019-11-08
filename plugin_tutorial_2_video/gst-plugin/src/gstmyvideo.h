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
  GstElement element;
  GstPad *sinkpad, *srcpad;
  gboolean silent;
  int count; // Global frame counter
  int probe; // Counter to compute FPS, reset every 16 frames.
  double started;
};

struct _GstMyVideoClass {
  GstElementClass parent_class;
};

GType gst_my_video_get_type(void);

/* The only boolean property we can set/get */
enum {
  PROP_SILENT = 1 // zero not allowed
};

G_END_DECLS

#endif /* __GST_MYVIDEO_H__ */
