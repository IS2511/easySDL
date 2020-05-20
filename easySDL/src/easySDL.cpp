
/** @file
 * @brief Main file containing basic functions.
 */

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>

#include "easySDL.h"

// Main easySDL variables

void (*easySDL::setup)();
void (*easySDL::update)();

SDL_Window* easySDL::window;
SDL_Renderer* easySDL::renderer;
SDL_GLContext easySDL::glcontext;

bool easySDL::quit_flag = false;
bool easySDL::mode3d = false;
bool easySDL::vsync = false;
Uint32 easySDL::time_step = 0;
Uint32 easySDL::last_step = 0;
Uint32 easySDL::frameTimes[10] = {0};
SDL_Color easySDL::strokeColor = { 0, 0, 0, 255};




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

    SDL_Event event;
    while (SDL_PollEvent(&event))
        handle_event(&event);

    // Running user update()
    update();
    frameCount++;
}

void easySDL::super_quit() {
    static bool super_quit_once = false;
    if (!super_quit_once) {
        if (mode3d) SDL_GL_DeleteContext(glcontext);
        SDL_Quit(); // TODO: Figure out how to quit properly?
        quit_flag = true;
        super_quit_once = true;
//        printf("[DEBUG] Super quit!\n");
    }
}

void easySDL::main(void (*setupPtr)(), void (*updatePtr)()) {
    setup = setupPtr;
    update = updatePtr;

    // Init before setup so quit() works in setup()
    quit_flag = false;

    // Initializing SDL2 + defaults and running user setup()
    super_setup();

    last_step = SDL_GetTicks();
    while (!quit_flag) {
        Uint32 now = SDL_GetTicks();

        if (now - last_step >= time_step || vsync) { // Check for max FPS

            frameDelta = now - last_step;
            frameTimes[frameCount%10] = frameDelta;
            if (frameCount > 8) {
                frameRate = 0;
                for (Uint32 frameTime : frameTimes) frameRate += frameTime;
                frameRate = 1000/(frameRate/10);
            }

            super_update();

            last_step = SDL_GetTicks();
            // TODO: Render here (swap buffers and etc.)
            if (mode3d) {
                SDL_GL_SwapWindow(window);
            } else {
                SDL_RenderPresent(renderer);
            }
        } else { // Don't fry the CPU
            SDL_Delay(1);
        }

    }
    super_quit();
}

void easySDL::handle_event(SDL_Event* event) {
    switch (event->type) { // TODO: Add more events
        case SDL_QUIT:
            // TODO: Handle! global Quit()?
            super_quit();
            break;
    }
}

void easySDL::createWindow(const char *title, int w, int h, Uint32 flags) {
    static bool createWindow_once = false;
    if (!createWindow_once) {
        mode3d = flags && SDL_WINDOW_OPENGL;

        window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  w, h, flags);
        if (mode3d) {
            glcontext = SDL_GL_CreateContext(window);
            // TODO: set some defaults
        } else {
            renderer = SDL_CreateRenderer(window, -1, 0); // TODO: Any flags?
        }
    }
}

void easySDL::vsyncMode(bool enable) {
    if (enable == vsync) return;
    if (mode3d) {
        // TODO: Write vsync toggle
    } else {

    }
    vsync = enable;
}

void easySDL::fill(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (mode3d) {
        glColor4b(r, g, b, a);
    } else {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }
}

void easySDL::stroke(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    strokeColor = {r, g, b, a};
    if (mode3d) {
        glColor4b(r, g, b, a);
    } else {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }
}




// Utility functions (not visible)

void Error(const char* err) {
    printf("%s\nSDL error:%s\n", err, SDL_GetError());
}




// Global functions


void window(const char* title, int w, int h, Uint32 flags) {
    easySDL::createWindow(title, w, h, flags);
}

void window(const char* title, int w, int h) {
    window(title, w, h, 0);
}

void window3d(const char* title, int w, int h, Uint32 flags) {
    window(title, w, h, flags | SDL_WINDOW_OPENGL);
}

void window3d(const char* title, int w, int h) {
    window(title, w, h, SDL_WINDOW_OPENGL);
}

void vsyncMode(bool enable) {
    if (easySDL::get_vsync() == enable) return;
    easySDL::vsyncMode(enable);
}

bool vsyncMode() {
    return easySDL::get_vsync();
}

Uint32 windowFlags() {
    return easySDL::get_windowFlags();
}

void windowFlags(Uint32 flags) { // TODO: all that
//    easySDL::set_windowFlags(flags); // TODO: replace/remove

//    if (flags & EASYSDL_WINDOW_VSYNC)
//        if (easySDL::get_mode3d()) { // TODO: check for 3d
//            if (!SDL_GL_SetSwapInterval(-1)) {
//                if (!SDL_GL_SetSwapInterval(1)) {
//                    Error("Failed to enable VSYNC");
//                }
//            }
//        } else {
//
//        }
//    }
}

void quit() {
    easySDL::super_quit();
}

// Time
void delay(Uint32 ms) {
    SDL_Delay(ms);
}

Uint32 millis() {
    return SDL_GetTicks();
}

// Color
void fill(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    easySDL::fill(r, g, b, a);
};
void fill(Uint8 r, Uint8 g, Uint8 b) {fill(r, g, b, 255); };
void fill(SDL_Color c) { fill(c.r, c.g, c.b, c.a); };

void stroke(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    easySDL::stroke(r, g, b, a);
};
void stroke(Uint8 r, Uint8 g, Uint8 b) {fill(r, g, b, 255); };
void stroke(SDL_Color c) { fill(c.r, c.g, c.b, c.a); };

// Matrix
void pushMatrix() {
    if (easySDL::get_mode3d()) {
        glPushMatrix();
    } else {
        // TODO: Write a matrix implementation
    }
}

void popMatrix() {
    if (easySDL::get_mode3d()) {
        glPopMatrix();
    } else {
        // TODO: Write a matrix implementation
    }
}











