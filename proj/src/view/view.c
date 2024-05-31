#include "view.h"
#include "messages.h"
#include "../drivers/rtc/rtc.h"

extern uint8_t *main_frame_buffer;
extern uint8_t *secondary_frame_buffer;
extern uint8_t *secondary_frame_buffer_no_mouse;
uint32_t frame_size;
extern Key currentKey;
extern int x_offset;
extern int y_offset;


extern int gameStateChange;
extern GameState currentState;
extern real_time_info time_info;
// TIMER VARIABLE

//CARET
extern Caret caret;


extern int timer;

extern mouse_position mouse_pos;

extern int stars;


extern Sprite *CURSOR_SPRITE;

extern Sprite *STAR_SPRITE;

extern Sprite *PLAY_SPRITE;
extern Sprite *INSTRUCTIONS_SPRITE;

extern Sprite *TIMER15_SPRITE;
extern Sprite *TIMER30_SPRITE;
extern Sprite *TIMER60_SPRITE;

extern int startPandaX;
extern int startPandaY;

extern int startPlayX;
extern int startPlayY;

extern int startInstructionsX;
extern int startInstructionsY;

extern int startTimer15X;
extern int startTimer15Y;

extern int startTimer30X;
extern int startTimer30Y;

extern int startTimer60X;
extern int startTimer60Y;

// Box dimensions
extern int startBoxX;
extern int startBoxY;
extern int sizeBoxX;
extern int sizeBoxY;

extern Sprite *A_SPRITE;
extern Sprite *B_SPRITE;
extern Sprite *C_SPRITE;
extern Sprite *D_SPRITE;
extern Sprite *E_SPRITE;
extern Sprite *F_SPRITE;
extern Sprite *G_SPRITE;
extern Sprite *H_SPRITE;
extern Sprite *I_SPRITE;
extern Sprite *J_SPRITE;
extern Sprite *K_SPRITE;
extern Sprite *L_SPRITE;
extern Sprite *M_SPRITE;
extern Sprite *N_SPRITE;
extern Sprite *O_SPRITE;
extern Sprite *P_SPRITE;
extern Sprite *Q_SPRITE;
extern Sprite *R_SPRITE;
extern Sprite *S_SPRITE;
extern Sprite *T_SPRITE;
extern Sprite *U_SPRITE;
extern Sprite *V_SPRITE;
extern Sprite *W_SPRITE;
extern Sprite *X_SPRITE;
extern Sprite *Y_SPRITE;
extern Sprite *Z_SPRITE;

extern Sprite *COMMA_SPRITE;
extern Sprite *PERIOD_SPRITE;

extern Sprite *EXCLAMATION_SPRITE;
extern Sprite *COLON_SPRITE;
extern Sprite *RIGHT_PARENTHESIS_SPRITE;

extern Sprite *ZERO_SPRITE;
extern Sprite *ONE_SPRITE;
extern Sprite *TWO_SPRITE;
extern Sprite *THREE_SPRITE;
extern Sprite *FOUR_SPRITE;
extern Sprite *FIVE_SPRITE;
extern Sprite *SIX_SPRITE;
extern Sprite *SEVEN_SPRITE;
extern Sprite *EIGHT_SPRITE;
extern Sprite *NINE_SPRITE;

extern Sprite* PANDA_SPRITE;
extern TypingTest *test;

uint32_t bg_color;


// screen box dimensions
extern int startBoxX;
extern int startBoxY;
extern int sizeBoxX;
extern int sizeBoxY;

int setUpFrameBuffer() {
    if (set_frame_buffer(0x14C) != 0) return 1;
    frame_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
    secondary_frame_buffer = (uint8_t *) malloc(frame_size);
    secondary_frame_buffer_no_mouse = (uint8_t *) malloc(frame_size);

    sizeBoxX = mode_info.XResolution - 400;
    sizeBoxY = mode_info.YResolution - 400;

    return 0;
}


