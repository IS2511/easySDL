
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "easySDL.h"


// Main easySDL functions


void easySDL::super_setup() {
    // Initializing SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { // TODO: Initialize only needed?
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    // Setting defaults
    time_step = 1000 / 60; // Default FPS is 60, TODO: VSYNC off?

    // Running user setup()
    setup();
}

void easySDL::super_update() {
    // TODO: Screen clear? Buffers? Renderer!?

    // Running user update()
    update();
}

void easySDL::super_quit() {
    if (mode3d) SDL_GL_DeleteContext(glcontext);
    SDL_Quit(); // TODO: Figure out how to quit properly?
}

void easySDL::main(void (*setupPtr)(), void (*updatePtr)()) {
    setup = setupPtr;
    update = updatePtr;

    // Initializing SDL2 + defaults and running user setup()
    super_setup();

    quit_flag = false;
    while (!quit_flag) {
        // TODO: test for SDL_QUIT event
        super_update(); // TODO: Make a good FPS mechanism
    }
    super_quit();
}

void easySDL::createWindow(const char *title, int w, int h, uint32_t flags) {
    static bool createWindow_once = true;
    if (!createWindow_once) {
        mode3d = flags && SDL_WINDOW_OPENGL;

        window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  w, h, flags);
        if (mode3d) {
            glcontext = SDL_GL_CreateContext(window);
            // TODO: set some defaults
        }
    }
}




// Utility functions (not visible)

void Error(const char* err) {
    printf("%s\nSDL error:%s\n", err, SDL_GetError());
}




// Global functions


void window(const char* title, int w, int h, uint32_t flags) {
    easySDL::createWindow(title, w, h, flags);
}

void window(const char* title, int w, int h) {
    window(title, w, h, 0);
}

void window3d(const char* title, int w, int h, uint32_t flags) {
    window(title, w, h, flags | SDL_WINDOW_OPENGL);
}

void window3d(const char* title, int w, int h) {
    window(title, w, h, SDL_WINDOW_OPENGL);
}

void delay(uint32_t ms) {
    SDL_Delay(ms);
}

void vsyncMode(bool enable) { // TODO: all that
//    if () // TODO: check for 3d
    if (enable) {
        if (!SDL_GL_SetSwapInterval(-1)) {
            if (!SDL_GL_SetSwapInterval(1)) {
                Error("Failed to enable VSYNC");
            }
        }
    } else {

    }
}

void quit() {
    easySDL::super_quit();
}










