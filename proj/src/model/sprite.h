#ifndef _SPRITE_H_
#define _SPRITE_H_

#
#include "../drivers/graphics/video.h"

typedef struct {
    uint16_t height;
    uint16_t width;
    uint16_t x;
    uint16_t y;
    uint32_t *colors;
    uint32_t color;
} Sprite; 


typedef struct {
    Sprite **frames;
    int frameCount;
    int currentFrame; 
    bool isActive; 
    int frameDuration;
    int frameCounter;
} Animation;


Sprite *create_sprite_xpm(xpm_map_t sprite, uint16_t x, uint16_t y);
Sprite *create_sprite_button(uint16_t width, uint16_t height, uint32_t color);
void destroy_sprite(Sprite *sprite);

#endif