int drawSpriteXPM(Sprite *sprite, int x, int y) {
    if (sprite == NULL) return 1;

    uint16_t width = sprite->width;
    uint16_t height = sprite->height;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            uint32_t color = sprite->colors[col + row * width];

            if (color == TRANSPARENT) {
                continue;
            }

            if (draw_pixel(x + col, y + row, color, secondary_frame_buffer_no_mouse) != 0) {
                return 1;
            }
        }
    }
    
    return 0;
}

int drawSpriteXPM_single_color(Sprite *sprite, int x, int y, uint32_t color) {
    if (sprite == NULL) return 1;

    uint16_t width = sprite->width;
    uint16_t height = sprite->height;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            if (sprite->colors[col + row * width] != TRANSPARENT) {
                if (draw_pixel(x + col, y + row, color, secondary_frame_buffer_no_mouse) != 0) {
                    return 1;
                }
            }
        }
    }
    
    return 0;
}


int drawSpriteXPM_mouse(Sprite *sprite, int x, int y) {
    if (sprite == NULL) return 1;

    uint16_t width = sprite->width;
    uint16_t height = sprite->height;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            uint32_t color = sprite->colors[col + row * width];

            if (color == TRANSPARENT) {
                continue;
            }

            if (draw_pixel(x + col, y + row, color, secondary_frame_buffer) != 0) {
                return 1;
            }
        }
    }
    
    return 0;
}

int GameDrawer(){
    switch(currentState){
        case MENU:
            if (gameStateChange){
                drawBackground(MENU);
                drawSpriteXPM(PANDA_SPRITE, startPandaX, startPandaY);
                drawSpriteXPM(PLAY_SPRITE, startPlayX, startPlayY);
                drawSpriteXPM(INSTRUCTIONS_SPRITE, startInstructionsX, startInstructionsY);
                gameStateChange = 0;
            }
            drawCursor();
            break;
        case GAME:
            if (gameStateChange){
                drawBackground(GAME);
                drawRecordedTime(); 
                gameStateChange = 0;
                drawWords(test);
            }
            drawRecordedTime(); 
            drawStars();
            drawCursor();
            break;
        case TIMERS:
            if (gameStateChange){
                drawBackground(TIMERS);
                drawTimers();
                gameStateChange = 0;
            }
            drawCursor();
            break;
        case INSTRUCTIONS:
            if (gameStateChange) {
                drawBackground(INSTRUCTIONS);
                drawText(game_instructions, GREY);
                gameStateChange = 0;
            }
            drawCursor();
            break;
    }

    return 0;
}

int drawBackground(GameState state) {
    if (rtc_read_time(&time_info) != 0) return 1;

    if (time_info.hours >= 6 && time_info.hours < 12) {
        bg_color = COLOR_MORNING;
    } else if (time_info.hours >= 12 && time_info.hours < 18) {
        bg_color = COLOR_AFTERNOON;
    } else if (time_info.hours >= 18 && time_info.hours < 21) {
        bg_color = COLOR_EVENING;
    } else {
        bg_color = COLOR_NIGHT;
    }

    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, bg_color, secondary_frame_buffer_no_mouse);

    if(state == GAME){
        draw_rectangle(startBoxX, startBoxY, sizeBoxX, sizeBoxY, BEIGE, secondary_frame_buffer_no_mouse);
    }else if(state == INSTRUCTIONS){
        draw_rectangle(100, 100, mode_info.XResolution - 200, mode_info.YResolution - 200, BEIGE, secondary_frame_buffer_no_mouse);
    }

    return 0;
}

int drawRecordedTime(){

    int x = 10;
    int y = mode_info.YResolution - 40;

    char timer_string[5];
    sprintf(timer_string, "%d", timer);

    char *timer_string_ptr = timer_string;
    
    while (*timer_string_ptr) { 

        Key key = char_to_key(*timer_string_ptr);
        if (drawNumber(key, x, y)) return 1;

        timer_string_ptr++;
        x+=13;
    }

    return 0;
}

int drawStars(){

    for (int i = 0; i < stars; i++) 
        if (drawSpriteXPM_mouse(STAR_SPRITE, 10 + i*30, 10)) return 1;
    
    return 0;
}

