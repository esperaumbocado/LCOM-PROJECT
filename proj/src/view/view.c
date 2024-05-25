#include "view.h"
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

extern int recorded_time;
extern bool recorded_time_has_changed;

extern mouse_position mouse_pos;

extern Sprite *CURSOR_SPRITE;
extern Sprite *PLAY_SPRITE;

extern int startPlayX;
extern int startPlayY;

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


int setUpFrameBuffer() {
    if (set_frame_buffer(0x14C) != 0) return 1;
    frame_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
    secondary_frame_buffer = (uint8_t *) malloc(frame_size);
    secondary_frame_buffer_no_mouse = (uint8_t *) malloc(frame_size);
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
            drawBackground();
            stopRecordingTime();
            drawRecordedTime();
            drawSpriteXPM(PLAY_SPRITE, startPlayX, startPlayY);
            gameStateChange = 0;
        }
            drawCursor();
            break;
        case GAME:
            if (gameStateChange){
                drawBackground();
                startRecordingTime();
                drawRecordedTime();
                gameStateChange = 0;
            }
            if (recorded_time_has_changed) 
                drawRecordedTime(); 
            drawCursor();
            break;
        case NONE_STATE:
            break;
    }

    return 0;
}

int drawBackground() {
    if (rtc_read_time(&time_info) != 0) return 1;

    uint32_t bg_color;

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
    return 0;
}

int drawRecordedTime(){

    int x = 10;
    int y = mode_info.YResolution - 40;

    char recorded_time_string[5];
    sprintf(recorded_time_string, "%d", recorded_time);

    char *recorded_time_string_ptr = recorded_time_string;

    while (*recorded_time_string_ptr) { 

        Key key = char_to_key(*recorded_time_string_ptr);
        if (drawNumber(key, x, y)) return 1;

        recorded_time_string_ptr++;
        x+=13;
    }

    return 0;
}

int drawNumber(Key key, int x, int y) {
    switch(key){
        case ZERO:
            return drawSpriteXPM(ZERO_SPRITE, x, y);
        case ONE:
            return drawSpriteXPM(ONE_SPRITE, x, y);
        case TWO:
            return drawSpriteXPM(TWO_SPRITE, x, y);
        case THREE:
            return drawSpriteXPM(THREE_SPRITE, x, y);
        case FOUR:
            return drawSpriteXPM(FOUR_SPRITE, x, y);
        case FIVE:
            return drawSpriteXPM(FIVE_SPRITE, x, y);
        case SIX:
            return drawSpriteXPM(SIX_SPRITE, x, y);
        case SEVEN:
            return drawSpriteXPM(SEVEN_SPRITE, x, y);
        case EIGHT: 
            return drawSpriteXPM(EIGHT_SPRITE, x, y);
        case NINE:
            return drawSpriteXPM(NINE_SPRITE, x, y);
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

int drawText(const char* text) {

    while (*text) {    
        Key key = char_to_key(*text);
        if (drawLetter(key,0x00FF00)) return 1;
        offset_handler(0);
        text++;
    }

    // Begin writing in a new line
    x_offset=0;
    y_offset+=20;

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
            return drawSpriteXPM(COMMA_SPRITE, x_offset, y_offset);
        case PERIOD:
            return drawSpriteXPM(PERIOD_SPRITE, x_offset, y_offset);
        default:
            break;
    }

    return 0;
}

int drawWords(TypingTest *test) {
    x_offset = 0;
    y_offset = 0;
    for (int i = 0; i < test->wordCount; i++) {
        Word *currentWord = &(test->words[i]);
        int word_status = test->words[i].status; 

        for (int j = 0; j < test->words[i].length; j++) {
            Key key = char_to_key(test->words[i].letters[j].character);
            if (word_status == 1) {
                if (drawLetter(key, 0x00FF00)) return 1;
            } else if (word_status == -1) {
                if (drawLetter(key, 0xFF0000)) return 1;
            } else {
                if (drawLetter(key, 0xFFFFFF)) return 1;
            }

            offset_handler(0);
        }

        if (i == (test->wordCount - 1)) {
            break; 
        }

        offset_handler(0);

        if (x_offset + word_length_in_pixels(currentWord) > mode_info.XResolution) {
            x_offset = 0;
            y_offset += 20;
        }
    }

    return 0;
}


int word_length_in_pixels(Word *word) {
    return word->length*13;
}
