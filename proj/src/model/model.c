#include "model.h"
#include "../view/view.h"
#include "wordPool.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


extern uint8_t data;
Key currentKey = NONE_KEY;
GameState currentState;
extern uint8_t *main_frame_buffer;
extern uint8_t *secondary_frame_buffer;
extern uint8_t *secondary_frame_buffer_no_mouse;
extern uint32_t frame_size;
extern vbe_mode_info_t mode_info;

int x_offset;
int y_offset;

//Caret
Caret caret;

//Maps of keys
Key keyMap[256];
char charMap[256];

bool gameStateChange;

// TIMER VARIABLES
int timer;

extern int counter;

// MOUSE VARIABLES

extern int bytes_read;
extern struct packet pp;
extern mouse_position mouse_pos;

// Checking for swipe right gesture
GestureState gestureState = GESTURE_ZERO;
int x_gesture = 0;

int stars; // stars for swiping right

//WORD

Sprite *CURSOR_SPRITE;

Sprite *STAR_SPRITE;

Sprite *PLAY_SPRITE;
Sprite *INSTRUCTIONS_SPRITE;

Sprite *TIMER15_SPRITE;
Sprite *TIMER30_SPRITE;
Sprite *TIMER60_SPRITE;

Sprite *A_SPRITE;
Sprite *B_SPRITE;
Sprite *C_SPRITE;
Sprite *D_SPRITE;
Sprite *E_SPRITE;
Sprite *F_SPRITE;
Sprite *G_SPRITE;
Sprite *H_SPRITE;
Sprite *I_SPRITE;
Sprite *J_SPRITE;
Sprite *K_SPRITE;
Sprite *L_SPRITE;
Sprite *M_SPRITE;
Sprite *N_SPRITE;
Sprite *O_SPRITE;
Sprite *P_SPRITE;
Sprite *Q_SPRITE;
Sprite *R_SPRITE;
Sprite *S_SPRITE;
Sprite *T_SPRITE;
Sprite *U_SPRITE;
Sprite *V_SPRITE;
Sprite *W_SPRITE;
Sprite *X_SPRITE;
Sprite *Y_SPRITE;
Sprite *Z_SPRITE;
Sprite *COMMA_SPRITE;
Sprite *PERIOD_SPRITE;

Sprite *EXCLAMATION_SPRITE;
Sprite *COLON_SPRITE;
Sprite *RIGHT_PARENTHESIS_SPRITE;

Sprite *ZERO_SPRITE;
Sprite *ONE_SPRITE;
Sprite *TWO_SPRITE;
Sprite *THREE_SPRITE;
Sprite *FOUR_SPRITE;
Sprite *FIVE_SPRITE;
Sprite *SIX_SPRITE;
Sprite *SEVEN_SPRITE;
Sprite *EIGHT_SPRITE;
Sprite *NINE_SPRITE;

Sprite *PANDA_SPRITE;

// Panda dimensions

int startPandaX;
int startPandaY;
int endPandaX;
int endPandaY;


// Play button dimensions
int startPlayX;
int startPlayY;
int endPlayX;
int endPlayY;

// Instructions button dimensions
int startInstructionsX;
int startInstructionsY;
int endInstructionsX;
int endInstructionsY;

// Timer buttons dimensions
int startTimer15X;
int startTimer15Y;
int endTimer15X;
int endTimer15Y;

int startTimer30X;
int startTimer30Y;
int endTimer30X;
int endTimer30Y;

int startTimer60X;
int startTimer60Y;
int endTimer60X;
int endTimer60Y;

TypingTest *test;
extern char* wordPool[];

// screen box dimensions
int startBoxX = 200;
int startBoxY = 200;
int sizeBoxX;
int sizeBoxY;

Statistics *stats;



