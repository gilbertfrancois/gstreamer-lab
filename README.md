# GStreamer experiments

This repository contains some small tests, closely following the tutorials from the gstreamer website. 

## Libraries

### Ubuntu

```bash
sudo apt install \
    cmake \
    libssl1.0.0 \
    libgstreamer1.0-0 \
    gstreamer1.0-tools \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav \
    libgstrtspserver-1.0-0 \
    libjansson4=2.11-1
```
GStreamer has tons of plugins. Search with `apt search gstreamer1.0-*` and 
install the plugins that you need.

### macOS

Download and install the following packages from the gstreamer homepage. Please use version 1.14.5 to have the same 
version as on Ubuntu 18.04 LTS.

[gstreamer-1.0-1.14.5-x86_64.pkg](https://gstreamer.freedesktop.org/data/pkg/osx/1.14.5/gstreamer-1.0-1.14.5-x86_64.pkg)
[gstreamer-1.0-devel-1.14.5-x86_64.pkg](https://gstreamer.freedesktop.org/data/pkg/osx/1.14.5/gstreamer-1.0-devel-1.14.5-x86_64.pkg)

At the time of writing, the code does not work on macOS with the homebrew version of GStreamer. Although compiling
works without errors, running the program results in an `illegal instruction: 4`.

## Building the source code 

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```


### Quick command line tests

To see if GStreamer works on your computer, you can use one of the commands below:

```shell script
# Should work, but not when the setup is non standard
gst-launch-1.0 videotestsrc ! videoconvert ! autovideosink
# Try this if you see no output (on Linux) or the environment variable DISPLAY is not set to :0
gst-launch-1.0 videotestsrc ! videoconvert ! xvimagesink
# You can try this on macOS if autovideosink does not work
gst-launch-1.0 videotestsrc ! videoconvert ! osxvideosink
```

## Buffers and metadata

You can access the streaming data in the following way: 

Make a callback `GstPadProbeReturn`
The `GstPadProbeInfo` has the data:
```
GstBuffer *buf = (GstBuffer*)info->data;

- GstBuffer *buf = (GstBuffer*) info->data;
    - NvDsBatchMeta *batch_meta = gst_buffer_get_nvds_batch_meta(buf)
        - NvDsMetaList* l_frame = batch_meta->frame_meta_list
            - NvDsFrameMeta *frame_meta = (NvDsFrameMeta*) (l_frame->data)
                - NvDsMetaList *l_obj = frame_meta->obj_meta_list
                    - obj_meta = (NvDsObjectMeta*) (l_obj->data)
