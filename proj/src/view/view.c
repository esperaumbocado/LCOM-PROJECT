#include "view.h"
#include <math.h>
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
extern Sprite *BACK_TO_MENU_SPRITE;
extern Sprite *PLAY_AGAIN_SPRITE;

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
extern Statistics *stats;

uint32_t bg_color;

// screen box dimensions
extern int x_margin;
extern int y_margin;

extern int statisticsBoxX;
extern int statisticsBoxY;
extern int statisticsBoxSizeX;
extern int statisticsBoxSizeY;

extern Sprite *key_sprite_map[KEY_SPRITE_MAP_SIZE]; 

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

                drawStatic(PANDA_SPRITE);
                drawStatic(BAMBU_LEFT_SPRITE);
                drawStatic(BAMBU_RIGHT_SPRITE);
                drawStatic(PLAY_SPRITE);
                drawStatic(INSTRUCTIONS_SPRITE);

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
                drawText(game_instructions, GREY, x_margin, mode_info.XResolution-x_margin, 100);
                gameStateChange = 0;
            }
            drawCursor();
            break;
        case STATISTICS:
            if (gameStateChange) {
                draw_rectangle(statisticsBoxX, statisticsBoxY, statisticsBoxSizeX, statisticsBoxSizeY, WHITE, secondary_frame_buffer_no_mouse);
                drawText(statistics, GREY, statisticsBoxX, statisticsBoxX+statisticsBoxSizeX, statisticsBoxY);
                drawStatistics();
                drawStatic(BACK_TO_MENU_SPRITE);
                drawStatic(PLAY_AGAIN_SPRITE);
                gameStateChange = 0;
            }
            drawCursor();
            break;
    }

    return 0;
}

int drawStatic(Sprite *sprite){
    return drawSpriteXPM(sprite, sprite->x, sprite->y, false, 0, false);
}

int drawStatistics() {
    int accuracy = round(((double)stats->correctWords / stats->typedWords) * 100);
    printf("Accuracy: %d\n", accuracy);
    int speed = round((double)stats->typedWords / (stats->time / 60.0));
    printf("Speed: %d\n", speed);
    if (drawInt(accuracy, statisticsBoxX + 368, statisticsBoxY + 110)) return 1;
    printf("Drawn accuracy\n");
    if (drawInt(speed, statisticsBoxX + 322, statisticsBoxY + 132)) return 1;
    printf("Drawn speed\n");
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

            if (drawSpriteXPM(key_sprite_map[key], x, y, false, 0, true)) 
                return 1;
            
            
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
        if (drawSpriteXPM(key_sprite_map[key], x, y, false, 0, true)) return 1;
        
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

    if (drawText(timer_selection, WHITE,x_margin,mode_info.XResolution-x_margin,y_margin)) return 1;

    if (drawSpriteXPM(TIMER15_SPRITE, TIMER15_SPRITE->x, TIMER15_SPRITE->y, false, 0, false)) return 1;
    if (drawSpriteXPM(TIMER30_SPRITE, TIMER30_SPRITE->x, TIMER30_SPRITE->y, false, 0, false)) return 1;
    if (drawSpriteXPM(TIMER60_SPRITE, TIMER60_SPRITE->x, TIMER60_SPRITE->y, false, 0, false)) return 1;

    return 0;
}


int drawInt(int number, int x, int y) {
    printf("Entering drawInt with number: %d\n", number);
    
    char number_string[12];
    sprintf(number_string, "%d", number);
    printf("Number string: %s\n", number_string);

    char *number_string_ptr = number_string;
    printf("Drawing number %d at (%d, %d)\n", number, x, y);

    while (*number_string_ptr) {
        printf("Processing character: %c\n", *number_string_ptr);
        Key key = char_to_key(*number_string_ptr);
        printf("Character '%c' converted to key %d\n", *number_string_ptr, key);
        if (key == NONE_KEY) {
            printf("Invalid key for character '%c'\n", *number_string_ptr);
            return 1;
        }

        if (drawSpriteXPM(key_sprite_map[key], x, y, false, 0, false))
            return 1;

        number_string_ptr++;
        x += 16;
    }

    printf("Exiting drawInt successfully\n");
    return 0;
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

int drawText(const char* text, uint32_t color, int start_x, int end_x, int start_y) {

    x_offset = start_x;
    y_offset = start_y;

    while (*text) {    

        if (*text == '\n') {
            x_offset = start_x; // new line
            y_offset += 22;  
            text++; 
            continue;
        }

        Key key = char_to_key(*text);
        
        if (key_sprite_map[key])
            if (drawSpriteXPM(key_sprite_map[key], x_offset, y_offset, true, color, false)) 
                return 1;

        offset_handler(start_x, end_x);
        text++;
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
                if (drawSpriteXPM(key_sprite_map[key], x_offset, y_offset, true, 0x336600, false)) return 1;
            }else if (currentWord->status == -1){
                if (drawSpriteXPM(key_sprite_map[key], x_offset, y_offset, true, RED, false)) return 1;
            }else if (currentWord->letters[j].status == -1) {
                if (drawSpriteXPM(key_sprite_map[key], x_offset, y_offset, true, RED, false)) return 1;
            } else if (currentWord->letters[j].status == 1) {
                if (drawSpriteXPM(key_sprite_map[key], x_offset, y_offset, true, WHITE, false)) return 1;
            } else {
                if (drawSpriteXPM(key_sprite_map[key], x_offset, y_offset, true, GREY, false)) return 1;
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
