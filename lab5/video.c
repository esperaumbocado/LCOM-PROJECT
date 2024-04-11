#include <lcom/lcf.h>

#include <stdint.h>
#include "video.h"

// MODE 
// 0x105 -> indexed
// 0x115 -> direct color

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped 
                        frame-buffer address*/
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel; /* Number of VRAM bytes per pixel */
static unsigned mem_model; /* Memory model used in the graphics mode */
static unsigned red_mask_size; /* Size of the red mask in bits */
static unsigned green_mask_size; /* Size of the green mask in bits */
static unsigned blue_mask_size; /* Size of the blue mask in bits */
static unsigned red_field_position; /* Bit position of the lsb of the red mask */
static unsigned green_field_position; /* Bit position of the lsb of the green mask */
static unsigned blue_field_position; /* Bit position of the lsb of the blue mask */

// static global -> not visible in other files
// akin static members in c++

int (map_vram)(uint16_t mode){
    vbe_mode_info_t vbe_mode_info;

    memset(&vbe_mode_info, 0, sizeof(vbe_mode_info)); 

    // 0x01 func -> return vbe mode info ; 
    //get vram physical addr from video controller
    // args: mode passed on CX, 
        // es:di real-mode (linear physical) addr of a 
        // buffer to be filled w params of specified mode
    if (vbe_get_mode_info(mode, &vbe_mode_info) !=0) 
        return 1; 
    
    phys_bytes vram_base_phys = vbe_mode_info.PhysBasePtr;

    h_res = vbe_mode_info.XResolution;
    v_res = vbe_mode_info.YResolution;
    
    printf("hres: %d\n", h_res);
    printf("vres: %d\n", v_res);

    bits_per_pixel = vbe_mode_info.BitsPerPixel;
    bytes_per_pixel = (bits_per_pixel+7)/8;

    printf("bits per pixel: %d\n", bits_per_pixel);
    printf("bytes per pixel: %d\n", bytes_per_pixel);

    unsigned int vram_size = h_res * v_res * bytes_per_pixel; // in bytes
    
    mem_model = vbe_mode_info.MemoryModel;

    red_mask_size = vbe_mode_info.RedMaskSize;
    green_mask_size = vbe_mode_info.GreenMaskSize;
    blue_mask_size = vbe_mode_info.BlueMaskSize;

    red_field_position = vbe_mode_info.RedFieldPosition;
    green_field_position = vbe_mode_info.GreenFieldPosition;
    blue_field_position = vbe_mode_info.BlueFieldPosition;

    struct minix_mem_range mr;
    mr.mr_base = vram_base_phys;
    mr.mr_limit = mr.mr_base + vram_size;

    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)!=0)
        panic("sys_privctl (ADD_MEM) failed\n");
    // 1st arg: process that will have access to the memory

    // map graphics mode vram in the process address space
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    // endpoint_t proc_ep

    if (video_mem == MAP_FAILED) return 1;
    
    return 0;
}

int (vbe_set_mode)(uint16_t mode){
    //configure video card to graphics mode
    //use VESA VBE
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86_t)); // clear the structure
    reg86.intno = 0x10; // bios video services INT number
    reg86.ax = 0x4F02; // AH = 4F - VBE call; 
                       // AL = 02 - func, set video mode
    reg86.bx = mode | BIT(14); 
    // bit 15 cleared - so display memory is cleared after 
        //switching to the desired graphics mode.
    // bit 14 -  linear frame buffer model -facilitates access to vram  
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

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    if (x >= h_res || y >= v_res) return 1;
    unsigned int pos = (y * h_res + x) * bytes_per_pixel; // in bytes
    for (unsigned int i = 0; i < bytes_per_pixel; i++){
        video_mem[pos+i] = (unsigned char)(color >> (i*8) & 0x0000FF); 
        // printf("color byte %02x\n", color >> (i*8) & 0xFF);
        // printf("video_mem byte %02x\n", (unsigned char) video_mem[pos+i]);
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for (int j = x; j < x+len; j++){
        // printf("drawing pixel %d\n", j);
        if (vg_draw_pixel(j, y, color)!=0) return 1;
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, 
            uint16_t height, uint32_t color){
    
    printf("color %x\n", color);
    if (bits_per_pixel != 32){
        color = color & ((1 << bits_per_pixel) - 1);
        printf("bits per pixel %d\n", bits_per_pixel);
        printf("color %x\n", color);
    }

    for (int i = y; i < y+height; i++){
        // printf("drawing line %d\n", i);
        if (vg_draw_hline(x, i, width, color)!=0) return 1;
    }
    return 0;
}

int (vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step){
    
    uint32_t color = first;
    uint16_t width = h_res/no_rectangles;
    uint16_t height = v_res/no_rectangles;

    printf("width: %d\n", width);
    printf("height: %d\n", height);

    for (int i = 0; i < no_rectangles; i++){
        for (int j = 0; j < no_rectangles; j++){
            
            if (mem_model == 0x04){ // indexed 
                color = (first + (i * no_rectangles + j) * step) % (1 << bits_per_pixel);
            } else if (mem_model == 0x06){ // direct color

                // ((1 << red_mask_size) - 1) extrai os bits mais à direita
                uint32_t red_first = (first >> red_field_position) & ((1 << red_mask_size) - 1);
                uint32_t red = ((red_first >> red_field_position) + j * step) % (1 << red_mask_size);
                
                uint32_t green_first = (first >> green_field_position) & ((1 << green_mask_size) - 1);
                uint32_t green = ((green_first >> green_field_position) + i * step) % (1 << green_mask_size);
                
                uint32_t blue_first = (first >> blue_field_position) & ((1 << blue_mask_size) - 1);
                uint32_t blue = ((blue_first >> blue_field_position) + (i + j) * step) % (1 << blue_mask_size);

                color = (red << red_field_position) 
                        | (green << green_field_position) 
                        | (blue << blue_field_position);
            }
            
            vg_draw_rectangle(j*width, i*height, width, height, color);
        }
    }

    return 0;
}

