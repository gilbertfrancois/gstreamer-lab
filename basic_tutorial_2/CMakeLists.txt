cmake_minimum_required(VERSION 3.10)

find_package(PkgConfig)
pkg_check_modules(FFI REQUIRED libffi)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

include_directories(
        ${GST_INCLUDE_DIRS})
link_directories(
        ${GST_LIBRARY_DIRS})

set(LIBRARIES ${GST_LIBRARIES})

add_executable(runme_2 main.c)
target_link_libraries(runme_2 ${LIBRARIES})
target_compile_options(runme_2 PUBLIC ${GST_CFLAGS_OTHER})
