cmake_minimum_required(VERSION 3.10)

add_executable(runme_3 main.c)
target_link_libraries(runme_3 ${LIBRARIES})
target_compile_options(runme_3 PUBLIC ${GST_CFLAGS_OTHER})
