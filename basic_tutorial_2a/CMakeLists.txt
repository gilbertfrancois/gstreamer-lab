cmake_minimum_required(VERSION 3.10)

add_executable(runme_2a main.c)
target_link_libraries(runme_2a ${LIBRARIES})
if (NOT APPLE)
    target_compile_options(runme_2a PUBLIC ${GST_CFLAGS_OTHER})
endif()
