#ifndef __VIDEO_H
#define __VIDEO_H

#include <stdbool.h>
#include <stdint.h>

/** @defgroup video video
 * @
 * Functions for using the graphics card
 */ 

/**
 * @brief Sets the video mode to the one specified
 * uses reg86_t struct to set the mode
 * @param mode mode to be set
 * @return Return 0 upon success and non-zero otherwise
*/
int (vbe_set_mode)(uint16_t mode);

/**
 * @brief Maps the video memory to the process address space
 * @param mode to get info from
 * @return Return 0 upon success and non-zero otherwise
*/
int (map_vram)(uint16_t mode);

/**
 * @brief Draws a pixel at the specified location
 * @param x x coordinate
 * @param y y coordinate
 * @param color color of the pixel
 * @return Return 0 upon success and non-zero otherwise 
*/
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line
 * @param x x coordinate of the starting point
 * @param y y coordinate 
 * @param len length of the line
 * @param color color of the line
 * @return Return 0 upon success and non-zero otherwise
*/
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle
 * @param x x coordinate of the starting point
 * @param y y coordinate of the starting point
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param color color of the rectangle
 * @return Return 0 upon success and non-zero otherwise
*/
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, 
            uint16_t height, uint32_t color);

#endif /* __VIDEO_H */
