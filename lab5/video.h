#include <lcom/lcf.h>

#include <stdint.h>

vbe_mode_info_t mode_info;
uint8_t* frame_buffer;

int set_graphic_mode(uint16_t mode);
int set_frame_buffer(uint16_t mode);

