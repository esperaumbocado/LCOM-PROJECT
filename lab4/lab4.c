#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "mouse.h"

extern int timer_hook_id;
extern int mouse_hook_id;
extern int counter;
extern struct packet pp;
extern int bytes_read;

// doxygen 
// https://web.fe.up.pt/~pfs/aulas/lcom2324/labs/lab4/src/doc/files.html

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (mouse_test_packet)(uint32_t cnt) {
    uint8_t mouse_irq_set;
    int ipc_status;
    message msg;
    if (mouse_enable_data_reporting() != OK) return 1; // done with 0xF4 command
    if (mouse_subscribe_int(&mouse_irq_set) != OK) return 1;
    int r;
    uint32_t packets_read = 0;
    while (packets_read < cnt) {
        if ((r=driver_receive(ANY, &msg, &ipc_status))!=0){
            // msg and ipc_status will be initialized by driver_receive()
            printf("driver_receive failed with: %d, r");
            continue;
        }
        if (is_ipc_notify(ipc_status)){ // received notification
            switch (_ENDPOINT_P(msg.m_source)) 
            // ENDPOINT_P extracts the process identifier from a process's endpoint
            {
                case HARDWARE:
                    if (msg.m_notify.interrupts & mouse_irq_set){ // subscribed interrupt
                        mouse_ih(); // reads only one byte per interrupt
                        sync();
                        if (bytes_read ==3){
                            parse();
                            mouse_print_packet(&pp);
                            packets_read++;
                            bytes_read = 0;
                        }              
                    }
                    break;
                default:
                    break;
            }
        }
    }
    if (mouse_unsubscribe_int() != OK) return 1;
    if (mouse_disable_data_reporting() != OK) return 1;
    // if (mouse_disable_data_reporting() != OK) return 1;
    
    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    // mouse_test_packet() + kbd_test_timed_scan()
    printf("%s(%u): under construction\n", __func__, idle_time);

    uint8_t mouse_irq_set;
    uint8_t timer_irq_set;
    int ipc_status;
    message msg;
    if (mouse_subscribe_int(&mouse_irq_set) != OK) return 1;
    if (timer_subscribe_int(&timer_irq_set) != OK) return 1;
    // so that the mouse sends packets 
    // reporting its displacement or changes in the state of buttons
    if (mouse_enable_data_reporting() != OK) return 1;
    
    int r;

    while (counter < idle_time*60) {
        if ((r=driver_receive(ANY, &msg, &ipc_status))!=0){
            // msg and ipc_status will be initialized by driver_receive()
            printf("driver_receive failed with: %d, r");
            continue;
        }
        if (is_ipc_notify(ipc_status)){ // received notification
            switch (_ENDPOINT_P(msg.m_source)) 
            // ENDPOINT_P extracts the process identifier from a process's endpoint
            {
                case HARDWARE:
                    if (msg.m_notify.interrupts & mouse_irq_set){ // subscribed interrupt
                        mouse_ih(); // reads only one byte per interrupt
                        // when a packet is received (the 3rd byte)
                        // parse it 
                        // initiliaze the struct packet, 
                        // whose address is pp

                        //mouse_print_packet(&pp); //*pp
                        counter = 0;
                    }
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        timer_int_handler();
                        if (counter%60 ==0){
                            timer_print_elapsed_time();
                            printf("secs = %d\n", counter/60);
                        }
                        // determine timer 0 freq with sys_hz()
                    }

                    break;
                default:
                    break;
            }
        }
    }

    if (mouse_unsubscribe_int() != OK) return 1;
    if (timer_unsubscribe_int() != OK) return 1;
    // - disable data reporting 

    return 1;
}

// state machines
//len is 8 or 16?
int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance){
    /* To be completed */
    printf("%s: under construction\n", __func__);
    uint8_t mouse_irq_set;

    if (mouse_subscribe_int(&mouse_irq_set) != OK) return 1;
    // so that the mouse sends packets 
    // reporting its displacement or changes in the state of buttons
    if (mouse_enable_data_reporting() != OK) return 1;


    // do essentially the same as mouse_test_packet(),
    // display the packets received from the mouse
    // exit condition: the user draws a logical AND symbol
    // First line drawn with left button pressed down (and no other)
    // Second line drawn with right button pressed down (and no other)
    // abs slope of each line > 1
    // value of displacemente of each line on X >= x_len
    // start of a line: press down the button when all buttons are released
    // negative displacements <= tolerance
    // end of a line: release of the button
    // realase left button first,
    // movements <= tolerance
    // then press right 
    // each mouse packet can lead to at most one event relevant for gesture matching
    // (ignore displacements reported on packets that generates
    // the start of line and the end of line)
    // if any of the above arent satisfied -> state=INIT

    if (mouse_unsubscribe_int() != OK) return 1;
    // - disable data reporting 

    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
