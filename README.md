# GStreamer experiments

This repository contains some small tests, closely following the tutorials from the gstreamer website. 

## Libraries

### Ubuntu

Install the packages `libgstreamer1.0` and `libgstreamer1.0-dev`. GStreamer has tons of plugins. Search with `apt search gstreamer1.0-*` and 
install the plugins that you need.

### macOS

You can install gstreamer with *homebrew* or as direct download from the gstreamer homepage.
At the time of writing, the code does not work on macOS with the homebrew version of GStreamer. Although compiling
works without errors, running the program results in an `illegal instruction: 4`.

## Building the source code 

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

