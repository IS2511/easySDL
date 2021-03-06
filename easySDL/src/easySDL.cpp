
/** @file
 * @brief Main file containing basic functions.
 */

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>

#include "easySDL.h"

#include <string>

// Main easySDL variables

void (*easySDL::setup)();
void (*easySDL::update)();

SDL_Window* easySDL::window;
SDL_Renderer* easySDL::renderer;
SDL_GLContext easySDL::glcontext;
EventHandlerPtr easySDL::eventHandler[SDL_LASTEVENT] = {nullptr}; // Is this necessary

int easySDL::main_return_code = 0;
bool easySDL::createWindow_once = false;
bool easySDL::super_setup_once = false;
bool easySDL::quit_flag = false;
bool easySDL::mode3d = false;
bool easySDL::vsync = false;
Uint32 easySDL::time_step = 0;
Uint32 easySDL::last_step = 0;
Uint32 easySDL::frameTimes[10] = {0};
SDL_Color easySDL::fillColor = { 255, 255, 255, 255};
SDL_Color easySDL::strokeColor = { 0, 0, 0, 255};




// Utility functions (not visible)

void ErrorSDL(std::string err) {
    printf("[SDL_ERROR] %s\nSDL error:%s\n", err.c_str(), SDL_GetError());
}

void Error(std::string err) {
    printf("[ERROR] %s\n", err.c_str());
}

void Warn(std::string str) {
    printf("[WARNING] %s\n", str.c_str());
}

void Log(std::string str) {
    printf("[LOG] %s\n", str.c_str());
}

void Debug(std::string str) {
    printf("[DEBUG] %s\n", str.c_str());
}




// Main easySDL functions


void easySDL::super_setup() {
    if (!super_setup_once) {
        super_setup_once = true;

        // Initializing SDL2
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { // TODO: Initialize only needed?
            printf("Error initializing SDL: %s\n", SDL_GetError());
            quit_flag = true;
            main_return_code = -1; // Critical failure or something
        }

        // Setting defaults
        time_step = 1000 / 60; // Default FPS is 60

        // Running user setup()
        setup();

        if (!createWindow_once) {
            Warn("No window created in setup!");
        }
    }
}

void easySDL::super_update() {

    SDL_Event event;
    while (SDL_PollEvent(&event))
        handle_event(&event);

    pmouseX = mouseX; pmouseY = mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mode3d) {
        glLoadIdentity();
        glTranslatef(-1.0f, 1.0f, 0.0f); // Translating origin to top left
        glScalef(2.0f/width, 2.0f/height, 2.0f/width);
        glScalef(1.0f, -1.0f, 1.0f);
    }

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

int easySDL::main(void (*setupPtr)(), void (*updatePtr)()) {
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
    return main_return_code;
}

void easySDL::handle_event(SDL_Event* event) {
    switch (event->type) { // TODO: Add more special cases
        case SDL_QUIT:
            // TODO: Handle! global Quit()?
            super_quit();
            break;
        default:
            if (eventHandler[event->type] != nullptr) {
                eventHandler[event->type](event);
            }
            break;
    }
}

void easySDL::createWindow(const char *title, int w, int h, Uint32 flags) {
    if (!createWindow_once) {
        mode3d = flags && SDL_WINDOW_OPENGL;

        window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  w, h, flags);
        width = w; height = h;
        if (mode3d) {
            glcontext = SDL_GL_CreateContext(window);
            vsyncMode(false); // vsync is off by default
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_MULTISAMPLE);
            glEnable(GL_LINE_SMOOTH);
//            glEnable(GL_POLYGON_SMOOTH);
            // TODO: MORE glEnable()!!!
            // TODO: Figure out good line antialiasing!
            // TODO: Some day we will even have culling... Some day...
        } else {
            renderer = SDL_CreateRenderer(window, -1, 0); // TODO: Any flags? I think defaults are OK
        }
        createWindow_once = true;
    }
}

void easySDL::registerHandler(SDL_EventType eventType, EventHandlerPtr handler) {
    // TODO: Register events based on type!!!
    eventHandler[eventType] = handler; // TODO: Check if we are in setup()?
}

void easySDL::unregisterHandler(SDL_EventType eventType) {
    eventHandler[eventType] = nullptr;
}

void easySDL::vsyncMode(bool enable) {
    if (enable == vsync) return;
    if (mode3d) {
        if (enable) {
            if (!SDL_GL_SetSwapInterval(-1)) {
                if (!SDL_GL_SetSwapInterval(1)) {
                    ErrorSDL("Failed to enable VSYNC!");
                    return;
                }
            }
        } else {
            if (!SDL_GL_SetSwapInterval(0)) {
                ErrorSDL("Failed to disable VSYNC!");
                return;
            }
        }
    } else {

    }
    vsync = enable;
}

void easySDL::fill(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    fillColor = {r, g, b, a};
}

void easySDL::stroke(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    strokeColor = {r, g, b, a};
}




// Global functions


