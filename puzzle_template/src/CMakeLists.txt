project(puzzle)

add_library(puzzle puzzle.c puzzle.h)
target_link_libraries(puzzle PRIVATE pthread)

add_executable(run_puzzle run_puzzle.c)
target_link_libraries(run_puzzle PRIVATE puzzle pthread)