int drawTimers() {

    if (drawText(timer_selection, GREY)) return 1;

    if (drawSpriteXPM(TIMER15_SPRITE, startTimer15X, startTimer15Y)) return 1;
    if (drawSpriteXPM(TIMER30_SPRITE, startTimer30X, startTimer30Y)) return 1;
    if (drawSpriteXPM(TIMER60_SPRITE, startTimer60X, startTimer60Y)) return 1;

    return 0;
}

int drawNumber(Key key, int x, int y) {
    switch(key){
        case ZERO:
            return drawSpriteXPM_mouse(ZERO_SPRITE, x, y);
        case ONE:
            return drawSpriteXPM_mouse(ONE_SPRITE, x, y);
        case TWO:
            return drawSpriteXPM_mouse(TWO_SPRITE, x, y);
        case THREE:
            return drawSpriteXPM_mouse(THREE_SPRITE, x, y);
        case FOUR:
            return drawSpriteXPM_mouse(FOUR_SPRITE, x, y);
        case FIVE:
            return drawSpriteXPM_mouse(FIVE_SPRITE, x, y);
        case SIX:
            return drawSpriteXPM_mouse(SIX_SPRITE, x, y);
        case SEVEN:
            return drawSpriteXPM_mouse(SEVEN_SPRITE, x, y);
        case EIGHT: 
            return drawSpriteXPM_mouse(EIGHT_SPRITE, x, y);
        case NINE:
            return drawSpriteXPM_mouse(NINE_SPRITE, x, y);
        default:
            printf("Key does not represent a number\n");
            break;
    }

    return 1;
}

int drawCursor(){

    if (mouse_pos.x < 0) mouse_pos.x = 0;
    if (mouse_pos.x > mode_info.XResolution - CURSOR_SPRITE->width){
        mouse_pos.x = mode_info.XResolution - CURSOR_SPRITE->width;
    }

    if (mouse_pos.y < 0) mouse_pos.y = 0;
    if (mouse_pos.y > mode_info.YResolution - CURSOR_SPRITE->height){
        mouse_pos.y = mode_info.YResolution - CURSOR_SPRITE->height;
    }

    return drawSpriteXPM_mouse(CURSOR_SPRITE, mouse_pos.x, mouse_pos.y);
}

int drawText(const char* text, uint32_t color) {

    reset_offset();

    while (*text) {    

        if (*text == '\n') {
            x_offset = 100; // new line
            y_offset += 20;  
            text++; 
            continue;
        }

        Key key = char_to_key(*text);
        if (drawLetter(key, color)) return 1;
        offset_handler(0);
        text++;
    }

    return 0;
}

