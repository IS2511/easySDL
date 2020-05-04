
project(TestGame)

include_directories(${easySDL_INCLUDE_DIRS} ${Project_SOURCE_DIR}/easySDL/inc)

add_executable(TestGame TestGame.cpp)
target_link_libraries(TestGame easySDL) # TODO: Link to SDL2 and OpenGL needed?