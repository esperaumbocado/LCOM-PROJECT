#include "KBC.h"


int (KBC_output)(uint8_t *output){

  uint8_t status;

  for (int attempt=0;attempt<10;attempt++){
    if(util_sys_inb(KBC_STATUS_REG, &status)!=0) {
      printf("Error: KBC not avaiable");
      return 1;
    }

    if ((status & BIT(0))!=0){ // OUTPUT Buffer cheio
      if(util_sys_inb(KBC_OUTPUT_REG,output)!=0) return 1;

      if((status & BIT(7))!=0){
        printf("Parity ERROR\n");
      }
      if((status & BIT(6))!=0){
        printf("Timeout ERROR\n");
      }
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
  }
  return 1;
} 

int (write_KBC)(uint8_t port, uint8_t in){
  uint8_t status;
  for (int i=0;i<10;i++){
    if (util_sys_inb(KBC_STATUS_REG,&status)!=0) return 1;

    if((status & BIT(1))==0){
      if (sys_outb(port,in)!=0) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
  }
  return 1;
}

