cmake_minimum_required(VERSION 3.10)

add_executable(runme_1 main.c)
target_link_libraries(runme_1 ${LIBRARIES})
if (NOT APPLE)
    target_compile_options(runme_1 PUBLIC ${GST_CFLAGS_OTHER})
endif()
