set(AOC_UTILS_NAME "aocutils")
add_library(${AOC_UTILS_NAME} STATIC utils.cpp)
target_compile_options(${AOC_UTILS_NAME} PUBLIC -std=c++20 -O3)
