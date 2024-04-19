#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "utils.c"

extern int counter_sysinb;
int kbd_hook_id = 1;
uint8_t scancode=0;

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


int (write_KBC_command)(uint8_t port, uint8_t commandByte) {

    uint8_t status;
    uint8_t attemps = 10;

    while (attemps) {

        if (read_KBC_status(&status) != 0){
            printf("Error: Status not available!\n");
            return 1;
        }

        if ((status & KBC_STATUS_INPUT) == 0){

            if(sys_outb(port, commandByte) != 0){
                printf("Error: Could not write commandByte!\n");
                return 1;
            }

            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        attemps--;
    }
    
    return 1;
}


int (keyboard_subscribe_interrupts)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(kbd_hook_id);
    return sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int(kbd_test_scan)() {
  bool flag = false;
  int ipc_status;
    uint8_t irq_set;
    message msg;

    if(keyboard_subscribe_interrupts(&irq_set) != 0) return 1;

    while(scancode != BREAK_ESC){ // a condição de paragem é obter um breakcode da tecla ESC

        if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
            printf("Error");
            continue;
        }

        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)){
                 case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        flag = false;
                        for (int i=0;i<10;i++){
                        uint8_t status;
                        if (util_sys_inb(KBC_STATUS_REGISTER,&status)!=0){
                          printf("Error in util_sys_inb inside kbd_test_scan\n");
                          return 1;
                        }

                        if (status & KBC_STATUS_OUTPUT){ //OUTPUT BUFFER FULL
                          if (status & KBC_STATUS_PARITY){
                            printf("Parity error\n");
                            return 1;
                          }
                          if (status & KBC_STATUS_TIMEOUT){
                            printf("Timeout error\n");
                            return 1;
                          }

    
                          if(util_sys_inb(KBC_OUTPUT_BUFFER,&scancode)!=0){
                            printf("Error in util_sys_inb to OUTPUT BUFFER inside kbd_test_scan\n");
                            return 1;
                          }
                          flag = true;
                          break;
                        }
                        tickdelay(micros_to_ticks(20000));
                        }
                        if (!flag) return 1;
                        kbd_print_scancode(!(scancode & BIT(7)), scancode == 0xE0 ? 2 : 1, &scancode);
                    }
              }
          }

    }

    if (sys_irqrmpolicy(&kbd_hook_id)!=0) return 1;
    if (kbd_print_no_sysinb(counter_sysinb)!=0) return 1;

  return 0;
}

int(kbd_test_poll)() {
  
    bool flag = false;
    int ipc_status;
    uint8_t irq_set;
    message msg;

    while(scancode != BREAK_ESC){ // a condição de paragem é obter um breakcode da tecla ESC

        if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
            printf("Error");
            continue;
        }

        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)){
                 case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        flag = false;
                        for (int i=0;i<10;i++){
                        uint8_t status;
                        if (util_sys_inb(KBC_STATUS_REGISTER,&status)!=0){
                          printf("Error in util_sys_inb inside kbd_test_scan\n");
                          return 1;
                        }

                        if (status & KBC_STATUS_OUTPUT){ //OUTPUT BUFFER FULL
                          if (status & KBC_STATUS_PARITY){
                            printf("Parity error\n");
                            return 1;
                          }
                          if (status & KBC_STATUS_TIMEOUT){
                            printf("Timeout error\n");
                            return 1;
                          }

    
                          if(util_sys_inb(KBC_OUTPUT_BUFFER,&scancode)!=0){
                            printf("Error in util_sys_inb to OUTPUT BUFFER inside kbd_test_scan\n");
                            return 1;
                          }
                          flag = true;
                          break;
                        }
                        tickdelay(micros_to_ticks(20000));
                        }
                        if (!flag) return 1;
                        kbd_print_scancode(!(scancode & BIT(7)), scancode == 0xE0 ? 2 : 1, &scancode);
                    }
              }
          }

    }

    uint8_t status;

    if (kbd_print_no_sysinb(counter_sysinb)!=0) return 1;

    if(write_KBC_command(0x64,0x20)!=0)return 1;

    if (util_sys_inb(KBC_STATUS_REGISTER,&status)!=0){
      printf("Error in util_sys_inb inside kbd_test_scan\n");
      return 1;
    }

  

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
