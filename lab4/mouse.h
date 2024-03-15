#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdbool.h>
#include <stdint.h>


/** @defgroup mouse
 * @{
 * Functions for using the i8042 KBC
 */ 


/**
 * @brief Subscribes and enables interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_unsubscribe_int)();

/**
 * @brief Enables stream mode mouse data reporting
 *
 * sends the respective command to the mouse
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_enable_data_reporting)(void);
 
 
/**
 * @brief Handles mouse interrupts
 *
 * Reads the st reg and the output buffer
 * if there was some error, the byte read from the OB should be discarded
 * 
 */
void (mouse_ih)(void);
#endif /* __MOUSE_H */
