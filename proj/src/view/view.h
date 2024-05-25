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
int drawSpriteXPM_single_color(Sprite *sprite, int x, int y, uint32_t color);
int drawBackground();


/**
 * @brief Draws the record time on the screen
 * @return 0 if successful, 1 otherwise
*/
int drawRecordedTime();

/**
 * @brief Draws a number on the screen
 * @param key the key to be drawn
 * @param x the x position of the number
 * @param y the y position of the number
 * @return 0 if successful, 1 otherwise
*/
int drawNumber(Key key, int x, int y);

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
int drawLetter(Key key, uint32_t color);

int drawWords(TypingTest *test);

int word_length_in_pixels(const char *word);

int num_words(char **words);

#endif
