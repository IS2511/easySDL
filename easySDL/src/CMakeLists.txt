
project(easySDL)

find_package(SDL2 REQUIRED)
find_package(SDL_mixer)
if (NOT SDL_MIXER_FOUND)
    message(WARNING "SDL_mixer not found! No sound will be available!")
endif ()

include_directories(${SDL_INCLUDE_DIRS} ${Project_SOURCE_DIR}/easySDL/inc)
if (SDL_MIXER_FOUND)
    include_directories(${SDL_MIXER_INCLUDE_DIRS})
endif ()

add_library(easySDL SHARED easySDL.cpp)

target_link_libraries(easySDL SDL2)
if (SDL_MIXER_FOUND)
    target_link_libraries(easySDL SDL_mixer)
endif ()