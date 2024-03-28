#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

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
 * if there was some error, the byte read from the OB is discarded
 * 
 */
void (mouse_ih)(void);

/**
 * @brief Parse packet
 * 
*/
void (parse)(void);

/**
 * @brief sync packet
 * 
*/
void (sync)(void);

/**
 * @brief Disables stream mode mouse data reporting
 *
 * sends the respective command to the mouse
 * 
 * @return Return 0 upon success and non-zero otherwise
 */ 
int (mouse_disable_data_reporting)(void);

/**
 * @brief Checks if the output buffer is full
 * 
 * so we know if we can read from it
 * 
 * @return true if the output buffer is full, false otherwise
*/
bool (obf_full)();

/**
 * @brief Checks if the input buffer is empty
 * 
 * so we know if we can write to it
 * 
 * @return true if the input buffer is empty, false otherwise
*/
bool (ibf_empty)();

#endif /* __MOUSE_H */
