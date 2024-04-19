#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t config;
  if (timer_get_conf(timer,&config)!=0){
    printf("Error getting config in timer_set_frequency\n");
    return 1;
  }

  config = (config & 0x0F) | TIMER_LSB_MSB;

  uint32_t initialValue = TIMER_FREQ / freq;
  
  switch (timer){
    case 0:
      config |= TIMER_SEL0;
      break;
    case 1:
      config |= TIMER_SEL1;
      break;
    case 2:
      config |= TIMER_SEL2;
      break;
    default:
      printf("Timer not identified in timer_set_frequency\n");
      return 1;
  }


  uint8_t msb, lsb;
  if (util_get_LSB(initialValue,&lsb)!=0){
    printf("Error getting LSB\n");
    return 1;
  }
  if (util_get_MSB(initialValue,&msb)!=0){
    printf("Error gettings MSB\n");
    return 1;
  }
  if (sys_outb(TIMER_CTRL,config)!=0){
    printf("Error letting control now of new configuration\n");
    return 1;
  }
  if (sys_outb(TIMER_0 + timer, lsb)!=0){
    printf("Error sending LSB");
    return 1;
  }
  if (sys_outb(TIMER_0 + timer, msb)!=0){
    printf("Error sending MSB");
    return 1;
  }
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no==NULL){
    printf("Invalid pointer in timer_subscribe_int\n");
    return 1;
  }
  *bit_no = BIT(hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id)!=0){
    printf("Error subscribing timer\n");
    return 1;
  }
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id)!=0){
    printf("Error unsubscribing timer\n");
    return 1;
  }
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st==NULL | timer < 0 | timer > 2){
    printf("Error in timer_get_conf params\n");
    return 1;
  }

  uint8_t READBACK_COMMAND = (TIMER_RB_SEL(timer) | TIMER_RB_COUNT_ | TIMER_RB_CMD);
  if (sys_outb(TIMER_CTRL,READBACK_COMMAND)!=0){
    printf("Error letting driver know of status\n");
    return 1;
  }

  if (util_sys_inb(TIMER_0 + timer,st)!=0){
    printf("Error getting status from config\n");
    return 1;
  }

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  union timer_status_field_val data;

  switch (field){
    case tsf_all:
      data.byte = st;
      break;
    case tsf_initial:
      if ((st & BIT(4)) && (st & BIT(5))){
        data.in_mode = MSB_after_LSB;
      }else if (st & BIT(4)){
        data.in_mode = LSB_only;
      }else if (st & BIT(5)){
        data.in_mode = MSB_only;
      }else{
        data.in_mode = INVAL_val;
      }
      break;
    case tsf_mode:
      if (!(st & BIT(1)) && !(st & BIT(2)) && !(st & BIT(3))){
        data.count_mode = 0;
      }else if (!(st & BIT(3)) && !(st & BIT(2)) && (st & BIT(1))){
        data.count_mode = 1;
      }else if ((st & BIT(2) && !(st & BIT(1)))){
        data.count_mode = 2;
      }else if ((st & BIT(2) && (st & BIT(1)))){
        data.count_mode = 3;
      }else if ((st & BIT(3)) && !(st & BIT(2)) && !(st & BIT(1))){
        data.count_mode = 4;
      }else if ((st & BIT(3)) && !(st & BIT(2)) && (st & BIT(1))){
        data.count_mode = 5;
      }
      break;
    case tsf_base:
      if (st & BIT(0)){
        data.bcd = true;
      }else{
        data.bcd = false;
      }
      break;
    default:
      printf("Error in field param\n");
      return 1;
  }

  timer_print_config(timer,field,data);
  return 0;
}
