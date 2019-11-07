cmake_minimum_required(VERSION 3.10)

add_executable(runme_2 main.c)
target_link_libraries(runme_2 ${LIBRARIES})
if (NOT APPLE)
    target_compile_options(runme_2 PUBLIC ${GST_CFLAGS_OTHER})
endif()