void initialize_sprites() {

    PANDA_SPRITE = create_sprite_xpm((xpm_map_t)panda_xpm);

    CURSOR_SPRITE = create_sprite_xpm((xpm_map_t)cursor_xpm);

    STAR_SPRITE = create_sprite_xpm((xpm_map_t)star_xpm);

    PLAY_SPRITE = create_sprite_xpm((xpm_map_t)play_xpm);

    INSTRUCTIONS_SPRITE = create_sprite_xpm((xpm_map_t)instructions_xpm);

    TIMER15_SPRITE = create_sprite_xpm((xpm_map_t)timer15_xpm);
    TIMER30_SPRITE = create_sprite_xpm((xpm_map_t)timer30_xpm);
    TIMER60_SPRITE = create_sprite_xpm((xpm_map_t)timer60_xpm);

    startPandaX = mode_info.XResolution / 2 - PANDA_SPRITE->width / 2;
    startPandaY = mode_info.YResolution / 2 - PANDA_SPRITE->height / 2 - 180;
    endPandaX = startPandaX + PANDA_SPRITE->width;
    endPandaY = startPandaY + PANDA_SPRITE->height;

    startPlayX = mode_info.XResolution / 2 - PLAY_SPRITE->width / 2;
    startPlayY = endPandaY + 50;
    endPlayX = startPlayX + PLAY_SPRITE->width;
    endPlayY = startPlayY + PLAY_SPRITE->height;

    startInstructionsX = mode_info.XResolution / 2 - INSTRUCTIONS_SPRITE->width / 2;
    startInstructionsY = endPlayY + 30;
    endInstructionsX = startInstructionsX + INSTRUCTIONS_SPRITE->width;
    endInstructionsY = startInstructionsY + INSTRUCTIONS_SPRITE->height;

    startTimer15X = mode_info.XResolution / 2 - TIMER15_SPRITE->width / 2;
    startTimer15Y = mode_info.YResolution / 2 - TIMER15_SPRITE->height / 2 ;
    endTimer15X = startTimer15X + TIMER15_SPRITE->width;
    endTimer15Y = startTimer15Y + TIMER15_SPRITE->height;

    startTimer30X = mode_info.XResolution / 2 - TIMER30_SPRITE->width / 2;
    startTimer30Y = mode_info.YResolution / 2 - TIMER30_SPRITE->height / 2 + 100;
    endTimer30X = startTimer30X + TIMER30_SPRITE->width;
    endTimer30Y = startTimer30Y + TIMER30_SPRITE->height;

    startTimer60X = mode_info.XResolution / 2 - TIMER60_SPRITE->width / 2;
    startTimer60Y = mode_info.YResolution / 2 - TIMER60_SPRITE->height / 2 + 200;
    endTimer60X = startTimer60X + TIMER60_SPRITE->width;
    endTimer60Y = startTimer60Y + TIMER60_SPRITE->height;

    A_SPRITE = create_sprite_xpm((xpm_map_t)KEY_A_xpm);
    B_SPRITE = create_sprite_xpm((xpm_map_t)KEY_B_xpm);
    C_SPRITE = create_sprite_xpm((xpm_map_t)KEY_C_xpm);
    D_SPRITE = create_sprite_xpm((xpm_map_t)KEY_D_xpm);
    E_SPRITE = create_sprite_xpm((xpm_map_t)KEY_E_xpm);
    F_SPRITE = create_sprite_xpm((xpm_map_t)KEY_F_xpm);
    G_SPRITE = create_sprite_xpm((xpm_map_t)KEY_G_xpm);
    H_SPRITE = create_sprite_xpm((xpm_map_t)KEY_H_xpm);
    I_SPRITE = create_sprite_xpm((xpm_map_t)KEY_I_xpm);
    J_SPRITE = create_sprite_xpm((xpm_map_t)KEY_J_xpm);
    K_SPRITE = create_sprite_xpm((xpm_map_t)KEY_K_xpm);
    L_SPRITE = create_sprite_xpm((xpm_map_t)KEY_L_xpm);
    M_SPRITE = create_sprite_xpm((xpm_map_t)KEY_M_xpm);
    N_SPRITE = create_sprite_xpm((xpm_map_t)KEY_N_xpm);
    O_SPRITE = create_sprite_xpm((xpm_map_t)KEY_O_xpm);
    P_SPRITE = create_sprite_xpm((xpm_map_t)KEY_P_xpm);
    Q_SPRITE = create_sprite_xpm((xpm_map_t)KEY_Q_xpm);
    R_SPRITE = create_sprite_xpm((xpm_map_t)KEY_R_xpm);
    S_SPRITE = create_sprite_xpm((xpm_map_t)KEY_S_xpm);
    T_SPRITE = create_sprite_xpm((xpm_map_t)KEY_T_xpm);
    U_SPRITE = create_sprite_xpm((xpm_map_t)KEY_U_xpm);
    V_SPRITE = create_sprite_xpm((xpm_map_t)KEY_V_xpm);
    W_SPRITE = create_sprite_xpm((xpm_map_t)KEY_W_xpm);
    X_SPRITE = create_sprite_xpm((xpm_map_t)KEY_X_xpm);
    Y_SPRITE = create_sprite_xpm((xpm_map_t)KEY_Y_xpm);
    Z_SPRITE = create_sprite_xpm((xpm_map_t)KEY_Z_xpm);

    COMMA_SPRITE = create_sprite_xpm((xpm_map_t)KEY_COMMA_xpm);
    PERIOD_SPRITE = create_sprite_xpm((xpm_map_t)KEY_PERIOD_xpm);

    EXCLAMATION_SPRITE = create_sprite_xpm((xpm_map_t)exclamation_xpm);
    COLON_SPRITE = create_sprite_xpm((xpm_map_t)colon_xpm);
    RIGHT_PARENTHESIS_SPRITE = create_sprite_xpm((xpm_map_t)right_parenthesis_xpm);

    ZERO_SPRITE = create_sprite_xpm((xpm_map_t)zero_xpm);
    ONE_SPRITE = create_sprite_xpm((xpm_map_t)one_xpm);
    TWO_SPRITE = create_sprite_xpm((xpm_map_t)two_xpm);
    THREE_SPRITE = create_sprite_xpm((xpm_map_t)three_xpm);
    FOUR_SPRITE = create_sprite_xpm((xpm_map_t)four_xpm);
    FIVE_SPRITE = create_sprite_xpm((xpm_map_t)five_xpm);
    SIX_SPRITE = create_sprite_xpm((xpm_map_t)six_xpm);
    SEVEN_SPRITE = create_sprite_xpm((xpm_map_t)seven_xpm);
    EIGHT_SPRITE = create_sprite_xpm((xpm_map_t)eight_xpm);
    NINE_SPRITE = create_sprite_xpm((xpm_map_t)nine_xpm);
}


