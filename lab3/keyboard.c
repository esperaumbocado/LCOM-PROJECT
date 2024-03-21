#include "keyboard.h"


int kbd_hook_id = 2;
uint8_t scan_code;


int (keyboard_subscribe_int)(uint8_t *bit_no){
  if (bit_no == NULL) return 1;
  *bit_no = BIT(kbd_hook_id);
  if (sys_irqsetpolicy(KEYBOARD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&kbd_hook_id)!=0) return 1;
  return 0;
}

int (keyboard_unsubscribe_int)(){
  if (sys_irqrmpolicy(&kbd_hook_id)!=0)return 1;
  return 0;
}

void (kbc_ih)(){
  if(KBC_output(&scan_code)!=0){printf("Error");};
}

