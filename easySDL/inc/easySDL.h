
/** @file
 * @brief Header with easySDL class and etc.
 */

#ifndef EASYSDL_EASYSDL_H
#define EASYSDL_EASYSDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

//#include "easySDL_timer.h"
//#include "easySDL_opengl.h"

/** @class easySDL
 * @brief Helper static class, used to hide some inner mechanisms.
 * @warning You actually shouldn't use any of the static methods
 * from the this class (except main()), use their global analogs instead.
 */
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
    static void createWindow(const char* title, int w, int h, Uint32 flags);


    static bool getmode3d() { return mode3d; };

private: // Yeah, I'm not documenting private
    static SDL_Window* window;
    static SDL_GLContext glcontext;

    static void super_setup();
    static void super_update();
    static void handle_event(SDL_Event* event);

    static bool quit_flag;
    static bool mode3d;
    static bool vsync;
    static Uint32 time_step;
    static Uint32 last_step;
    static Uint32 frameTimes[10];
};




// Global variables

/** @brief How many milliseconds passed since last update() call.
 *
 * Use for physics calculation and etc.
 */
Uint32 frameDelta;
Uint32 frameCount = 0;
float frameRate = 10;



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
void window(const char* title, int w, int h, Uint32 flags);

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
void window3d(const char* title, int w, int h, Uint32 flags);

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
void delay(Uint32 ms);

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
