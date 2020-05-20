
project(TestGame)

include_directories(${easySDL_INCLUDE_DIRS} ${Project_SOURCE_DIR}/easySDL/inc)

find_package(OpenGL REQUIRED)

add_executable(TestGame TestGame.cpp)
target_link_libraries(TestGame easySDL OpenGL) # TODO: Link to SDL2 and OpenGL needed?