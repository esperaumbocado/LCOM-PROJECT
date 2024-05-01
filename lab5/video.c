#include <lcom/lcf.h>
#include "video.h"

int set_graphic_mode(uint16_t mode){

    reg86_t reg86;
    memset(&reg86,0,sizeof(reg86));
    reg86.intno = 0x10;
    reg86.ah = 0x4F;
    reg86.al = 0x02;
    reg86.bx = mode | BIT(14);
    if (sys_int86(&reg86)!=0){
        printf("Error setting graphic mode\n");
        return 1;
    }

    return 0;
}



int set_frame_buffer(uint16_t mode){
    memset(&mode_info,0,sizeof(mode_info));
    if(vbe_get_mode_info(mode,&mode_info)){
        printf("Error calling vbe_get_mode_info\n");
        return 1;
    }

    unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7)/8;
    unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

    struct minix_mem_range physic_adresses;
    physic_adresses.mr_base = mode_info.PhysBasePtr;
    physic_adresses.mr_limit = physic_adresses.mr_base + frame_size;

    if(sys_privctl(SELF,SYS_PRIV_ADD_MEM,&physic_adresses)!=0){
        printf("Error allocating memory for minix_mem_range\n");
        return 1;
    }

    frame_buffer = vm_map_phys(SELF, (void*) physic_adrreses.mr_base,frame_size);
    if (frame_buffer==NULL){
        printf("Error allocating virtual memory\n");
        return 1;
    }

    return 0;
}