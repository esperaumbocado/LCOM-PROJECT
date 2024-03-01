#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  int counter=0;
  while(val<<=1){
    counter++;
  }
  if (val!=0){
    *lsb=counter;
    return 0;
  }else{
    return 1;
  }
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  int counter=0;
  while(val>>=1){
    counter++;
  }
  if (val!=0){
    *msb=counter;
    return 0;
  }else{
    return 1;
  }
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t val;
  int return_value = sys_inb(port, &val);
  *value = (uint8_t)val;
  return return_value;
}
