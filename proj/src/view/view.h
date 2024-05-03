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

#endif
