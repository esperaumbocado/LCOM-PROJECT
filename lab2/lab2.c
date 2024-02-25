#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

// make
// minix$ lcom_run lab2 "config 0 all -t 0"
// timer::timer_test_read_config(0, all)
// timer_test_read_config is not yet implemented!        
// Test FAILED for seed fd11d0f2!
// Timer Config: 0x79

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
 // only using timer 0 
 // do not change timer 1 configuration
int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  // timer 0, 1, 2
  // tsf_all - status byte, in hexadecimal
  // tsf_initial - initialization mode
  // tsf_mode - counting mode
  // tsf_base - counting base

  uint8_t st; // status
  
  if (timer_get_conf(timer, &st) != OK) {
    printf("Error in timer_get_conf\n");
    return 1;
  }  // lê a configuração do timer
  
  if (timer_display_conf(timer, st, field) != OK) {
    printf("Error in timer_display_conf\n");
    return 1;
  }  // mostra a configuração do timer

  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(timer_test_int)(uint8_t time) {
  /* To be implemented by the students */
  //lab2, second part
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
