#include <lcom/lcf.h>
#include "graphics.h"
#include <math.h>



//Muda o minix para o modo gráfico
int (set_graphic_mode)(uint16_t submode) {
    reg86_t reg86;
    
    // Seta todos os bits em 0
    memset(&reg86, 0, sizeof(reg86));

    // Isso é sempre 0x10
    reg86.intno = 0x10;

    // Parte mais significativa de AX (When invoking a VBE function, the AH register must be set to 0x4F)
    reg86.ah = 0x4F;

    // Parte menos significativa de AX. 0x02 no caso de modo gráfico
    reg86.al = 0x02;

    // reg86.ax = 0x4F02; é equivalente às duas anteriores

    //Seta o submode para ter memória linear (que é o único tipo de memória que vamos usar)
    reg86.bx = submode | BIT(14);

    // se houver algum erro, abortar a função
    if (sys_int86(&reg86) != 0) {
        printf("Set graphic mode failed\n");
        return 1;
    }
    return 0;
}

int (set_frame_buffer)(uint16_t mode) {

    // reseta o mode_info para todos os bits em 0
    memset(&mode_info, 0, sizeof(mode_info));
    if(vbe_get_mode_info(mode, &mode_info)) return 1;

    //Cálculo do tamanho do frame buffer em bytes
    unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

    //Preenchimento dos endereços físicos
    struct minix_mem_range physic_adresses;

    //Início físico do buffer
    physic_adresses.mr_base = mode_info.PhysBasePtr;

    //Fim físico do buffer
    physic_adresses.mr_limit = physic_adresses.mr_base + frame_size;

    //Alocação física da memória necessária para o frame buffer
    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_adresses)) {
        printf("Physical memory allocation error\n");
        return 1;
    }

    //Alocação virtual da memória necessária para o frame buffer
    frame_buffer = vm_map_phys(SELF, (void*) physic_adresses.mr_base, frame_size);
    if(frame_buffer == NULL) {
        printf("Virtual memory allocation error\n");
        return 1;
    }

    return 0;
}


// Atualização da cor de um pixel
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint16_t color) {

    //As coordenadas tem de ser válidas
    if(x > mode_info.XResolution || y > mode_info.YResolution) return 1;

    //Cálculo dos Bytes per pixel da cor escolhida. Arredondamento por excesso
    unsigned BytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

    // Índice (em bytes) da zona do píxel a colorir
    unsigned int index = (mode_info.XResolution * y + x) * BytesPerPixel;

    // A partir da zona frame_buffer[index], copia @BytesPerPixel bytes da @color
    if (memcpy(&frame_buffer[index], &color, BytesPerPixel) == NULL) return 1;

    return 0;
}

// Desenha uma linha horizontal
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned i = 0 ; i < len ; i++)   
    if (vg_draw_pixel(x+i, y, color) != 0) return 1;

  return 0;
}


// Desenha um rectângulo
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for(unsigned i = 0; i < height ; i++)
    if (vg_draw_hline(x, y+i, width, color) != 0) {
      vg_exit();
      return 1;
    }

  return 0;
}

int normalize_color(uint16_t color, uint32_t *new_color) {
    if (mode_info.BitsPerPixel == 32) {
        *new_color = color;
    } else {
        *new_color = color & (BIT(mode_info.BitsPerPixel) - 1);
    }
    return 0;
}
