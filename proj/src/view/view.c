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

extern int stars;

extern Sprite *CURSOR_SPRITE;

extern Sprite *STAR_SPRITE;

extern Sprite *PLAY_SPRITE;
extern Sprite *INSTRUCTIONS_SPRITE;

extern Sprite *TIMER15_SPRITE;
extern Sprite *TIMER30_SPRITE;
extern Sprite *TIMER60_SPRITE;


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
extern Sprite* BAMBU_LEFT_SPRITE;
extern Sprite* BAMBU_RIGHT_SPRITE;
extern TypingTest *test;

uint32_t bg_color;

// screen box dimensions
extern int x_margin;
extern int y_margin;

int setUpFrameBuffer() {
    if (set_frame_buffer(0x14C) != 0) return 1;
    frame_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
    secondary_frame_buffer = (uint8_t *) malloc(frame_size);
    secondary_frame_buffer_no_mouse = (uint8_t *) malloc(frame_size);

    return 0;
}


int drawSpriteXPM(Sprite *sprite, int x, int y, bool single_color, uint32_t color, bool moving){
    if (sprite == NULL) return 1;

    uint16_t width = sprite->width;
    uint16_t height = sprite->height;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            
            uint32_t sprite_color = sprite->colors[col + row * width];

            if (sprite_color != TRANSPARENT) {
                if (!single_color) color = sprite_color; // reassign color to the sprite color
                
                if (moving) 
                    draw_pixel(x + col, y + row, color, secondary_frame_buffer);
                else 
                    draw_pixel(x + col, y + row, color, secondary_frame_buffer_no_mouse);

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
                drawSpriteXPM(PANDA_SPRITE, PANDA_SPRITE->x, PANDA_SPRITE->y, false, 0, false);
                drawSpriteXPM(BAMBU_RIGHT_SPRITE, BAMBU_RIGHT_SPRITE->x, BAMBU_RIGHT_SPRITE->y, false, 0, false);
                drawSpriteXPM(BAMBU_LEFT_SPRITE, BAMBU_LEFT_SPRITE->x, BAMBU_LEFT_SPRITE->y, false, 0, false);
                drawSpriteXPM(PLAY_SPRITE, PLAY_SPRITE->x, PLAY_SPRITE->y, false, 0, false);
                drawSpriteXPM(INSTRUCTIONS_SPRITE, INSTRUCTIONS_SPRITE->x, INSTRUCTIONS_SPRITE->y, false, 0, false);
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
                drawRealTime();
            }
            drawRealTime();
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

int drawRealTime() {
    if (rtc_read_time(&time_info) != 0) {
        printf("Failed to read RTC time\n");
        return 1;
    } else {
        int x = mode_info.XResolution - 13 * 8 - 10; // 8 characters (HH:MM:SS) * 13 pixels per character
        int y = mode_info.YResolution - 40; // 40 pixels from the bottom

        char time_string[9]; // HH:MM:SS
        sprintf(time_string, "%02d:%02d:%02d", time_info.hours, time_info.minutes, time_info.seconds);

        char *time_string_ptr = time_string;

        while (*time_string_ptr) {
            Key key = char_to_key(*time_string_ptr);
            if (drawNumber(key, x, y, BLACK)) {
                if (drawSpriteXPM(COLON_SPRITE, x, y, true, BLACK, true)) return 1;
            }
            
            time_string_ptr++;
            x += 13; // Move to the next position
        }

        // printf("Current time: %02d:%02d:%02d, Date: %02d/%02d/%02d\n",
        //        time_info.hours, time_info.minutes, time_info.seconds,
        //        time_info.day, time_info.month, time_info.year % 100);
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

    if(state == GAME || state == INSTRUCTIONS)
       draw_rectangle(100, 100, mode_info.XResolution - 200, mode_info.YResolution - 200, BEIGE, secondary_frame_buffer_no_mouse);
    

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
        if (drawNumber(key, x, y, BLACK)) return 1;

        timer_string_ptr++;
        x+=13;
    }

    return 0;
}

int drawStars(){

    for (int i = 0; i < stars; i++) 
        if (drawSpriteXPM(STAR_SPRITE, STAR_SPRITE->x + i*30, STAR_SPRITE->y, false, 0, true)) return 1;
    
    return 0;
}

int drawTimers() {

    if (drawText(timer_selection, WHITE)) return 1;

    if (drawSpriteXPM(TIMER15_SPRITE, TIMER15_SPRITE->x, TIMER15_SPRITE->y, false, 0, false)) return 1;
    if (drawSpriteXPM(TIMER30_SPRITE, TIMER30_SPRITE->x, TIMER30_SPRITE->y, false, 0, false)) return 1;
    if (drawSpriteXPM(TIMER60_SPRITE, TIMER60_SPRITE->x, TIMER60_SPRITE->y, false, 0, false)) return 1;

    return 0;
}

int drawNumber(Key key, int x, int y, uint32_t color) {
    switch(key){
        case ZERO:
            return drawSpriteXPM(ZERO_SPRITE, x, y, true, color, true);
        case ONE:
            return drawSpriteXPM(ONE_SPRITE, x, y, true, color, true);
        case TWO:
            return drawSpriteXPM(TWO_SPRITE, x, y, true, color, true);
        case THREE:
            return drawSpriteXPM(THREE_SPRITE, x, y, true, color, true);
        case FOUR:
            return drawSpriteXPM(FOUR_SPRITE, x, y, true, color, true);
        case FIVE:
            return drawSpriteXPM(FIVE_SPRITE, x, y, true, color, true);
        case SIX:
            return drawSpriteXPM(SIX_SPRITE, x, y, true, color, true);
        case SEVEN:
            return drawSpriteXPM(SEVEN_SPRITE, x, y, true, color, true);
        case EIGHT: 
            return drawSpriteXPM(EIGHT_SPRITE, x, y, true, color, true);
        case NINE:
            return drawSpriteXPM(NINE_SPRITE, x, y, true, color, true);
        default:
            break;
    }

    return 1;
}

int drawCursor(){

    if (CURSOR_SPRITE->x < 0) CURSOR_SPRITE->x = 0;
    if (CURSOR_SPRITE->x > mode_info.XResolution - CURSOR_SPRITE->width){
        CURSOR_SPRITE->x = mode_info.XResolution - CURSOR_SPRITE->width;
    }

    if (CURSOR_SPRITE->y < 0) CURSOR_SPRITE->y = 0;
    if (CURSOR_SPRITE->y > mode_info.YResolution - CURSOR_SPRITE->height){
        CURSOR_SPRITE->y = mode_info.YResolution - CURSOR_SPRITE->height;
    }

    return drawSpriteXPM(CURSOR_SPRITE, CURSOR_SPRITE->x, CURSOR_SPRITE->y, false, 0, true);
}

int drawText(const char* text, uint32_t color) {

    x_offset = 200;
    y_offset = mode_info.YResolution/2 - 300;

    while (*text) {    

        if (*text == '\n') {
            x_offset = 100; // new line
            y_offset += 22;  
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
            return drawSpriteXPM(A_SPRITE, x_offset, y_offset, true, color, false);
        case B:
            return drawSpriteXPM(B_SPRITE, x_offset, y_offset, true, color, false);
        case C:
            return drawSpriteXPM(C_SPRITE, x_offset, y_offset, true, color, false);
        case D:
            return drawSpriteXPM(D_SPRITE, x_offset, y_offset, true, color, false);
        case E:
            return drawSpriteXPM(E_SPRITE, x_offset, y_offset, true, color, false);
        case F:
            return drawSpriteXPM(F_SPRITE, x_offset, y_offset, true, color, false);
        case G:
            return drawSpriteXPM(G_SPRITE, x_offset, y_offset, true, color, false);
        case H:
            return drawSpriteXPM(H_SPRITE, x_offset, y_offset, true, color, false);
        case I:
            return drawSpriteXPM(I_SPRITE, x_offset, y_offset, true, color, false);
        case J:
            return drawSpriteXPM(J_SPRITE, x_offset, y_offset, true, color, false);
        case K:
            return drawSpriteXPM(K_SPRITE, x_offset, y_offset, true, color, false);
        case L:
            return drawSpriteXPM(L_SPRITE, x_offset, y_offset, true, color, false);
        case M:
            return drawSpriteXPM(M_SPRITE, x_offset, y_offset, true, color, false);
        case N:
            return drawSpriteXPM(N_SPRITE, x_offset, y_offset, true, color, false);
        case O:
            return drawSpriteXPM(O_SPRITE, x_offset, y_offset, true, color, false);
        case P:
            return drawSpriteXPM(P_SPRITE, x_offset, y_offset, true, color, false);
        case Q:
            return drawSpriteXPM(Q_SPRITE, x_offset, y_offset, true, color, false);
        case R:
            return drawSpriteXPM(R_SPRITE, x_offset, y_offset, true, color, false);
        case S:
            return drawSpriteXPM(S_SPRITE, x_offset, y_offset, true, color, false);
        case T:
            return drawSpriteXPM(T_SPRITE, x_offset, y_offset, true, color, false);
        case U:
            return drawSpriteXPM(U_SPRITE, x_offset, y_offset, true, color, false);
        case V:
            return drawSpriteXPM(V_SPRITE, x_offset, y_offset, true, color, false);
        case W:
            return drawSpriteXPM(W_SPRITE, x_offset, y_offset, true, color, false);
        case X:
            return drawSpriteXPM(X_SPRITE, x_offset, y_offset, true, color, false);
        case Y:
            return drawSpriteXPM(Y_SPRITE, x_offset, y_offset, true, color, false);
        case Z:
            return drawSpriteXPM(Z_SPRITE, x_offset, y_offset, true, color, false);
        case COMMA:
            return drawSpriteXPM(COMMA_SPRITE, x_offset, y_offset, true, color, false);
        case PERIOD:
            return drawSpriteXPM(PERIOD_SPRITE, x_offset, y_offset, true, color, false);
        case EXCLAMATION:
            return drawSpriteXPM(EXCLAMATION_SPRITE, x_offset, y_offset, true, color, false);
        case COLON:
            return drawSpriteXPM(COLON_SPRITE, x_offset, y_offset, true, color, false);
        case RIGHT_PARENTHESIS:
            return drawSpriteXPM(RIGHT_PARENTHESIS_SPRITE, x_offset, y_offset, true, color, false);
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
                if (drawLetter(key, RED)) return 1;  
            } else if (currentWord->letters[j].status == 1) {
                if (drawLetter(key, WHITE)) return 1; 
            } else {
                if (drawLetter(key, GREY)) return 1; 
            }

            x_offset += 16;
        }

        if (i == (test->wordCount - 1)) {
            break; 
        }

        if (i != (test->wordCount - 1)) {
            Word *nextWord = &(test->words[i+1]);
            if (x_offset + word_length_in_pixels(nextWord) > mode_info.XResolution - x_margin - 1) {
                x_offset = x_margin;
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
