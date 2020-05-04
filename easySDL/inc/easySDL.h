
#ifndef EASYSDL_EASYSDL_H
#define EASYSDL_EASYSDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

//#include "easySDL_timer.h"
//#include "easySDL_opengl.h"

class easySDL {
public:
    easySDL() = delete; // Needed?

    /** @brief Executes once at program start.
     *
     * It's preferred to call window() here.
     */
    static void (*setup)();
    /** @brief Executes on every frame.
     *
     * Drawing and calculating.
     */
    static void (*update)();

    // &nbsp; is a space in html, <br/> is a line break in html
    // &nbsp; followed by a space is 2 spaces
    /** @brief Put this in your main() and pass the setup() and update() functions.
     *
     * @example int main() { <br/>
     *     &nbsp; &nbsp; easySDL::main(setup, update); <br/>
     *   }
     *
     * @param setupPtr Pointer to your setup() function.
     * @param updatePtr Pointer to your update() function.
     */
    static void main(void (*setupPtr)(), void (*updatePtr)());

    /** @brief Not intended for usage, use quit() instead.
     *
     * @see quit()
     */
    static void super_quit();

    /** @brief Not intended for usage, use window() or window3d() instead.
     *
     * @see window()
     * @see window3d()
     */
    static void createWindow(const char* title, int w, int h, uint32_t flags);


    static void getmode3d();

private: // Yeah, I'm not documenting private
    static SDL_Window* window;
    static SDL_GLContext glcontext;

    static void super_setup();
    static void super_update();

    static bool quit_flag;
    static bool mode3d;
    static uint32_t time_step;
};




// Global variables

/** @brief How many milliseconds passed since last update() call.
 *
 * Use for physics calculation and etc.
 */
uint32_t frameDelta;



// Global functions


/** @brief Create a window with SDL2 flags.
 *
 * @note Can only be run once.
 *
 * @param title Window title in UTF-8.
 * @param w Window width in screen coordinates.
 * @param h Window height in screen coordinates.
 * @param flags SDL2 flags for SDL_CreateWindow().
 */
void window(const char* title, int w, int h, uint32_t flags);

/** @brief Create a window with default SDL2 flags.
 *
 * @note Can only be run once.
 *
 * @param title Window title in UTF-8.
 * @param w Window width in screen coordinates.
 * @param h Window height in screen coordinates.
 */
void window(const char* title, int w, int h);

/** @brief Create a window with SDL2 flags and OpenGL context (3D).
 *
 * @note Can only be run once.
 *
 * @param title Window title in UTF-8.
 * @param w Window width in screen coordinates.
 * @param h Window height in screen coordinates.
 */
void window3d(const char* title, int w, int h, uint32_t flags);

/** @brief Create a window with default SDL2 flags and OpenGL context (3D).
 *
 * @note Can only be run once.
 *
 * @param title Window title in UTF-8.
 * @param w Window width in screen coordinates.
 * @param h Window height in screen coordinates.
 */
void window3d(const char* title, int w, int h);

/** @brief Wait a specified number of milliseconds before returning.
 *
 * @note On some systems the minimum time can be 10-15 ms.
 *
 * @param ms Number of milliseconds.
 */
void delay(uint32_t ms);

/** @brief Enable or disable vsync.
 *
 * @note Tries for adaptive vsync first ( SDL_GL_SetSwapInterval(-1) ).
 *
 * @param enable True to enable, false to disable.
 */
void vsyncMode(bool enable);

/// @brief Quit with proper cleanup
void quit();

#endif //EASYSDL_EASYSDL_H
