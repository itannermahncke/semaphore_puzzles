project(Producer_Consumer)

add_library(pc pc.c pc.h)
target_link_libraries(pc PRIVATE pthread)

add_executable(run_pc run_pc.c)
target_link_libraries(run_pc PRIVATE pc pthread)
