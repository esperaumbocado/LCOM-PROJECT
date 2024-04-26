#include "KBC.h"

int (read_KBC_status)(uint8_t* status) {
    return util_sys_inb(KBC_STATUS_REG, status);
}

int read_KBC_output(uint8_t port, uint8_t *output, uint8_t mouse) {

    uint8_t status;
    uint8_t attempts = 10;

    while(attempts) {

        if(read_KBC_status(&status) != 0) {
            printf("Error: Status not available!\n");
            return 1;
        }

        // Verifica se o output buffer está cheio
        if((status & BIT(0)) != 0) {
            // Lê o buffer de saída
            if(util_sys_inb(port, output) != 0) {
                printf("Error: Could not read output!\n");
                return 1;
            }
            // Checa erro de paridade
            if((status & BIT(7)) != 0) {
                printf("Error: Parity error!\n");
                return 1;
            }
            // Checa erro de timeout
            if((status & BIT(6)) != 0) {
                printf("Error: Parity error!\n");
                return 1;
            }
            // Fica à espera do mouse
            if(mouse && !(status & BIT(5))) {
                printf("Error: Mouse output not found!\n");
                return 1;
            }
            // Fica à espera do output do teclado
            if(!mouse && (status & BIT(5))) {
                printf("Error: Keyboard ouutput not found\n");
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        attempts--;
    }
    return 1;
}

int(write_KBC_command)(uint8_t port, uint8_t commandByte) {
    uint8_t status;
    uint8_t attempts = MAX_ATTEMPTS;

    while(attempts) {
        if(read_KBC_status(&status) != 0) {
            printf("Error: Status not available!\n");
            return 1;
        }
        if((status & FULL_IN_BUFFER) == 0) {
            if(sys_outb(port, commandByte) != 0) {
                printf("Error: Could not write commandByte!\n");
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        attempts--;
    }
    return 1;
}

