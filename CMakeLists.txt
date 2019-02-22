# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("snake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
    )

set(
    CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/lib
)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL_TTF_INCLUDE_DIRS})


# find_package(SDL2_TTF REQUIRED)
# Include sub-projects.
add_subdirectory ("snake")

# copy over the required DLLs on windows
if(WIN32)
    file(GLOB DLLS ${SDL2_DIR}lib/x86/*.dll )
    file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR}/bin)
    file(GLOB DLLS ${SDLTTFDIR}lib/x86/*.dll ) 
    file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR}/bin)

	file(COPY ${PROJECT_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()

add_executable(snakegame "main.cpp")

target_link_libraries(snakegame PRIVATE snake ${SDL_TTF_LIBRARIES})