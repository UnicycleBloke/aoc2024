# Generate project name from the folder name in which this this file is included.
get_filename_component(AOC_PROGRAM_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" AOC_PROGRAM_NAME ${AOC_PROGRAM_NAME})

project(${AOC_PROGRAM_NAME} LANGUAGES CXX)

add_executable(${AOC_PROGRAM_NAME} solution.cpp)

target_include_directories(${AOC_PROGRAM_NAME} PUBLIC
    .
    ../utils
)

target_compile_options(${AOC_PROGRAM_NAME} PUBLIC -std=c++20 -O2)
#target_link_libraries(${AOC_PROGRAM_NAME} curses aocutils)
target_link_libraries(${AOC_PROGRAM_NAME} aocutils)
