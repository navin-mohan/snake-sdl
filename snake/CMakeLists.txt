# CMakeList.txt : CMake project for snake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


# Add source to this project's executable.
add_executable (snake "main.cpp" "snake.hpp" "snake.cpp" )

target_link_libraries(snake PRIVATE ${SDL2_LIBRARIES} ${SDL_TTF_LIBRARIES})

# TODO: Add tests and install targets if needed.
