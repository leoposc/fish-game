cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(fish_game VERSION 1.0 DESCRIPTION "a 2D mini game" LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# Configure a header file to pass the version number to the source code
configure_file(include/fish_game/Config.hpp.in Config.hpp)

# target_include_directories(Fish_game PRIVATE
# "${CMAKE_CURRENT_BINARY_DIR}"
# "${CMAKE_CURRENT_SOURCE_DIR}/src"
# "${CMAKE_CURRENT_SOURCE_DIR}/include"
# "${CMAKE_CURRENT_SOURCE_DIR}/include/fish_game"
# "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.0.14/include"                          
# )


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/fish_game)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/fish_game/ECS)
# include_directories(include/fish_game)
add_executable(fish_game ${SOURCE_FILES})

# find SDL" package
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("/usr/local/include/SDL2")
# include_directories(%{CMAKE_CURRENT_SOURCE_DIR}/include/fish_game)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include/SDL2")
# target_link_libraries(fish_game ${SDL2_LIBRARIES})
# target_link_libraries(fish_game ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(fish_game PRIVATE SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image)

# Debugging output to check paths
message(STATUS "SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries: ${SDL2_LIBRARIES}")
message(STATUS "SDL2_image include dirs: /usr/local/include/SDL2")
message(STATUS "SDL2_image library: /usr/local/lib/libSDL2_image.dylib")
