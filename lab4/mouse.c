#include <lcom/lcf.h>

#include <stdint.h>
#include "mouse.h"

int mouse_hook_id = 2; 
uint8_t byte_received;
int counter = 0;
struct packet pp;
int bytes_read = 0;
uint8_t pp_bytes[3];
    
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

// 0xF4, ativa o data report;
// 0xF5, desativa o data report;
// 0xEA, ativa o stream mode;
// 0xF0, ativa o remote mode; polling 
// 0xEB, manda um request de novos dados;

void (mouse_ih)(){    
    //read the status register and check if there was some communication error;
    uint8_t st;

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

    if (((st & BIT(0)) != 0) && (st & BIT(5))){ // if the output buffer is full, we can read it
      if (util_sys_inb(0x60 , &byte_received) != OK){
        printf("this byte should be discarded \n");  
        return;
      }
      // otherwise, we successfully read the byte
    }

    // i guess i will use later for enabling reporting...?

    // // request forwarding of byte (command) to the mouse
    // if (sys_outb(0x64, 0xD4) != OK){
    //     printf("writing 0xD4 to 0x64 was not ok. \n");  
    //     return;
    // }

 
    // uint8_t commandWord;
    // if (util_sys_inb(0x60, &commandWord) != OK){ // ler o command byte
    //     printf("reading commandWord from 0x60 was not ok. \n");  
    //     // we should now dischard this byte 
    //     return;
    // }  

    // // write the command byte    
    // commandWord = commandWord | 0x00; // ?
    // if (sys_outb(0x60, commandWord) != OK){
    //     printf("writing commandWord to 0x60 was not ok. \n");  
    //     return;
    // }

    // uint8_t response;
    // // read the acknowledgement byte received from the mouse
    // if (util_sys_inb(0x60, &response) != OK){
    //     printf("reading response from 0x60 was not ok. \n");  
    //     return;
    // }

    // if (response != 0xFA){
    //     printf("response was not a success case. \n");  
    //     if (response == 0xFE){
    //         printf("response was 0xFE, send the entire command again! \n");  
    //     }
    //     return;
    // }
}

void (parse)(){
  pp.bytes[0] = pp_bytes[0];
  pp.bytes[1] = pp_bytes[1];
  pp.bytes[2] = pp_bytes[2];

  pp.rb = pp_bytes[0] & BIT(1); 
  pp.mb = pp_bytes[0] & BIT(2);
  pp.lb = pp_bytes[0] & BIT(0);
  
  if (pp_bytes[0] & BIT(4)){
      pp.delta_x = pp_bytes[1] | 0xFF00; 
  } else {
      pp.delta_x =pp_bytes[1];
  }

  if (pp_bytes[0] & BIT(5)){
      pp.delta_y = pp_bytes[2] | 0xFF00; 
  } else {
      pp.delta_y = pp_bytes[2];
  }
  
  pp.x_ov = pp_bytes[0] & BIT(6);
  pp.y_ov = pp_bytes[0] & BIT(7);
}

void (sync)(){  
  if (bytes_read == 0){
      if (byte_received & BIT(3)){
          bytes_read++;
          pp_bytes[0] = byte_received;
      };
  } else if (bytes_read == 1){
      pp_bytes[1] = byte_received;
      bytes_read++;
  } else if (bytes_read == 2){
      pp_bytes[2] = byte_received;
      bytes_read++;
  }
}

int (mouse_disable_data_reporting)(){
    // disable data reporting with 0xF5 command
    // certainly it is wrongkkkkkkkk
    int tries=10;
    uint8_t response;

    while (tries>0){
      tries--;
      // write byte mouse command
      if (sys_outb(0x64, 0xD4) != OK){
          printf("writing 0xD4 to 0x64 was not ok. \n");  
          continue;
      }
      // disable data reporting command
      if (sys_outb(0x60, 0xF5) != OK){
          printf("writing 0xF5 to 0x60 was not ok. \n");  
          continue;
      }

      tickdelay(micros_to_ticks(DELAY_US));
      if (util_sys_inb(0x60, &response) != OK){
          printf("reading response from 0x60 was not ok. \n"); 
          continue; 
      }

      if (response != 0xFA){ 
          printf("response was not a success case. \n");  
          if (response == 0xFE){ // NACK
              printf("response was 0xFE, send the entire command again! \n");  
              continue;
          }
          continue;
      } else { // ACK
          return 0;
      }
    }
    printf("Mouse did not acknowledge disable data reporting command.\n");
    return 1;
}
