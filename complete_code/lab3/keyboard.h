#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdint.h>

#include "i8042.h"
#include "KBC.h"

int (keyboard_subscribe_int)(uint8_t *bit_no);

int (keyboard_unsubscribe_int)();

void (kbd_ih)();
