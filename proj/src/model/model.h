#ifndef _MODEL_H_
#define _MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../drivers/graphics/video.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/mouse/mouse.h"
#include "model/sprite.h"
#include "xpm/letters.xpm"
#include "xpm/cursor.xpm"
#include "xpm/novoTeste.xpm"


typedef enum{
    NONE_KEY,
    ENTER,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z
} Key;

typedef enum{
    NONE_STATE,
    MENU,
    GAME
} GameState;

typedef enum{
    RUNNING,
    EXIT
} GeneralState;


void initialize_sprites();
void destroy_sprites();
void update_keyboard();
void update_timer();
void key_handler();

/**
 * @brief Handles the offset for drawing
 * @param x the offset to be handled
 * @return the new offset
*/
int offset_handler(int x);

// MOUSE STUFF
/**
 * @brief Initializes the mouse data
*/
void initialize_mouse_data();

/**
 * @brief Sync mouse bytes and calls update_mouse_date
*/
void update_mouse();

/**
 * @brief Updates last position of the cursor
*/
void update_cursor_last_pos();

/**
 * @brief Gets a key based on a char
*/
Key char_to_key(char c);

#endif

