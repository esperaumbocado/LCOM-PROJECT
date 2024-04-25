#ifndef _TIMER_H_
#define _TIMER_H_

#include <lcom/lcf.h>

#include "i8254.h"


int (timer_set_frequency)(uint8_t timer, uint32_t freq);

int (timer_subscribe)();

int (timer_unsubscribe_int)();

void (timer_int_handler)();

int (timer_get_conf)(uint8_t timer, uint8_t *st);

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field);



#endif
