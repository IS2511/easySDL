
project(easySDL)

#find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ../inc)

add_library(easySDL SHARED easySDL.cpp)
target_link_libraries(easySDL SDL2)