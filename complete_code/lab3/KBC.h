#include <stdint.h>
#include "i8042.h"

int KBC_output(uint8_t *output);
int write_KBC(uint8_t port, uint8_t in);
