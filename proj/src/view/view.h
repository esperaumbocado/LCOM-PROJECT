#ifndef _VIEW_H_
#define _VIEW_H_


#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../drivers/graphics/video.h"
#include "../model/sprite.h"
#include "../model/model.h"

int setUpFrameBuffer();
int mainDraw();

/**
 * @brief Draws a sprite on the screen, secondary_frame_buffer_no_mouse
 * @param sprite the sprite to be drawn
 * @param x the x position of the sprite
 * @param y the y position of the sprite
 * @param single_color if the sprite is a single color
 * @param color the color of the sprite
 * @param moving if the sprite will be moving
 * @return 0 if successful, 1 otherwise
*/
int drawSpriteXPM(Sprite *sprite, int x, int y, bool single_color, uint32_t color, bool moving);

/**
 * @brief Draws a static sprite on the screen
 * @param sprite the sprite to be drawn
 * @return 0 if successful, 1 otherwise
*/
int drawStatic(Sprite *sprite);

int drawBackground(GameState state);


/**
 * @brief Draws the record time on the screen
 * @return 0 if successful, 1 otherwise
*/
int drawRecordedTime();

/**
 * @brief Draws the remaining stars on the screen
 * @return 0 if successful, 1 otherwise
*/
int drawStars();

/**
 * @brief Displays the timers xpms on the screen
 * @return 0 if successful, 1 otherwise
*/
int drawTimers();

/**
 * @brief Draws the real-time clock on the screen.
 * @return 0 if successful, 1 otherwise
*/
int drawRealTime();


int drawStatistics();

/**
 * @brief Draws the cursor on the screen
 * @return 0 if successful, 1 otherwise
*/
int drawCursor();
int GameDrawer();

/**
 * @brief Draws a text on the screen
 * @param text the text to be drawn
 * @param color the color of the text
 * @return 0 if successful, 1 otherwise
*/
int drawText(const char* text, uint32_t color, int start_x, int end_x, int y);

int drawWords(TypingTest *test);

int word_length_in_pixels(Word *word);

int deleteCaret();

int drawCaret(int x,int y);

int drawInt(int number, int x, int y);

#endif
