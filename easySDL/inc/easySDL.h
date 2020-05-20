
// &nbsp; is a space in html, <br/> is a line break in html
// &nbsp; followed by a space is 2 spaces

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

//    /** @brief Put this in your main() and pass the setup() and update() functions.
//     *
//     * @param setupPtr Pointer to your setup() function.
//     * @param updatePtr Pointer to your update() function.
//     */
    static void main(void (*setupPtr)(), void (*updatePtr)());

//    /** @brief Not intended for usage, use quit() instead.
//     *
//     * @see quit()
//     */
    static void super_quit();

//    /** @brief Not intended for usage, use window() or window3d() instead.
//     *
//     * @see window()
//     * @see window3d()
//     */
    static void createWindow(const char* title, int w, int h, Uint32 flags);


    // "Get" functions
    static bool get_mode3d() { return mode3d; };
    static bool get_vsync() { return vsync; };
    static bool get_windowFlags() { return SDL_GetWindowFlags(window); };
//    static void set_windowFlags(Uint32 flags) { return SDL_SetWindowFlags(window, flags); }; // TODO: remove/replace


    // Public -> private functions
    static void vsyncMode(bool enable);
    static void fill(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    static void stroke(Uint8 r, Uint8 g, Uint8 b, Uint8 a);



private: // Yeah, I'm not documenting private
    static void (*setup)();
    static void (*update)();

    static SDL_Window* window;
    static SDL_Renderer* renderer;
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
    static SDL_Color strokeColor;
};




// Global variables

/// @brief How many milliseconds passed since last update() call.
Uint32 frameDelta;
/// @brief How many frames were drawn, first update() is 0
Uint32 frameCount = 0;
/// @brief FPS or frames per second, counted over 10 frames
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

/** @brief Turns vsync on or off.
 *
 * @note Tries for adaptive vsync first ( SDL_GL_SetSwapInterval(-1) ).
 *
 * @param enable True to enable, false to disable.
 */
void vsyncMode(bool enable);

/** @brief Get vsync state.
 *
 *
 */
bool vsyncMode();

/** @brief Set window flags.
 *
 * @param flags Window flags.
 */
void windowFlags(Uint32 flags);

/** @brief Get window flags.
 *
 *
 */
Uint32 windowFlags();

/// @brief Quit with proper cleanup.
void quit();

// Time
/** @brief Wait a specified number of milliseconds before returning.
 *
 * @note On some systems the minimum time can be 10-15 ms.
 *
 * @param ms Number of milliseconds.
 */
void delay(Uint32 ms);

/// @brief Returns the number of milliseconds since starting the program.
Uint32 millis();

// Color
/** @brief Set the fill color.
 *
 * @note If the opacity is 0 object drawing will be skipped.
 *
 * @param r The red value (0-255)
 * @param g The green value (0-255)
 * @param b The blue value (0-255)
 * @param a The alpha value, opacity (0-255)
 */
void fill(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
/** @brief Set the fill color. Alpha is 255 by default.
 *
 * @param r The red value (0-255)
 * @param g The green value (0-255)
 * @param b The blue value (0-255)
 */
void fill(Uint8 r, Uint8 g, Uint8 b);
/** @brief Set the fill color.
 *
 * @note If the opacity is 0 object drawing will be skipped.
 *
 * @param color Color to use.
 */
void fill(SDL_Color color);

/** @brief Set the stroke color.
 *
 * @note If the opacity is 0 stroke drawing will be skipped.
 *
 * @param r The red value (0-255)
 * @param g The green value (0-255)
 * @param b The blue value (0-255)
 * @param a The alpha value, opacity (0-255)
 */
void stroke(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
/** @brief Set the stroke color. Alpha is 255 by default.
 *
 * @param r The red value (0-255)
 * @param g The green value (0-255)
 * @param b The blue value (0-255)
 */
void stroke(Uint8 r, Uint8 g, Uint8 b);
/** @brief Set the stroke color.
 *
 * @note If the opacity is 0 stroke drawing will be skipped.
 *
 * @param color Color to use.
 */
void stroke(SDL_Color color);

// Matrix
/** @brief Pushes current transformation matrix to the stack.
 *
 * Understanding pushMatrix() and popMatrix() requires understanding the concept of a matrix stack.
 * The pushMatrix() function saves the current coordinate system to the stack and popMatrix() restores the prior coordinate system.
 * pushMatrix() and popMatrix() are used in conjuction with the other transformation functions and may be embedded to control the scope of the transformations.
 */
void pushMatrix();

/** @brief Pops current transformation matrix from the stack.
 *
 * Understanding pushMatrix() and popMatrix() requires understanding the concept of a matrix stack.
 * The pushMatrix() function saves the current coordinate system to the stack and popMatrix() restores the prior coordinate system.
 * pushMatrix() and popMatrix() are used in conjuction with the other transformation functions and may be embedded to control the scope of the transformations.
 */
void popMatrix();

#endif //EASYSDL_EASYSDL_H
