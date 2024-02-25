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
  // Reads the input timer configuration (status) via read-back command.
  // st	- Address of memory position to be filled with the timer config

  if (st == NULL) return 1;  // Verificar se o ponteiro não é NULL
  if (timer < 0 || timer > 2) return 1;  // Verificar se o timer é válido

  uint8_t rb_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); 
  // TIMER_RB_CMD - read-back command (BIT(7) | BIT(6))
  // TIMER_RB_COUNT_ - read-back count BIT(5), o count não é lido, apenas o status
  // TIMER_RB_SEL(timer) - select the timer to read BIT((n) + 1)

  if (sys_outb(TIMER_CTRL, rb_command) != OK) return 1;  // envia o RBC p/ o controlador
  // TIMER_CTRL 0x43 - Control register

  if (util_sys_inb(TIMER_0 + timer, st) != OK) return 1;  // lê o status do timer

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val conf;

  switch (field) {
    case tsf_all: //configuration/status
      conf.byte = st; //status
      break;
    case tsf_initial: //timer initialization mode
      // confirmar

      conf.in_mode = (st & (TIMER_LSB | TIMER_MSB)) >> 4; //inicialization mode
      break;

    case tsf_mode: //timer counting mode
      // confirmar

      conf.count_mode = (st & TIMER_SQR_WAVE) >> 1; //counting mode 0..5
      if (conf.count_mode > 5) conf.count_mode &= 0x3; //?

      break;
    case tsf_base: // timer counting base
      conf.bcd = st & TIMER_BCD; //counting base, true if BCD
      break;
    default:
      return 1;
  }

  if (timer_print_config(timer, field, conf) != 0) return 1;
  return 0;
}
