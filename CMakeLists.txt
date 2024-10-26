cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(fish_game VERSION 1.0 DESCRIPTION "a 2D mini game" LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# Configure a header file to pass the version number to the source code
configure_file(include/fish_game/Config.hpp.in Config.hpp)

include_directories(include)

# Find SDL2 and SDL2_image packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(fish_game ${SOURCE_FILES})

# Include SDL2 and SDL2_image directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Link SDL2 and SDL2_image libraries
target_link_libraries(fish_game PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# Debugging output to check paths
message(STATUS "SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries: ${SDL2_LIBRARIES}")
message(STATUS "SDL2_image include dirs: ${SDL2_IMAGE_INCLUDE_DIRS}")
message(STATUS "SDL2_image libraries: ${SDL2_IMAGE_LIBRARIES}")
