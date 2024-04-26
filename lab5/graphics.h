#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h"

vbe_mode_info_t mode_info;
uint8_t* frame_buffer;


int (set_graphic_mode)(uint16_t submode);
int (set_frame_buffer)(uint16_t mode);
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint16_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int normalize_color(uint16_t color, uint32_t *new_color);

#endif /* __GRAPHICS_H */
