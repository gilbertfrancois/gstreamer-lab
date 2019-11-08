#ifndef __GST_MYFILTER_H__
#define __GST_MYFILTER_H__

#include <gst/gst.h>
#include "boilerplate.h"

G_BEGIN_DECLS

#define GST_TYPE_MYFILTER \
  (gst_my_filter_get_type())
#define GST_MYFILTER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MYFILTER,GstMyFilter))

typedef struct _GstMyFilter GstMyFilter;
typedef struct _GstMyFilterClass GstMyFilterClass;

struct _GstMyFilter {
  GstElement element;
  GstPad *sinkpad, *srcpad;
  gboolean silent;
  int count; // Global frame counter
  int probe; // Counter to compute FPS, reset every 16 frames.
  double started;
};

struct _GstMyFilterClass {
  GstElementClass parent_class;
};

GType gst_my_filter_get_type(void);

/* The only boolean property we can set/get */
enum {
  PROP_SILENT = 1 // zero not allowed
};

G_END_DECLS

#endif /* __GST_MYFILTER_H__ */
