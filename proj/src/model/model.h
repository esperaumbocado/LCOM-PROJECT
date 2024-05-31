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
#include "xpm/star.xpm"

#include "xpm/play.xpm"
#include "xpm/instructions.xpm"

#include "xpm/timer15.xpm"
#include "xpm/timer30.xpm"
#include "xpm/timer60.xpm"

#include "xpm/punctuation/comma.xpm"
#include "xpm/punctuation/period.xpm"
#include "xpm/punctuation/colon.xpm"
#include "xpm/punctuation/exclamation.xpm"
#include "xpm/punctuation/right_parenthesis.xpm"

#include "xpm/numbers/zero.xpm"
#include "xpm/numbers/one.xpm"
#include "xpm/numbers/two.xpm"
#include "xpm/numbers/three.xpm"
#include "xpm/numbers/four.xpm"
#include "xpm/numbers/five.xpm"
#include "xpm/numbers/six.xpm"
#include "xpm/numbers/seven.xpm"
#include "xpm/numbers/eight.xpm"
#include "xpm/numbers/nine.xpm"

#define MAX_WORDS 30
#define MAX_WORD_LENGTH 20
#define POOL_SIZE 90

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
    PERIOD,
    COLON,
    EXCLAMATION,
    RIGHT_PARENTHESIS,
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    BACK,
    ESC
} Key;

typedef enum{
    MENU,
    INSTRUCTIONS,
    GAME,
    TIMERS
} GameState;

typedef enum{
    RUNNING,
    EXIT
} GeneralState;

typedef struct{
    int x;
    int y;
    int width;
    int height;
} Caret;


typedef struct {
    char character;  // The character itself
    int status;      // Status of the letter: 0 for not typed, 1 for typed, -1 for incorrect
} Letter;

typedef struct {
    Letter letters[MAX_WORD_LENGTH];  // Array of letters for the current word
    int length;                        // Length of the word
    int status;                        // Status of the word: 0 for not typed, 1 for correct, -1 for incorrect
    int x;                             // X position of the word
    int y;                             // Y position of the word
    int line;                          // Line of the word
} Word;

typedef struct {
    Word *words;             // Dynamic array of words for the current test
    int wordCount;           // Number of words in the current test
    int currentWordIndex;    // Index of the current word
    int currentInputIndex;   // Index of the current input for the current word
    char currentInput[MAX_WORD_LENGTH];  // User's current input for the current word
    int number_of_lines;     // Number of lines in the test
    
} TypingTest;

typedef enum {
    GESTURE_ZERO,
    GESTURE_LB
} GestureState;


void initialize_key_maps();
void initialize_sprites();
void destroy_sprites();
void update_keyboard();
void update_timer();
void key_handler();
void destroy_test();

/**
* @brief Updates currentState and gameStateChange
*/
void setGameState(GameState state);

/**
 * @brief Resets offset to starting of box
*/
void reset_offset();

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

/**
 * @brief Updates the swipe-right gesture status
*/
void checkGesture();

/**
 * @brief Fills correctly the remaining letters of the current word
*/
void fill_current_word();

void handle_space_key(TypingTest *test);

#endif

