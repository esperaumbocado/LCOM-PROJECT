#include "view.h"
#include "../model/sprite.h"


int drawSpriteXPM(Sprite *sprite, int x, int y) {
    if (sprite == NULL) return 1;

    uint16_t width = sprite->width;
    uint16_t height = sprite->height;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            uint32_t color = sprite->colors[col + row * width];

            if (color == TRANSPARENT) {
                continue;
            }

            if (draw_pixel(x + col, y + row, color, drawing_frame_buffer) != 0) {
                return 1;
            }
        }
    }
    
    return 0;
}
