cmake_minimum_required(VERSION 3.10)
project(gstreamer-lab)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig)
if (APPLE)
    find_library(GST_LIBRARIES GStreamer)
    include_directories(/Library/Frameworks/GStreamer.framework/Headers)
    link_directories(/Library/Frameworks/GStreamer.framework/Versions/Current/lib)
    set(LIBRARIES ${GST_LIBRARIES})
elseif (UNIX)
    pkg_check_modules(FFI REQUIRED libffi)
    pkg_check_modules(GST REQUIRED gstreamer-1.0)
    include_directories( ${GST_INCLUDE_DIRS})
    link_directories( ${GST_LIBRARY_DIRS})
    set(LIBRARIES ${GST_LIBRARIES})
endif()



# ============================================================================
# ------------------------------ Include all programs ------------------------

add_subdirectory(basic_tutorial_1)
add_subdirectory(basic_tutorial_2)
add_subdirectory(basic_tutorial_2a)
add_subdirectory(basic_tutorial_3)
add_subdirectory(basic_tutorial_3a)
add_subdirectory(basic_tutorial_4)

