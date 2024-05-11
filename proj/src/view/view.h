#ifndef _VIEW_H_
#define _VIEW_H_


#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../drivers/graphics/video.h"
#include "../model/sprite.h"
#include "../model/model.h"
#include "model/text.h"

int setUpFrameBuffer();
int mainDraw();
int drawSpriteXPM(Sprite *sprite, int x, int y);
int drawBackground();

/**
 * @brief Draws the cursor on the screen
 * @return 0 if successful, 1 otherwise
*/
int drawCursor();
int GameDrawer();

/**
 * @brief Draws a text on the screen
 * @param text the text to be drawn
 * @return 0 if successful, 1 otherwise
*/
int drawText(const char* text);

/**
 * @brief Draws a letter on the screen
 * @param key the key to be drawn
 * @return 0 if successful, 1 otherwise
*/
int drawLetter(Key key);

#endif
