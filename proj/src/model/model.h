#ifndef _MODEL_H_
#define _MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../drivers/graphics/video.h"
#include "../drivers/keyboard/keyboard.h"
#include "model/sprite.h"
#include "xpm/letters.xpm"

typedef enum{
    NONE_KEY,
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


#endif

