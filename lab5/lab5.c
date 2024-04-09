#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "video.h"
#include "keyboard.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/*
https://web.fe.up.pt/~pfs/aulas/lcom2223/labs/lab5/lab5_extra.html#reg86

types:
db : 1 bye
dw : 2 bytes
dd : 4 bytes

for reasons of alingment:
#pragma pack(1) -> no padding
#pragma options align=reset -> reset to default
*/
// set AH = 0x4F (distinguish vbe function from vga bios)
// AL -> specifies the function
// if return OK, AH = 0x00
// ah = 0x01 -> func call failed
// ah = 0x02 -> func not supported in current HW config
// ah = 0x03 -> invalid function in current video mode

// 0x00 func -> return VBE controller info
// args: real-mode addr of a buffer to be filled controller info
    // ret: list of video mode numbers supported by the controller

// real mode addr:
// base addr of a seg: 16 bit, to be left shifted by 4 bits
// creates a 20 bit addr
// offset: 16 bit addr, to be added to the base addr

// 0x03 : minix text mode
// use 0x00 func (int 0x10 bios) 
// set AH = 0x00
// set AL = 0x03 // smth else to different modes?

int (video_test_init)(uint16_t mode, uint8_t delay){
    if (vbe_set_mode(mode)!=0) return 1;
    sleep(delay);
    // resets video controller to text mode
    // calls func AL=0x00 (set video mode) of BIOS video service int 0x10
    return vg_exit();
}

// 0x01 func -> return vbe mode info ; 
    //get vram physical addr from video controller
    // get vram size
    // args: mode passed on CX, 
        // es:di real-mode (linear physical) addr of a buffer to be filled w params of specified mode
        // horizontal res and vert res
        // bits per pixel
// map graphics mode vram in the process address space
int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                           uint32_t color){
    
    if (map_vram(mode)!=0) return 1;
    if (vbe_set_mode(mode)!=0) return 1;
    if (vg_draw_rectangle(x, y, width, height, color)!=0) return 1;
    if (wait_ESC()!=0) return 1;

    return vg_exit();
}
//--------------------------------
int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step){
    // if no_rectangles is 3, then the pattern will be a matrix of 3 by 3 rectangles
// * The color of each rectangle depends on its coordinates in the rectangle
//  *  matrix (row, col).
//  * 
//  * If the color model is indexed then the (index of the) color of the rectangle 
//  *  with coordinates (row, col) is given by:
//  * index(row, col) = (first + (row * no_rectangles + col) * step) % (1 << BitsPerPixel)
//  * 
//   * If the color model is direct then the components of the rectangle with 
//  *  coordinates (row, col) are given by:
//  *   R(row, col) = (R(first) + col * step) % (1 << RedScreeMask) 
//  * G(row, col) = (G(first) + row * step) % (1 << GreenScreeMask)
//  *  B(row, col) = (B(first) + (col + row) * step) % (1 << BlueScreeMask)	
//  * 
//  * Where *MaskSize are the values of the members with that name of the VBE
//  *  ModeInfoBlock struct.
//  * 
//  * Returns upon release of the ESC key
//  * @param first Color to be used in the first rectangle (the rectangle at the
//  * 				 top-left corner of the screen). 
//  * @param step	 Difference between the values of the R/G component in adjacent
//  * 				 	rectangles in the same row/column. For the B
//  * 					component check the detailed description.

    return 0;
}

int (video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
    // writing to vram in video mode 0x105
    // read_xpm() to parse the input XPM
    return 0;
}

int (video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                      int16_t speed, uint8_t fr_rate){
    // video mode 0x105
    // speed positive: displacement in pixels between consecutive frames
    // speed negative: number of frames required for a 1 pixel displacement
    // fr rate: frames per second
    return 0;
}

int(video_test_controller)() {
    // Retrieves, parses and displays VBE controller information (VBE function 0x0)
 
//  * Retrieves the VBE controller information (VBE function 0x0), 
    //parses it
//  vg_display_vbe_contr_info()
//  already provided with
//  *  the LCF, to display that information on the console.
 
  return 1;
}

