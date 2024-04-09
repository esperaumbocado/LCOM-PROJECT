#include <lcom/lcf.h>

#include <stdint.h>
#include "video.h"

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped 
                        frame-buffer address*/
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel; /* Number of VRAM bytes per pixel */

// static global -> not visible in other files
// akin static members in c++
int (vbe_set_mode)(uint16_t mode){
    //configure video card to graphics mode
    //use VESA VBE
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86_t)); // clear the structure
    reg86.intno = 0x10; // bios video services INT number
    reg86.ax = 0x4F02; // 4F - ah VBE call; 02 -  al func, set video mode
    reg86.bx = mode | BIT(14); // linear frame buffer model
        // linear facilitates access to vram  
    // vram stores the pixels values
        
    // sys_int86 : temp switches (emulates) to 16-bit real mode to call bios services
    // executes the softw interrupt inst INT
    // processor jumps to the corresponding handler,
    // which must be configured at boot time
    // switch back to 32-bit protected mode
    if (sys_int86(&reg86)!=0) {
        printf("sys_int86 failed\n");
        return 1;
    }
    printf("Mode %x successfully set!\n", mode);
    return 0;
}

int (map_vram)(uint16_t mode){
    // map the video memory to the process addr space
    vbe_mode_info_t vbe_mode_info;
    
    // this func uses 0x01, return vbe mode info
    if (vbe_get_mode_info(mode, &vbe_mode_info) !=0) 
        return 1; 
    
    phys_bytes vram_base_phys = vbe_mode_info.PhysBasePtr;
    h_res = vbe_mode_info.XResolution;
    v_res = vbe_mode_info.YResolution;
    bits_per_pixel = vbe_mode_info.BitsPerPixel;
    bytes_per_pixel = (bits_per_pixel+7)/8;

    printf("bits per pixel: %d\n", bits_per_pixel);
    printf("bytes per pixel: %d\n", bytes_per_pixel);

    unsigned int vram_size = h_res * v_res * bytes_per_pixel; // in bytes
    
    struct minix_mem_range mr;
    mr.mr_base = vram_base_phys;
    mr.mr_limit = mr.mr_base + vram_size;

    int r;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    // endpoint_t proc_ep
    // 1st arg: process that will have access to the memory

    if (video_mem == MAP_FAILED) return 1;
    
    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    if (x >= h_res || y >= v_res) return 1;
    unsigned int pos = (y * h_res + x) * bytes_per_pixel; // in bytes
    for (unsigned int i = 0; i < bytes_per_pixel; i++){
        video_mem[pos+i] = (color >> (i*8) & 0xFF); 
        printf("color byte %x\n", color >> (i*8) & 0xFF);
        printf("video_mem byte %x\n", video_mem[pos+i]);
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for (int j = x; j < x+len; j++){
        printf("drawing pixel %d\n", j);
        if (vg_draw_pixel(j, y, color)!=0) return 1;
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, 
            uint16_t height, uint32_t color){

    for (int i = y; i < y+height; i++){
        printf("drawing line %d\n", i);
        if (vg_draw_hline(x, i, width, color)!=0) return 1;
    }
    return 0;
}