int drawLetter(Key key, uint32_t color) {
    switch(key){
        case A:
            return drawSpriteXPM_single_color(A_SPRITE, x_offset, y_offset, color);
        case B:
            return drawSpriteXPM_single_color(B_SPRITE, x_offset, y_offset, color);
        case C:
            return drawSpriteXPM_single_color(C_SPRITE, x_offset, y_offset, color);
        case D:
            return drawSpriteXPM_single_color(D_SPRITE, x_offset, y_offset, color);
        case E:
            return drawSpriteXPM_single_color(E_SPRITE, x_offset, y_offset, color);
        case F:
            return drawSpriteXPM_single_color(F_SPRITE, x_offset, y_offset, color);
        case G:
            return drawSpriteXPM_single_color(G_SPRITE, x_offset, y_offset, color);
        case H:
            return drawSpriteXPM_single_color(H_SPRITE, x_offset, y_offset, color);
        case I:
            return drawSpriteXPM_single_color(I_SPRITE, x_offset, y_offset, color);
        case J:
            return drawSpriteXPM_single_color(J_SPRITE, x_offset, y_offset, color);
        case K:
            return drawSpriteXPM_single_color(K_SPRITE, x_offset, y_offset, color);
        case L:
            return drawSpriteXPM_single_color(L_SPRITE, x_offset, y_offset, color);
        case M:
            return drawSpriteXPM_single_color(M_SPRITE, x_offset, y_offset, color);
        case N:
            return drawSpriteXPM_single_color(N_SPRITE, x_offset, y_offset, color);
        case O:
            return drawSpriteXPM_single_color(O_SPRITE, x_offset, y_offset, color);
        case P:
            return drawSpriteXPM_single_color(P_SPRITE, x_offset, y_offset, color);
        case Q:
            return drawSpriteXPM_single_color(Q_SPRITE, x_offset, y_offset, color);
        case R:
            return drawSpriteXPM_single_color(R_SPRITE, x_offset, y_offset, color);
        case S:
            return drawSpriteXPM_single_color(S_SPRITE, x_offset, y_offset, color);
        case T:
            return drawSpriteXPM_single_color(T_SPRITE, x_offset, y_offset, color);
        case U:
            return drawSpriteXPM_single_color(U_SPRITE, x_offset, y_offset, color);
        case V:
            return drawSpriteXPM_single_color(V_SPRITE, x_offset, y_offset, color);
        case W:
            return drawSpriteXPM_single_color(W_SPRITE, x_offset, y_offset, color);
        case X:
            return drawSpriteXPM_single_color(X_SPRITE, x_offset, y_offset, color);
        case Y:
            return drawSpriteXPM_single_color(Y_SPRITE, x_offset, y_offset, color);
        case Z:
            return drawSpriteXPM_single_color(Z_SPRITE, x_offset, y_offset, color);
        case COMMA:
            return drawSpriteXPM_single_color(COMMA_SPRITE, x_offset, y_offset, color);
        case PERIOD:
            return drawSpriteXPM_single_color(PERIOD_SPRITE, x_offset, y_offset, color);
        case EXCLAMATION:
            return drawSpriteXPM_single_color(EXCLAMATION_SPRITE, x_offset, y_offset, color);
        case COLON:
            return drawSpriteXPM_single_color(COLON_SPRITE, x_offset, y_offset, color);
        case RIGHT_PARENTHESIS:
            return drawSpriteXPM_single_color(RIGHT_PARENTHESIS_SPRITE, x_offset, y_offset, color);
        default:
            break;
    }

    return 0;
}

int deleteCaret() {
    if (draw_rectangle(caret.x, caret.y, caret.width, caret.height, BEIGE, secondary_frame_buffer_no_mouse)) return 1;
    return 0;
}

int drawCaret(int x,int y){
    if (draw_rectangle(x, y, 13, 1, 0xFFFFFF, secondary_frame_buffer_no_mouse)) return 1;
    caret.height = 1;
    caret.width = 13;
    caret.x = x;
    caret.y = y;
    return 0;
}
    
int drawWords(TypingTest *test) {

    reset_offset();
    int current_line = 0;

    for (int i = 0; i < test->wordCount; i++) {
        Word *currentWord = &(test->words[i]);
        currentWord->x = x_offset;
        currentWord->y = y_offset;
        currentWord->line = current_line;

        for (int j = 0; j < currentWord->length; j++) {
            Key key = char_to_key(currentWord->letters[j].character);

            if (i == test->currentWordIndex && j == test->currentInputIndex) {
                if(deleteCaret()!=0) return 1;
                if(drawCaret(x_offset, y_offset+24)!=0) return 1;
            }

            if (currentWord->status == 1){
                if (drawLetter(key, 0x336600)) return 1;
            }else if (currentWord->status == -1){
                if (drawLetter(key, RED)) return 1;
            }else if (currentWord->letters[j].status == -1) {
                if (drawLetter(key, RED)) return 1;  // Red 
            } else if (currentWord->letters[j].status == 1) {
                if (drawLetter(key, 0xFFFFFF)) return 1;  // WHITE
            } else {
                if (drawLetter(key, GREY)) return 1; // Grey
            }

            x_offset += 16;
        }

        if (i == (test->wordCount - 1)) {
            break; 
        }

        if (i != (test->wordCount - 1)) {
            Word *nextWord = &(test->words[i+1]);
            if (x_offset + word_length_in_pixels(nextWord) > mode_info.XResolution - startBoxX - 1) {
                x_offset = startBoxX;
                y_offset += 32;
                current_line++;
            }else{
                x_offset += 16;
            }
        }
    }

    test->number_of_lines = current_line;
    return 0;
}


int word_length_in_pixels(Word *word) {
    return word->length*16;
}
