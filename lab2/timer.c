#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
    //Read-back command
    uint8_t rb=0;
    rb += BIT(0); // RESERVED FIELD
    
    //Select counter
    if (timer==0){
      rb |= TIMER_SEL0;
    }else if (timer==1){
      rb |= TIMER_SEL1;
    }else if (timer==2){
      rb |= TIMER_SEL2;
    }
   rb |= TIMER_RB_STATUS_; // Read programmed mode
   rb |= TIMER_RB_COUNT_; // Read count value 
   rb |= TIMER_RB_CMD; //Read back command

   if(sys_outb(TIMER_CTRL,rb)!=0){
    return 1;
   }
   
   switch(timer){
    case 0:
      if(util_sys_inb(TIMER_0,st)){
        return 1;
      }
      break;
    case 1:
      if(util_sys_inb(TIMER_1,st)){
        return 1;
      }
      break;
    case 2:
      if(util_sys_inb(TIMER_2,st)){
        return 1;
      }
      break;
   }
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {


  union timer_status_field_val val;

  switch (field){

    case tsf_base:
      if (st&TIMER_BCD){
        val.bcd = true;
      }else{
        val.bcd = false;
      }
      break;

    case tsf_mode:
      if (st&TIMER_SQR_WAVE){
        val.count_mode = 3;
      }else if (st&TIMER_RATE_GEN){
        val.count_mode = 2;
      }else {
        val.count_mode = st;
      }
      break;

    case tsf_initial:
      if (st&TIMER_LSB){
        val.in_mode = LSB_only;
      }else if (st&TIMER_MSB){
        val.in_mode = MSB_only;
      }else if (st&TIMER_LSB_MSB){
        val.in_mode = MSB_after_LSB;
      }
      break;

    case tsf_all:
      val.byte = st;
      break;
      
    default:
      return 1;
      break;
  }

  if(timer_print_config(timer,field,val)!=0) return 1;
  return 0;
}
