#ifndef _VIEW_H_
#define _VIEW_H_


#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../drivers/graphics/video.h"
#include "../model/sprite.h"
#include "../model/model.h"

int setUpFrameBuffer();
int mainDraw();
int drawSpriteXPM(Sprite *sprite, int x, int y);
int drawCurrentLetter();
int drawBackground();

/**
 * @brief Draws the cursor on the screen
 * @return 0 if successful, 1 otherwise
*/
int draw_cursor();

/**
 * @brief Draws a text on the screen
 * @param text the text to be drawn
 * @param start_x the x coordinate of the starting point
 * @param start_y the y coordinate of the starting point
 * @return 0 if successful, 1 otherwise
*/
int draw_text(const char* text, int start_x, int start_y);

/**
 * @brief Draws a letter on the screen
 * @param key the key to be drawn
 * @param x the x coordinate of the starting point
 * @param y the y coordinate of the starting point
 * @return 0 if successful, 1 otherwise
*/
int draw_letter(Key key, int x, int y);

#endif
