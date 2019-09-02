cmake_minimum_required(VERSION 3.10)
project(gstreamer-lab)

set(CMAKE_CXX_STANDARD 14)

# ============================================================================
# ------------------------------ Platform specific pc location ---------------

if (APPLE)
    set(ENV{PKG_CONFIG_PATH} "/usr/local/opt/libffi/lib/pkgconfig")
endif()

add_subdirectory(basic_tutorial_1)
add_subdirectory(basic_tutorial_2)
add_subdirectory(basic_tutorial_3)
add_subdirectory(ds_test_1)
