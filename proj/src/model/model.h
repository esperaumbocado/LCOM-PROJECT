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
#include "xpm/play.xpm"
#include "xpm/comma.xpm"
#include "xpm/period.xpm"


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
    Z,
    COMMA,
    PERIOD
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

typedef enum{
    CORRECT,
    INCORRECT,
    NOT_TYPED,
    MAIN
} WordStatus;

typedef struct{
    int index;
    char *word;
    int length;
    int starting_x;
    int starting_y;
    WordStatus status;
} Word;

void initialize_words();
void initialize_key_maps();
void initialize_sprites();
void destroy_sprites();
void update_keyboard();
void update_timer();
void key_handler();
void clear_current_word();
void add_current_word_to_typedWords();

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
 * @brief Checks the actions to be taken based on the mouse data
*/
void checkActions();

/**
 * @brief Gets a key based on a char
*/
Key char_to_key(char c);

#endif

