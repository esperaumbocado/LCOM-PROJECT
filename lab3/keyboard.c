#include <lcom/lcf.h>

#include <stdint.h>
#include "keyboard.h"

int hook_id =1;
uint8_t data;
int (keyboard_subscribe_int)(uint8_t *bit_no) {
 
  if (bit_no==NULL) return 1;
  *bit_no = BIT(hook_id); 

  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK)
    return 1;
  // it enables the corresponding interrupt
  // (so we dont need to call sys_irqenable())
  // IRQ_REENABLE (int, the policy) so that the generic
  // interrupt handler will acknowledge the interrupt,
  // output the EOI command to the PIC
  // enabling further interrupts on the corresponding IRQ line
  return 0;
}

int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id)!=OK)
    return 1; 
  return 0;
}

void (kbc_ih)(){
    //read the status register and check if there was some communications error;
    uint8_t st;
    uint8_t tries=10;

    while (tries>0){
      if (util_sys_inb(0x64 , &st) != OK) // lê o status register
      {
        printf("reading status register 0x64 was not ok\n");  
        return;
      } 

      if (st & (BIT(7))) {
        printf("Parity Error.\n");
        return;
      }
      
      if (st & (BIT(6))) {
        printf("Timeout Error.\n");
        return;
      }

      if ((st & BIT(0)) != 0){ // if the output buffer is full, we can read the scancode 
        //read the scancode byte from the output buffer;
        // data é uma variável global
        if (util_sys_inb(0x60 , &data) != OK){
          printf("reading data from output buffer 0x60 was not ok");  
          return;
        }
        return; // success
      }
      tickdelay(micros_to_ticks(DELAY_US));  
      tries--;
    }
      
}

int (kbc_activate_interrupts)(){
  return 0;
}


