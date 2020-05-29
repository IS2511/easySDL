
// &nbsp; is a space in html, <br/> is a line break in html
// &nbsp; followed by a space is 2 spaces

/** @file
 * @brief Header with easySDL class and etc.
 */

#ifndef EASYSDL_EASYSDL_H
#define EASYSDL_EASYSDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

//#include "easySDL_opengl.h"

const float TWO_PI = 6.2831855f;
const float PI = 3.1415927f;
const float HALF_PI = 1.5707964f;
const float QUARTER_PI = 0.7853982f;

/** @class easySDL
 * @brief Helper static class, used to hide some inner mechanisms.
 * @warning You actually shouldn't use any of the static methods
 * from the this class (except main()), use their global analogs instead.
 */
class easySDL {
public:
    easySDL() = delete; // Needed?

    /** @brief Put this in your main() and pass the setup() and update() functions.
     *
     * @param setupPtr Pointer to your setup() function.
     * @param updatePtr Pointer to your update() function.
     */
    static int main(void (*setupPtr)(), void (*updatePtr)());

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
//    static bool get_windowWidth() { int w = 0; SDL_GetWindowSize(window, &w, nullptr); return w; };
//    static bool get_windowHeight() { int h = 0; SDL_GetWindowSize(window, nullptr, &h); return h; };
    static SDL_Color get_strokeColor() { return strokeColor; };
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

