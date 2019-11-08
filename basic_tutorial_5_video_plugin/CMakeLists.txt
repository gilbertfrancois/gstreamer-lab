cmake_minimum_required(VERSION 3.10)
project(basic_tutorial_4)
find_package(PkgConfig)

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

include_directories(
        ${GLIB_INCLUDE_DIRS}
        ${GSTREAMER_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

link_directories(
        ${GLIB_LIBRARY_DIRS}
        ${GSTREAMER_LIBRARY_DIRS}
        ${GSTREAMER-APP_LIBRARY_DIRS}
)

link_libraries(
        ${GSTREAMER_LIBRARIES}
        ${GSTREAMER-APP_LIBRARIES}
)
# The main executable to run.
add_executable(runtime_5 main.cpp)

# The plugin .so library that will be loaded by the main executable.
add_library(myvideo SHARED gst-plugin/src/gstmyvideo.c)