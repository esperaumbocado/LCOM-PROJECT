#include <stdint.h>
#include <lcom/lcf.h>

int counter_sysinb=0;

int util_sys_inb(int port, uint8_t *out){
  uint32_t value;
  if (sys_inb(port,&value)!=0){
    printf("Error in sys_inb inside util_sys_inb\n");
    return 1;
  }
  *out = value;
  counter_sysinb++;
  return 0;
}