void window(const char* title, int w, int h, Uint32 flags) {
    easySDL::createWindow(title, w, h, flags);
}

void window3d(const char* title, int w, int h, Uint32 flags) {
    window(title, w, h, flags | SDL_WINDOW_OPENGL);
}

void vsyncMode(bool enable) {
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

}

void quit() {
    easySDL::super_quit();
}

// Utils
float radians(float degrees) {
    return degrees*(PI/180);
}

float degrees(float radians) {
    return radians*(180/PI);
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
}
void fill(Uint8 c) { fill(c, c, c, 255); }
void fill(SDL_Color color) { fill(color.r, color.g, color.b, color.a); }

void stroke(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    easySDL::stroke(r, g, b, a);
}
void stroke(Uint8 c) { stroke(c, c, c, 255); }
void stroke(SDL_Color color) { stroke(color.r, color.g, color.b, color.a); }

void background(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    glClearColor((float)r/255, (float)g/255, (float)b/255, (float)a/255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void background(Uint8 c) { background(c, c, c, 255); }
void background(SDL_Color color) { background(color.r, color.g, color.b, color.a); }

// 2D primitives
//void rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
//
//}

// 3D primitives
void box(GLfloat w, GLfloat h, GLfloat d) {
    if (!easySDL::get_mode3d()) return;
    if (w == 0 or h == 0 or d == 0) return;
    pushMatrix();
    glScalef(w, h, d);

    // Fill
    SDL_Color c = easySDL::get_fillColor();
    if (c.a != 0) {
        glColor4ub(c.r, c.g, c.b, c.a);
        glBegin(GL_QUADS);
        // Top
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);

        // Front
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);

        // Right
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);

        // Left
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);

        // Bottom
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);

        // Back
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);

        glEnd();
    }

    // Stroke
    c = easySDL::get_strokeColor();
    if (c.a != 0) {
        glColor4ub(c.r, c.g, c.b, c.a);
        //   5 +---+ 6  // 1 (-0.5f,  0.5f,  0.5f)
        //     | B |    // 2 ( 0.5f,  0.5f,  0.5f)
        //   8 +---+ 7  // 3 ( 0.5f, -0.5f,  0.5f)
        //    // //     // 4 (-0.5f, -0.5f,  0.5f)
        // 1 +---+ 2    // 5 (-0.5f,  0.5f, -0.5f)
        //   | F |      // 6 ( 0.5f,  0.5f, -0.5f)
        // 4 +---+ 3    // 7 ( 0.5f, -0.5f, -0.5f)
        //              // 8 (-0.5f, -0.5f, -0.5f)
        // 2143267856 15 37 48
        glBegin(GL_LINE_STRIP);
        glVertex3f( 0.5f,  0.5f,  0.5f); // 2
        glVertex3f(-0.5f,  0.5f,  0.5f); // 1
        glVertex3f(-0.5f, -0.5f,  0.5f); // 4
        glVertex3f( 0.5f, -0.5f,  0.5f); // 3
        glVertex3f( 0.5f,  0.5f,  0.5f); // 2
        glVertex3f( 0.5f,  0.5f, -0.5f); // 6
        glVertex3f( 0.5f, -0.5f, -0.5f); // 7
        glVertex3f(-0.5f, -0.5f, -0.5f); // 8
        glVertex3f(-0.5f,  0.5f, -0.5f); // 5
        glVertex3f( 0.5f,  0.5f, -0.5f); // 6
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);

        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);

        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glEnd();
    }
    popMatrix();
}
void box(GLfloat size) {
    box(size, size, size);
}

// Matrix
void pushMatrix() {
    if (easySDL::get_mode3d()) {
        glPushMatrix();
    } else {
        // TODO: Write a matrix implementation
        Warn("No matrix manipulation for 2D mode yet!");
    }
}

void popMatrix() {
    if (easySDL::get_mode3d()) {
        glPopMatrix();
    } else {
        // TODO: Write a matrix implementation
        Warn("No matrix manipulation for 2D mode yet!");
    }
}

void translate(GLfloat x, GLfloat y, GLfloat z) {
    if (easySDL::get_mode3d()) {
        glTranslatef(x, y, z);
    } else {
        if (z != 0.0f) return;
        Warn("No matrix manipulation for 2D mode yet!");
    }
}

void rotateX(GLfloat angle) {
    if (!easySDL::get_mode3d()) return;
    glRotatef(degrees(angle), 1.0f, 0.0f, 0.0f);
}
void rotateY(GLfloat angle) {
    if (!easySDL::get_mode3d()) return;
    glRotatef(degrees(angle), 0.0f, 1.0f, 0.0f);
}
void rotateZ(GLfloat angle) {
    if (easySDL::get_mode3d()) {
        glRotatef(degrees(angle), 0.0f, 0.0f, 1.0f);
    } else {
        Warn("No matrix manipulation for 2D mode yet!");
    }
}
void rotate(GLfloat angle) {
    rotateZ(angle);
}
