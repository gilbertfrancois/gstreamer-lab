cmake_minimum_required(VERSION 3.15)
project(deepstreams)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O3 -march=skylake)

set(DEEP_STREAM_INSTALL /opt/nvidia/deepstream/deepstream-4.0)

find_package(PkgConfig)

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER-APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GSTREAMER-AUDIO REQUIRED gstreamer-audio-1.0)
pkg_check_modules(GSTREAMER-PBUTILS REQUIRED gstreamer-pbutils-1.0)
pkg_check_modules(GSTREAMER-FFT REQUIRED gstreamer-fft-1.0)

include_directories(
        ${GLIB_INCLUDE_DIRS}
        includes
        ${GSTREAMER_INCLUDE_DIRS}
        ${GSTREAMER-APP_INCLUDE_DIRS}
        ${GSTREAMER-AUDIO_INCLUDE_DIRS}
        ${GSTREAMER-PBUTILS_INCLUDE_DIRS}
        ${GSTREAMER-FFT_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${DEEP_STREAM_INSTALL}/sources/includes
)

link_directories(
        ${GLIB_LIBRARY_DIRS}
        ${GSTREAMER_LIBRARY_DIRS}
        ${GSTREAMER-APP_LIBRARY_DIRS}
        ${GSTREAMER-AUDIO_LIBRARY_DIRS}
        ${GSTREAMER-PBUTILS_LIBRARY_DIRS}
        ${GSTREAMER-FFT_LIBRARY_DIRS}
        ${DEEP_STREAM_INSTALL}/lib
)

link_libraries(
        nvdsgst_meta nvds_meta
        nvdsgst_helper
        ${GSTREAMER_LIBRARIES}
        ${GSTREAMER-APP_LIBRARIES}
        ${GSTREAMER-AUDIO_LIBRARIES}
        ${GSTREAMER-PBUTILS_LIBRARIES}
        ${GSTREAMER-FFT_LIBRARIES}
)

# The plugin .so library that will be loaded by the main executable.
add_library(nvideo SHARED gst-plugin/src/gstmyvideo.cpp)
add_executable(deepstream_test1_app deepstream_test1_app.cpp)
configure_file(dstest1_pgie_config.txt dstest1_pgie_config.txt COPYONLY)