void initialize_key_maps() {
    for (int i = 0; i < 256; i++) {
        keyMap[i] = NONE_KEY;
        charMap[i] = 0;
    }

    keyMap[KEY_A] = A; charMap[KEY_A] = 'A';
    keyMap[KEY_B] = B; charMap[KEY_B] = 'B';
    keyMap[KEY_C] = C; charMap[KEY_C] = 'C';
    keyMap[KEY_D] = D; charMap[KEY_D] = 'D';
    keyMap[KEY_E] = E; charMap[KEY_E] = 'E';
    keyMap[KEY_F] = F; charMap[KEY_F] = 'F';
    keyMap[KEY_G] = G; charMap[KEY_G] = 'G';
    keyMap[KEY_H] = H; charMap[KEY_H] = 'H';
    keyMap[KEY_I] = I; charMap[KEY_I] = 'I';
    keyMap[KEY_J] = J; charMap[KEY_J] = 'J';
    keyMap[KEY_K] = K; charMap[KEY_K] = 'K';
    keyMap[KEY_L] = L; charMap[KEY_L] = 'L';
    keyMap[KEY_M] = M; charMap[KEY_M] = 'M';
    keyMap[KEY_N] = N; charMap[KEY_N] = 'N';
    keyMap[KEY_O] = O; charMap[KEY_O] = 'O';
    keyMap[KEY_P] = P; charMap[KEY_P] = 'P';
    keyMap[KEY_Q] = Q; charMap[KEY_Q] = 'Q';
    keyMap[KEY_R] = R; charMap[KEY_R] = 'R';
    keyMap[KEY_S] = S; charMap[KEY_S] = 'S';
    keyMap[KEY_T] = T; charMap[KEY_T] = 'T';
    keyMap[KEY_U] = U; charMap[KEY_U] = 'U';
    keyMap[KEY_V] = V; charMap[KEY_V] = 'V';
    keyMap[KEY_W] = W; charMap[KEY_W] = 'W';
    keyMap[KEY_X] = X; charMap[KEY_X] = 'X';
    keyMap[KEY_Y] = Y; charMap[KEY_Y] = 'Y';
    keyMap[KEY_Z] = Z; charMap[KEY_Z] = 'Z';
    keyMap[KEY_COMMA] = COMMA; charMap[KEY_COMMA] = ',';
    keyMap[KEY_PERIOD] = PERIOD; charMap[KEY_PERIOD] = '.';
    keyMap[KEY_ENTER] = ENTER;
    keyMap[KEY_DELETE] = BACK;
    keyMap[KEY_ESC] = ESC;
    keyMap[KEY_SPACE] = NONE_KEY;
}

