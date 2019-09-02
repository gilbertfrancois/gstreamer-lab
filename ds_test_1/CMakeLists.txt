cmake_minimum_required(VERSION 3.10)

find_package(PkgConfig)
pkg_check_modules(FFI REQUIRED libffi)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

include_directories(
        ../3rdparty/deepstream_sdk_v4.0_jetson/sources/includes
        ${GST_INCLUDE_DIRS})
link_directories(
        /opt/nvidia/deepstream/deepstream-4.0/lib
        ${GST_LIBRARY_DIRS})

set(LIBRARIES ${GST_LIBRARIES})

add_executable(runme main.c)
target_link_libraries(runme ${LIBRARIES})
target_compile_options(runme PUBLIC ${GST_CFLAGS_OTHER})
