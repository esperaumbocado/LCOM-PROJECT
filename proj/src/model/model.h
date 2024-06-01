#ifndef _MODEL_H_
#define _MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../drivers/graphics/video.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/mouse/mouse.h"
#include "model/sprite.h"
#include "../view/xpm/letters.xpm"
#include "../view/xpm/cursor.xpm"
#include "../view/xpm/star.xpm"

#include "../view/xpm/play.xpm"
#include "../view/xpm/instructions.xpm"

#include "../view/xpm/timer15.xpm"
#include "../view/xpm/timer30.xpm"
#include "../view/xpm/timer60.xpm"

#include "../view/xpm/punctuation/comma.xpm"
#include "../view/xpm/punctuation/period.xpm"
#include "../view/xpm/punctuation/colon.xpm"
#include "../view/xpm/punctuation/exclamation.xpm"
#include "../view/xpm/punctuation/right_parenthesis.xpm"

#include "../view/xpm/numbers.xpm"

#include "../view/xpm/panda.xpm"
#include "../view/xpm/bambu_right.xpm"
#include "../view/xpm/bambu_left.xpm"

#include "../view/xpm/back_to_menu.xpm"
#include "../view/xpm/play_again.xpm"

#include "../view/xpm/animation/panda0.xpm"
#include "../view/xpm/animation/mad1.xpm"
#include "../view/xpm/animation/mad2.xpm"
#include "../view/xpm/animation/mad3.xpm"
#include "../view/xpm/animation/mad4.xpm"
#include "../view/xpm/animation/happy1.xpm"
#include "../view/xpm/animation/happy2.xpm"
#include "../view/xpm/animation/happy3.xpm"
#include "../view/xpm/animation/happy4.xpm"

#define MAX_WORDS 40
#define MAX_WORD_LENGTH 20

#define KEY_SPRITE_MAP_SIZE 43

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
    STATISTICS,
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
    int correctWords;    // Number of correct words typed
    int incorrectWords;  // Number of incorrect words typed
    int correctLetters;  // Number of correct letters typed
    int incorrectLetters;// Number of incorrect letters typed
    int typedLetters;    // Number of letters typed
    int typedWords;      // Number of words typed
    int time;
} Statistics;

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

/**
 * @brief Initializes the key sprite map
*/
void initialize_key_sprite_map();

void destroy_sprites();
void update_keyboard();
void update_timer();
void key_handler();
void destroy_test();
void destroy_stats();

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
int offset_handler(int x, int end_x);

/**
 * @brief Sync mouse bytes and calls update_mouse_date
*/
void update_mouse();


/**
 * @brief Checks if the mouse sprite is over a button and if left button is pressed
 * @param button_sprite the sprite of the button
 * @return 1 if the button was pressed, 0 otherwise
*/
int pressed_button(Sprite *button_sprite);

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


void updateAnimation(Animation *animation);

#endif