void initializeTest(TypingTest **testPtr, char *wordPool[], int poolSize, int wordCount) {
    if (poolSize > POOL_SIZE || poolSize <= 0) {
        fprintf(stderr, "Invalid poolSize: %d\n", poolSize);
        exit(EXIT_FAILURE);
    }
    if (wordCount <= 0 || wordCount > MAX_WORDS) {
        fprintf(stderr, "Invalid wordCount: %d\n", wordCount);
        exit(EXIT_FAILURE);
    }

    *testPtr = (TypingTest*)malloc(sizeof(TypingTest));
    if (*testPtr == NULL) {
        fprintf(stderr, "Failed to allocate memory for TypingTest\n");
        exit(EXIT_FAILURE);
    }
    TypingTest *test = *testPtr;

    test->words = (Word*)malloc((wordCount + 1) * sizeof(Word)); // Allocate one extra element for the NULL terminator
    if (test->words == NULL) {
        fprintf(stderr, "Failed to allocate memory for words array\n");
        free(test);
        exit(EXIT_FAILURE);
    }

    test->wordCount = wordCount;
    test->number_of_lines = -1;
    test->currentWordIndex = 0;
    memset(test->currentInput, 0, MAX_WORD_LENGTH);

    srand(time(NULL));

    for (int i = 0; i < wordCount; i++) {
        int index = rand() % poolSize;
        char *word = wordPool[index];
        Word newWord;
        newWord.length = strlen(word);
        newWord.status = 0;
        for (int j = 0; j < newWord.length; j++) {
            newWord.letters[j].character = word[j];
            newWord.letters[j].status = 0;
        }
        newWord.letters[newWord.length].character = '\0';
        newWord.letters[newWord.length].status = 0;
        test->words[i] = newWord;
    }

    test->words[wordCount].length = 0;
    test->words[wordCount].status = 0;
    test->words[wordCount].x = -1;
    test->words[wordCount].y = -1;
    test->words[wordCount].line = -1;
    test->words[wordCount].letters[0].character = '\0';
    test->words[wordCount].letters[0].status = 0;
}
void initializeStats(Statistics **statsPtr) {
    *statsPtr = (Statistics*)malloc(sizeof(Statistics));
    if (*statsPtr == NULL) {
        fprintf(stderr, "Failed to allocate memory for Statistics\n");
        exit(EXIT_FAILURE);
    }
    Statistics *stats = *statsPtr;

    stats->correctWords = 0;
    stats->incorrectWords = 0;
    stats->correctLetters = 0;
    stats->incorrectLetters = 0;
    stats->typedLetters = 0;
    stats->typedWords = 0;
}


void load_new_word(TypingTest *test, int index){
    int wordIndex = rand() % 50;
    char *word = wordPool[wordIndex];
    Word newWord;
    newWord.length = strlen(word);
    newWord.status = 0;
    for (int j = 0; j < newWord.length; j++) {
        newWord.letters[j].character = word[j];
        newWord.letters[j].status = 0;
    }
    newWord.letters[newWord.length].character = '\0';
    newWord.letters[newWord.length].status = 0;
    test->words[index] = newWord;
}

