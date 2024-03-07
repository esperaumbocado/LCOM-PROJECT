#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <math.h>

#include <stdint.h>

#include "i8254.h"


int hook_id = 0;
int counter = 0;


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint32_t min = (uint32_t)(ceil((uint16_t)(TIMER_FREQ/USHRT_MAX)));
  uint32_t max = TIMER_FREQ;
  uint16_t div = TIMER_FREQ/freq;
  printf("div:%x\n",div);
  if (freq<min||freq>max){return 1;}
  uint8_t msb,lsb,control;
  util_get_MSB(div,&msb);
  printf("msb:%x\n",msb);
  util_get_LSB(div,&lsb);
  printf("lsb;%x\n",lsb);
  if(timer_get_conf(timer,&control)!=0){return 1;}
  control = control&(BIT(0)|BIT(1)|BIT(2)|BIT(3));
  control |= (TIMER_RB_SEL(timer) | TIMER_LSB_MSB)&(BIT(7)|BIT(6)|BIT(5)|BIT(4));
  printf("0x%x",control);
  if(sys_outb(TIMER_CTRL,control)!=0){return 1;}
  if(sys_outb(TIMER_0+timer,lsb)!=0){return 1;}
  if(sys_outb(TIMER_0+timer,msb)!=0){return 1;}
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  if (bit_no == NULL) return 1;
  *bit_no = BIT(hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE, &hook_id)!=0)return 1;
  else return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id)!=0) return 1;
  else return 0;
}

void (timer_int_handler)() {
  counter++;
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
   rb |= TIMER_RB_COUNT_; // Read count value 
   rb |= TIMER_RB_CMD; //Read back command
   rb |= TIMER_RB_SEL(timer);
   timer_adress = TIMER_0 + timer;
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
