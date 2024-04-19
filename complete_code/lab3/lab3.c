#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include "keyboard.h"
#include "KBC.c"
#include "timer.c"


#include <stdbool.h>
#include <stdint.h>

extern uint8_t scan_code;
extern int counter_sysinb;
extern int counter_time;
extern uint32_t counter_KBC;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  /*
  int ipc_status, r;
  uint8_t keyboard_irq_set;
  message msg;
  uint8_t scan_codes[2];
  bool scan_double = false;

if (keyboard_subscribe_int(&keyboard_irq_set)!=0) return 1;
  while(scan_code != 0x81){
    if ((r = driver_receive(ANY,&msg,&ipc_status))!=0){
      printf("driver_receive failed with %d",r);
      continue;
    }
    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & keyboard_irq_set){
              kbd_ih();
              if (scan_code == 0xE0){ // Check if there will be one or two bytes.
                scan_codes[0]= scan_code;
                scan_double=true; //Added flag to wait for the next byte
              }else if (scan_code & BIT(7)){ // Check if it is a Break CODE
                if (scan_double==true){
                  scan_codes[1]=scan_code;
                  kbd_print_scancode(false,2,scan_codes);
                  scan_double = false;
                }else{ //Only one byte, no second one needed
                  scan_codes[0] = scan_code;
                  kbd_print_scancode(false,1,scan_codes);
                }
              }else{ //It is a make code
                if (scan_double==true){ // Byte one was received, process byte two
                  scan_codes[1]=scan_code;
                  kbd_print_scancode(true,2,scan_codes);
                  scan_double = false;
                }else{ //Only one byte, no second one needed
                  scan_codes[0]=scan_code;
                  kbd_print_scancode(true,1,scan_codes);
                }
              }
            }
          break;
        default:
          break;
      }
    }else{
      // Interrupt not recognized, do nothing
      }
    }
    kbd_print_no_sysinb(counter_sysinb);
    keyboard_unsubscribe_int();
    */
    // TRYING TO MAKE CLEANER CODE
    // TRYING TO MAKE CLEANER CODE
    // TRYING TO MAKE CLEANER CODE
    // TRYING TO MAKE CLEANER CODE
    // TRYING TO MAKE CLEANER CODE


  int ipc_status;
    uint8_t irq_set;
    message msg;

    if(keyboard_subscribe_int(&irq_set) != 0) return 1;

    while(scan_code != 0x81){

        if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
            printf("Error");
            continue;
        }

        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)){
                 case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        kbc_ih(); // aumenta o contador interno
                        kbd_print_scancode(!(scan_code & BIT(7)), scan_code == 0xE0 ? 2 : 1, &scan_code);
                    }
            }
        }
    }

  if (keyboard_unsubscribe_int() != 0) return 1;
  if (kbd_print_no_sysinb(counter_sysinb) != 0) return 1;

  return 0;
}

int(kbd_test_poll)() {

  while(scan_code!=0x81){
    if(KBC_output(&scan_code)==0){
      kbd_print_scancode(!(scan_code & BIT(7)), scan_code == 0xE0 ? 2 : 1, &scan_code);
    }
  }


  uint8_t commandByte;
  if (write_KBC(0x64, 0x20) != 0) return 1;          
  if (KBC_output(&commandByte) != 0) return 1; 

  commandByte |= BIT(0);  

  if (write_KBC(0X64, 0x60) != 0) return 1;    
  if (write_KBC(0x60, commandByte) != 0) return 1;

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status;
    uint8_t irq_set_KBC,irq_set_timer;
    message msg;
    int time=0;

    if(keyboard_subscribe_int(&irq_set_KBC) != 0) return 1;
    if(timer_subscribe_int(&irq_set_timer)!=0) return 1;

    while((scan_code != 0x81) && (time < n)){

        if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
            printf("Error");
            continue;
        }

        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)){
                 case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_KBC) {
                        kbc_ih(); // aumenta o contador interno
                        kbd_print_scancode(!(scan_code & BIT(7)), scan_code == 0xE0 ? 2 : 1, &scan_code);
                        time = 0;
                        counter_time = 0;
                    }
                    if (msg.m_notify.interrupts & irq_set_timer) {
                        timer_int_handler();
                        if (counter_time % 60 == 0) time++;
                    }
            }
        }
    }

  if (timer_unsubscribe_int() != 0) return 1;
  if (keyboard_unsubscribe_int() != 0) return 1;
  if (kbd_print_no_sysinb(counter_sysinb) != 0) return 1;
  return 1;
}