void shift_words_up(TypingTest *test) {
    int penultimate_line_index = 0;
    int last_line_index = 0;
    for (int i = 0; i < test->wordCount - 1; i++) {
        if (test->words[i].line == test->number_of_lines - 1) {
            test->words[0+penultimate_line_index] = test->words[i];
            penultimate_line_index++;
        }

        if (test->words[i].line == test->number_of_lines) {
            test->words[penultimate_line_index+last_line_index] = test->words[i];
            last_line_index++;
        }
    }

    for (int i = penultimate_line_index+last_line_index; i < test->wordCount; i++) {
        load_new_word(test, i);
    }

    
    drawBackground(GAME);
}

void reset_offset() {
    x_offset = startBoxX;
    y_offset = startBoxY;
}

int offset_handler(int x) {
    if (x == 0) {
        x_offset += 16;
    }if (x == 1) {
        x_offset -= 16;
    }
    if (x_offset + 16 >= startBoxX + sizeBoxX) {
        x_offset = startBoxX;
        y_offset += 32;
    }
    return 1;
}

void update_timer() {
    memcpy(main_frame_buffer, secondary_frame_buffer, frame_size);
    memcpy(secondary_frame_buffer, secondary_frame_buffer_no_mouse, frame_size);
    (timer_int_handler)();

    switch (currentState){
        case GAME:
            if (timer == 0){
                setGameState(MENU);
                printf("Stats:\n");
                printf("Correct words: %d\n", stats->correctWords);
                printf("Incorrect words: %d\n", stats->incorrectWords);
                printf("Correct letters: %d\n", stats->correctLetters);
                printf("Incorrect letters: %d\n", stats->incorrectLetters);
                printf("Typed letters: %d\n", stats->typedLetters);
                printf("Typed words: %d\n", stats->typedWords);
            }

            if (counter%60==0 && timer)
                timer--;

            break;
        default:
            break;
    }
    
}

void setGameState(GameState state) {
    currentState = state;
    gameStateChange = 1;
    if (state == GAME){
        stars = 3;
    }
}

void initialize_mouse_data() {
    mouse_pos.x = 500;
    mouse_pos.y = 300;
}

void update_mouse() {
    mouse_ih();
    sync();
    if (bytes_read == 3) {
        parse();
        checkActions();
        bytes_read = 0;
    }
}

void checkActions() {
    switch (currentState) {
        case MENU:
            if (mouse_pos.x >= startPlayX && mouse_pos.x <= endPlayX &&
                mouse_pos.y >= startPlayY && mouse_pos.y <= endPlayY &&
                pp.lb) {
                setGameState(TIMERS);
                initializeTest(&test, wordPool, 50, 30);
                initializeStats(&stats);
            }
            if (mouse_pos.x >= startInstructionsX && mouse_pos.x <= endInstructionsX &&
                mouse_pos.y >= startInstructionsY && mouse_pos.y <= endInstructionsY &&
                pp.lb) {
                setGameState(INSTRUCTIONS);
            }
            break;
        case TIMERS:
            if (mouse_pos.x >= startTimer15X && mouse_pos.x <= endTimer15X &&
                mouse_pos.y >= startTimer15Y && mouse_pos.y <= endTimer15Y &&
                pp.lb) {
                timer = 15;
                setGameState(GAME);
            }
            if (mouse_pos.x >= startTimer30X && mouse_pos.x <= endTimer30X &&
                mouse_pos.y >= startTimer30Y && mouse_pos.y <= endTimer30Y &&
                pp.lb) {
                timer = 30;
                setGameState(GAME);
            }
            if (mouse_pos.x >= startTimer60X && mouse_pos.x <= endTimer60X &&
                mouse_pos.y >= startTimer60Y && mouse_pos.y <= endTimer60Y &&
                pp.lb) {
                timer = 60;
                setGameState(GAME);
            }
            break;
        case GAME:
            checkGesture();
            break;
        case INSTRUCTIONS:
            break;
    }
}

void checkGesture() {
    if (stars){
        switch (gestureState){
            case GESTURE_ZERO:
                if (pp.lb)
                    gestureState = GESTURE_LB;
                break;
            case GESTURE_LB:
                if (!pp.lb){
                    gestureState =GESTURE_ZERO;
                    x_gesture = 0; // reset x_gesture
                } else { // lb maintained pressed
                    if (pp.delta_x > 0){
                        x_gesture+=pp.delta_x;
                    }
                    if (x_gesture > 300){ 
                        fill_current_word();
                        gestureState =GESTURE_ZERO;
                        x_gesture = 0; // reset x_gesture
                    }
                }
                break;
        }
    }
}

