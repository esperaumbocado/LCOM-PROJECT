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
    uint8_t rb=0x00;
    if (st==NULL){
      return 1;
    }
    if (timer<0 | timer>2){
      return 1;
    }

    int timer_adress;
    //Select counter
    if (timer==0){
      timer_adress = TIMER_0;
      rb |= TIMER_RB_SEL(0);
    }else if (timer==1){
      timer_adress = TIMER_1;
      rb |= TIMER_RB_SEL(1);
    }else if (timer==2){
      timer_adress = TIMER_2;
      rb |= TIMER_RB_SEL(2);
    }
  
   rb |= TIMER_RB_COUNT_; // Read count value 
   rb |= TIMER_RB_CMD; //Read back command

   if(sys_outb(TIMER_CTRL,rb)!=0){
    return 1;
   }
   
   uint r_value;


  r_value = util_sys_inb(timer_adress,st);
  return r_value;
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
      val.count_mode= ((st&(BIT(1)|BIT(2)|BIT(3)))>>1);
      break;

    case tsf_initial:
      val.in_mode = ((st&TIMER_LSB_MSB)>>4);
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
