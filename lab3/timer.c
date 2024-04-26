#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if( freq > TIMER_FREQ || freq < 19) return 1;

  uint8_t controlWord;

  if( timer_get_conf(timer, &controlWord) != 0) return 1;

  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB;

  //Cálculo do valor inicial do contador e partes mais e menos significativas
  uint32_t initialValue = TIMER_FREQ / freq;
  uint8_t MSB, LSB;
  util_get_LSB(initialValue, &LSB);
  util_get_MSB(initialValue, &MSB);

  // Atualiza a controlWord pelo timer escolhido
  // @selectedTimer terá a porta do timer escolhido (0x40, 0x41, 0x42)
  uint8_t selectedTimer;
  switch(timer) {
    case 0:
      controlWord |= TIMER_SEL0;
      selectedTimer = TIMER_0;
      break;
    case 1:
      controlWord |= TIMER_SEL1;
      selectedTimer = TIMER_0;
      break;
    case 2:
      controlWord |= TIMER_SEL2;
      selectedTimer = TIMER_1;
      break;
    default: return 1;
  }

  // Avisa o i8254 que o timer vai ser configurado
  if (sys_outb(TIMER_CTRL, controlWord) != 0) return 1;

  // Injeta o valor inicial do contador no registro
  if (sys_outb(selectedTimer, LSB) != 0) return 1;
  if (sys_outb(selectedTimer, MSB) != 0) return 1;
  
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = BIT(hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id) != 0) return 1;
  
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if( st == NULL || timer > 2 || timer < 0) return 1;
  uint8_t RBC = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if(sys_outb(TIMER_CTRL, RBC) != 0) return 1;
  if(util_sys_inb(TIMER_0 + timer, st)) return 1;

  return 1;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val data;

  switch (field) {

    case tsf_all: 
      data.byte = st; 
      break;

    case tsf_initial:                                       
      st = (st >> 4) & 0x03;

      if (st == 1) data.in_mode = LSB_only;
      else if (st == 2) data.in_mode = MSB_only;
      else if (st == 3) data.in_mode = MSB_after_LSB;
      else data.in_mode = INVAL_val;
      
      break;

    case tsf_mode:
      st = (st >> 1) & 0x07;

      if(st == 6) data.count_mode = 2;
      else if(st == 7) data.count_mode = 3;
      else data.count_mode = st;

      break;
    
    case tsf_base:
      data.bcd = st & TIMER_BCD;
      break;        

    default:
      return 1;
  }

  if (timer_print_config(timer, field, data) != 0) return 1;
  
  return 0;
}
