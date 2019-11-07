cmake_minimum_required(VERSION 3.10)

add_executable(runme_3a main.c)
target_link_libraries(runme_3a ${LIBRARIES})
target_compile_options(runme_3a PUBLIC ${GST_CFLAGS_OTHER})
