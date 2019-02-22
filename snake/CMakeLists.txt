# CMakeList.txt : CMake project for snake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)




# Add source to this project's executable.
add_library(snake "snake.cpp" )

target_include_directories(snake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(snake PUBLIC ${SDL2_LIBRARIES})


# TODO: Add tests and install targets if needed.