void fill_current_word() {
    Word *currentWord = &test->words[test->currentWordIndex];
    for (int i = test->currentInputIndex; i < currentWord->length; i++) {
        currentWord->letters[i].status = 1;  // Mark the remaining letters as correct
        test->currentInput[i] = currentWord->letters[i].character; // Fill the remaining letters in the input
    }
    test->currentInputIndex = currentWord->length;  // Update the input index
    drawWords(test);  // Update words on the screen

    stars--; // Decrement stars
}


Key char_to_key(char c) {
    if (c >= 'A' && c <= 'Z') {
        return (Key)(c - 'A' + 2);
    }
    switch (c) {
        case '0':
            return ZERO;
        case '1':
            return ONE;
        case '2':
            return TWO;
        case '3':
            return THREE;
        case '4':
            return FOUR;
        case '5':
            return FIVE;
        case '6':
            return SIX;
        case '7':
            return SEVEN;
        case '8':
            return EIGHT;
        case '9':
            return NINE;
        case ',':
            return COMMA;
        case '.':
            return PERIOD;
        case '!':
            return EXCLAMATION;
        case ':':
            return COLON;
        case ')':
            return RIGHT_PARENTHESIS;
        default:
            return NONE_KEY;
    }
    return NONE_KEY;
}

void key_handler() {
    /*printf("Current word %d: %.*s\n", currentWord->index, currentWord->length, currentWord->word);

    for (int i = 0; i < 100; i++) {
        if (typedWords[i].length > 0) {
            printf("Typed word %d: %s\n", i, typedWords[i].word);
        }
    }*/

    switch (currentState) {
        case MENU:
            update_keyboard(test);
            if (currentKey == ENTER) {
                setGameState(TIMERS);
                initializeTest(&test, wordPool, 50, 30);
            }
            break;
        case GAME:
            update_keyboard(test);
            if (currentKey == ESC) {
                setGameState(MENU);
                currentKey = NONE_KEY;
            }
            break;
        case INSTRUCTIONS:
            update_keyboard(test);
            if (currentKey == BACK) {
                setGameState(MENU);
            }
        case TIMERS:
            update_keyboard(test);
            if (currentKey == BACK) {
                setGameState(MENU);
            }
            break;
    }
}

void process_key(char c, Key key, TypingTest *test, GameState state) {
    if (state == GAME){
        Word *currentWord = &test->words[test->currentWordIndex];

        if (test->currentInputIndex == currentWord->length-1){
            deleteCaret();
        }
        if (test->currentInputIndex < currentWord->length) {
            stats->typedLetters++;
            currentWord->letters[test->currentInputIndex].status = 1;
            if (c != currentWord->letters[test->currentInputIndex].character) {
                currentWord->letters[test->currentInputIndex].status = -1;
                stats->incorrectLetters++;
                printf("Mistake\n");
            } else {
                stats->correctLetters++;
            }
            test->currentInputIndex++;
            drawWords(test);
        }
    }

    currentKey = key;
}


void handle_space_key(TypingTest *test) {

    Word *currentWord = &test->words[test->currentWordIndex];
    int isCorrect = 1;
    for (int i = 0; i < currentWord->length; i++) {
        if (currentWord->letters[i].status == -1 || currentWord->letters[i].status == 0) { 
            isCorrect = 0;
            break;
        }
    }

    stats->typedWords++;
    if (isCorrect) {
        currentWord->status = 1; 
        stats->correctWords++;
        printf("Correct word\n");
    } else {
        currentWord->status = -1; 
        stats->incorrectWords++;
        printf("Incorrect word\n");
    }

    int number_of_words_in_penultimate_line = 0;
    for (int i = 0; i < test->wordCount; i++) {
        if (test->words[i].line == test->number_of_lines - 1) {
            number_of_words_in_penultimate_line++;
        }
    }


    if (test->words[test->currentWordIndex].line == test->number_of_lines - 1) {
        int isLastWordOnLine = 1;
        for (int i = test->currentWordIndex + 1; i < test->wordCount; i++) {
            if (test->words[i].line == test->number_of_lines - 1) {
                isLastWordOnLine = 0;
                break;
            }

        }
        if (isLastWordOnLine) {
            shift_words_up(test);
            test->currentWordIndex = number_of_words_in_penultimate_line;
            test->currentInputIndex = 0;
            memset(test->currentInput, 0, MAX_WORD_LENGTH);
            drawWords(test);
            return;
        }
    }

    test->currentWordIndex++;
    test->currentInputIndex = 0;
    memset(test->currentInput, 0, MAX_WORD_LENGTH);

    drawWords(test);
}

