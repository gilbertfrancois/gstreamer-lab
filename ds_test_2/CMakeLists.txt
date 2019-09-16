cmake_minimum_required(VERSION 3.10)

find_package(PkgConfig)
pkg_check_modules(FFI REQUIRED libffi)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

set(TARGET_DEVICE $(shell gcc -dumpmachine | cut -f1 -d -))
message(${TARGET_DEVICE})

include_directories(
        ../3rdparty/deepstream_sdk_v4.0_jetson/sources/includes
        ${GST_INCLUDE_DIRS})
link_directories(
        /opt/nvidia/deepstream/deepstream-4.0/lib
        ${GST_LIBRARY_DIRS})

set(LIBRARIES ${GST_LIBRARIES})

add_executable(sentinel_test_2 main.c)
target_link_libraries(sentinel_test_2 ${LIBRARIES})
target_compile_options(sentinel_test_2 PUBLIC ${GST_CFLAGS_OTHER})