    static int main_return_code;
    static bool createWindow_once;
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
/// @brief How many frames were drawn, first update() is 0.
Uint32 frameCount = 0;
/// @brief FPS or frames per second, counted over 10 frames.
float frameRate = 10;
/// @brief Window width in pixels.
Uint32 width = 1;
/// @brief Window height in pixels.
Uint32 height = 1;
/// @brief Mouse X position in pixels relative to window.
int mouseX = 0;
/// @brief Mouse Y position in pixels relative to window.
int mouseY = 0;
/// @brief Previous mouse X position in pixels relative to window.
int pmouseX = 0;
/// @brief Previous mouse Y position in pixels relative to window.
int pmouseY = 0;



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

// Utils
/** @brief Converts a degree measurement to its corresponding value in radians.
 *
 * Radians and degrees are two ways of measuring the same thing.
 * There are 360 degrees in a circle and 2*PI radians in a circle.
 * For example, 90° = PI/2 = 1.5707964.
 * All trigonometric functions in easySDL require their parameters to be specified in radians.
 *
 * @param degrees degree value to convert to radians
 * @return radians converted from degrees
 */
float radians(float degrees);
/** @brief Converts a radian measurement to its corresponding value in degrees.
 *
 * Radians and degrees are two ways of measuring the same thing.
 * There are 360 degrees in a circle and 2*PI radians in a circle.
 * For example, 90° = PI/2 = 1.5707964.
 * All trigonometric functions in easySDL require their parameters to be specified in radians.
 *
 * @param radians radian value to convert to degrees
 * @return degrees converted from radians
 */
float degrees(float radians);

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
/** @brief Set the fill color. Alpha is 255 by default.
 *
 * @param r The red value (0-255)
 */
void fill(Uint8 c);
/** @brief Set the fill color.
 *
 * @note If the opacity is 0 object drawing will be skipped.
 *
 * @param c Same for Red, Green and Blue (0-255)
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
/** @brief Set the stroke color. Alpha is 255 by default.
 *
 * @param c Same for Red, Green and Blue (0-255)
 */
void stroke(Uint8 c);
/** @brief Set the stroke color.
 *
 * @note If the opacity is 0 stroke drawing will be skipped.
 *
 * @param color Color to use.
 */
void stroke(SDL_Color color);

/** @brief Fill the background with color.
 *
 * @note If the opacity is 0 stroke drawing will be skipped.
 *
 * @param r The red value (0-255)
 * @param g The green value (0-255)
 * @param b The blue value (0-255)
 * @param a The alpha value, opacity (0-255)
 */
void background(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
/** @brief Fill the background with color. Alpha is 255 by default.
 *
 * @param r The red value (0-255)
 * @param g The green value (0-255)
 * @param b The blue value (0-255)
 */
void background(Uint8 r, Uint8 g, Uint8 b);
/** @brief Fill the background with color. Alpha is 255 by default.
 *
 * @param c Same for Red, Green and Blue (0-255)
 */
void background(Uint8 c);
/** @brief Fill the background with color.
 *
 * @note If the opacity is 0 stroke drawing will be skipped.
 *
 * @param color Color to use.
 */
void background(SDL_Color color);

// 2D primitives
//void rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h)

// 3D primitives
void box(GLfloat w, GLfloat h, GLfloat d);
void box(GLfloat size);

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

/** @brief Specifies an amount to displace objects within the display window.
 *
 * Transformations are cumulative and apply to everything that happens after and subsequent calls to the function accumulates the effect.
 * For example, calling translate(50, 0) and then translate(20, 0) is the same as translate(70, 0).
 * If translate() is called within update(), the transformation is reset when the loop begins again.
 * This function can be further controlled by using pushMatrix() and popMatrix().
 *
 * @note Using this function with the z parameter requires using window3d().
 *
 * @param x Amount to move left/right
 * @param y Amount to move up/down
 * @param z Amount to move toward/away
 */
void translate(GLfloat x, GLfloat y, GLfloat z);
/** @brief Specifies an amount to displace objects within the display window.
 *
 * Transformations are cumulative and apply to everything that happens after and subsequent calls to the function accumulates the effect.
 * For example, calling translate(50, 0) and then translate(20, 0) is the same as translate(70, 0).
 * If translate() is called within update(), the transformation is reset when the loop begins again.
 * This function can be further controlled by using pushMatrix() and popMatrix().
 *
 * @note Using this function with the z parameter requires using window3d().
 *
 * @param x Amount to move left/right
 * @param y Amount to move up/down
 */
void translate(GLfloat x, GLfloat y);

/** @brief Rotates around the x-axis the amount specified by the angle parameter.
 *
 * Angles should be specified in radians (values from 0 to TWO_PI) or converted from degrees to radians with the radians() function.
 * Coordinates are always rotated around their relative position to the origin.
 * Positive numbers rotate in a clockwise direction and negative numbers rotate in a counterclockwise direction.
 * Transformations apply to everything that happens after and subsequent calls to the function accumulates the effect.
 * For example, calling rotateX(PI/2) and then rotateX(PI/2) is the same as rotateX(PI).
 * If rotateX() is run within the update(), the transformation is reset when the loop begins again.
 *
 * @param angle Angle of rotation in radians.
 */
void rotateX(GLfloat angle);
/** @brief Rotates around the y-axis the amount specified by the angle parameter.
 *
 * Angles should be specified in radians (values from 0 to TWO_PI) or converted from degrees to radians with the radians() function.
 * Coordinates are always rotated around their relative position to the origin.
 * Positive numbers rotate in a clockwise direction and negative numbers rotate in a counterclockwise direction.
 * Transformations apply to everything that happens after and subsequent calls to the function accumulates the effect.
 * For example, calling rotateY(PI/2) and then rotateY(PI/2) is the same as rotateY(PI).
 * If rotateY() is run within the update(), the transformation is reset when the loop begins again.
 *
 * @param angle Angle of rotation in radians.
 */
void rotateY(GLfloat angle);
/** @brief Rotates around the z-axis the amount specified by the angle parameter.
 *
 * Angles should be specified in radians (values from 0 to TWO_PI) or converted from degrees to radians with the radians() function.
 * Coordinates are always rotated around their relative position to the origin.
 * Positive numbers rotate in a clockwise direction and negative numbers rotate in a counterclockwise direction.
 * Transformations apply to everything that happens after and subsequent calls to the function accumulates the effect.
 * For example, calling rotateY(PI/2) and then rotateY(PI/2) is the same as rotateY(PI).
 * If rotateY() is run within the update(), the transformation is reset when the loop begins again.
 *
 * @note Using this function requires using window3d().
 *
 * @param angle Angle of rotation in radians.
 */
void rotateZ(GLfloat angle);
/** @brief Rotates the amount specified by the angle parameter.
 *
 * Angles must be specified in radians (values from 0 to TWO_PI), or they can be converted from degrees to radians with the radians() function.
 *
 * The coordinates are always rotated around their relative position to the origin.
 * Positive numbers rotate objects in a clockwise direction and negative numbers rotate in the couterclockwise direction.
 * Transformations apply to everything that happens afterward, and subsequent calls to the function compound the effect.
 * For example, calling rotate(PI/2.0) once and then calling rotate(PI/2.0) a second time is the same as a single rotate(PI).
 * All tranformations are reset when draw() begins again.
 *
 * Technically, rotate() multiplies the current transformation matrix by a rotation matrix.
 * This function can be further controlled by pushMatrix() and popMatrix().
 *
 * @param angle
 */
void rotate(GLfloat angle);

#endif //EASYSDL_EASYSDL_H