void handle_delete_key(TypingTest *test) {
    Word *currentWord = &test->words[test->currentWordIndex];
    if (test->currentInputIndex > 0) {
        test->currentInputIndex--;
        currentWord->letters[test->currentInputIndex].status = 0;
    }
    drawWords(test);
}

void update_keyboard(TypingTest *test) {
    kbc_ih();
    if (!(data & BIT(7))) {
        Key curKey = keyMap[data];
        char currentChar = charMap[data];

        if (currentChar) {
            process_key(currentChar, curKey, test, currentState);
        } else {
            switch (data) {
                case KEY_SPACE:
                    if (currentState == GAME){
                        handle_space_key(test);
                    }
                    break;
                case KEY_ENTER:
                    currentKey = ENTER;
                    break;
                case KEY_DELETE:
                    currentKey = BACK;
                    if (currentState == GAME){
                        handle_delete_key(test);
                    }
                    break;
                case KEY_ESC:
                    currentKey = ESC;
                    break;
                default:
                    break;
            }
        }
    }
}


void destroy_sprites(){
    destroy_sprite(A_SPRITE);
    destroy_sprite(B_SPRITE);
    destroy_sprite(C_SPRITE);
    destroy_sprite(D_SPRITE);
    destroy_sprite(E_SPRITE);
    destroy_sprite(F_SPRITE);
    destroy_sprite(G_SPRITE);
    destroy_sprite(H_SPRITE);
    destroy_sprite(I_SPRITE);
    destroy_sprite(J_SPRITE);
    destroy_sprite(K_SPRITE);
    destroy_sprite(L_SPRITE);
    destroy_sprite(M_SPRITE);
    destroy_sprite(N_SPRITE);
    destroy_sprite(O_SPRITE);
    destroy_sprite(P_SPRITE);
    destroy_sprite(Q_SPRITE);
    destroy_sprite(R_SPRITE);
    destroy_sprite(S_SPRITE);
    destroy_sprite(T_SPRITE);
    destroy_sprite(U_SPRITE);
    destroy_sprite(V_SPRITE);
    destroy_sprite(W_SPRITE);
    destroy_sprite(X_SPRITE);
    destroy_sprite(Y_SPRITE);
    destroy_sprite(Z_SPRITE);
    destroy_sprite(CURSOR_SPRITE);

    destroy_sprite(STAR_SPRITE);

    destroy_sprite(PLAY_SPRITE);
    destroy_sprite(INSTRUCTIONS_SPRITE);

    destroy_sprite(TIMER15_SPRITE);
    destroy_sprite(TIMER30_SPRITE);
    destroy_sprite(TIMER60_SPRITE);

    destroy_sprite(COMMA_SPRITE);
    destroy_sprite(PERIOD_SPRITE);
    destroy_sprite(EXCLAMATION_SPRITE);
    destroy_sprite(COLON_SPRITE);
    destroy_sprite(RIGHT_PARENTHESIS_SPRITE);

    destroy_sprite(ZERO_SPRITE);
    destroy_sprite(ONE_SPRITE);
    destroy_sprite(TWO_SPRITE);
    destroy_sprite(THREE_SPRITE);
    destroy_sprite(FOUR_SPRITE);
    destroy_sprite(FIVE_SPRITE);
    destroy_sprite(SIX_SPRITE);
    destroy_sprite(SEVEN_SPRITE);
    destroy_sprite(EIGHT_SPRITE);
    destroy_sprite(NINE_SPRITE);
}

void destroy_test(){
    free(test->words);
    free(test);
}


void destroy_stats(){
    free(stats);
}

