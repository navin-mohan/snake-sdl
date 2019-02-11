# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("snake")

find_package(SDL2 CONFIG REQUIRED)
# find_package(SDL2_TTF REQUIRED)
# Include sub-projects.
add_subdirectory ("snake")
