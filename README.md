# GStreamer experiments

This repository contains some small tests, closely following the tutorials from the gstreamer website. 

## Libraries

### Ubuntu

```bash
sudo apt install \
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

Download and install the following packages from the gstreamer homepage:

[gstreamer-1.0-1.15.2-x86_64.pkg](https://gstreamer.freedesktop.org/data/pkg/osx/1.15.2/gstreamer-1.0-1.15.2-x86_64.pkg)
[gstreamer-1.0-devel-1.15.2-x86_64.pkg](https://gstreamer.freedesktop.org/data/pkg/osx/1.15.2/gstreamer-1.0-devel-1.15.2-x86_64.pkg)

[gstreamer-1.0-1.15.2-x86_64-packages.dmg](https://gstreamer.freedesktop.org/data/pkg/osx/1.15.2/gstreamer-1.0-1.15.2-x86_64-packages.dmg)

At the time of writing, the code does not work on macOS with the homebrew version of GStreamer. Although compiling
works without errors, running the program results in an `illegal instruction: 4`.

## Building the source code 

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

