#include <lcom/lcf.h>

#include <stdint.h>
#include "mouse.h"

int mouse_hook_id =12; //?

int (mouse_subscribe_int)(uint8_t *bit_no) {
 
  if (bit_no==NULL) return 1;
  *bit_no = BIT(mouse_hook_id); 

  if (sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK)
    return 1;
  // it enables the corresponding interrupt
  // (so we dont need to call sys_irqenable())
  // IRQ_REENABLE (int, the policy) so that the generic
  // interrupt handler will acknowledge the interrupt,
  // output the EOI command to the PIC
  // enabling further interrupts on the corresponding IRQ line
  return 0;
}

int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&mouse_hook_id)!=OK)
    return 1; 
  return 0;
}

void (mouse_ih)(){
    // Reads the st reg 0x64
    // read the output buffer 0x60
    // if there was some error, 
    // the byte read from the OB should be discarded
 
}
