#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (val & 0xFF);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = val >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t call = 0;

  if (sys_inb(port,&call)!=0){
    printf("Error in sys_inb\n");
    return 1;
  }

  *value = call & 0xFF;

  return 0;
}
