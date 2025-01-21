cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(fish_game VERSION 1.0 DESCRIPTION "a 2D mini game" LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# Configure a header file to pass the version number to the source code
configure_file(include/fish_game/Config.hpp.in Config.hpp)

include_directories(include)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)

add_executable(fish_game ${SOURCE_FILES})

# Main SDL library for init
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static)

# SDL image to make a surface (aka what we'll render)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image-static)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer::SDL2_mixer-static)
# SDL ttf to render text
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf-static)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

