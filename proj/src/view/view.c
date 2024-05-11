#include "view.h"

extern uint8_t *main_frame_buffer;
extern uint8_t *secondary_frame_buffer;
extern uint8_t *secondary_frame_buffer_no_mouse;
uint32_t frame_size;
extern Key currentKey;
extern int x_offset;
extern int y_offset;
extern int gameStateChange;
extern GameState currentState;

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
            drawSpriteXPM(PLAY_SPRITE, startPlayX, startPlayY);
            gameStateChange = 0;
        }
            drawCursor();
            break;
        case GAME:
            if (gameStateChange){
                drawBackground();
                drawText(text);
                gameStateChange = 0;
            }
            drawCursor();
            break;
        case NONE_STATE:
            break;
    }

    return 0;
}

int drawBackground() {
    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, WHITE, secondary_frame_buffer_no_mouse);
    return 0;
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
        if (drawLetter(key)) return 1;
        offset_handler(0);
        text++;
    }

    // Begin writing in a new line
    x_offset=0;
    y_offset+=20;

    return 0;
}

int drawLetter(Key key) {
    switch(key){
        case A:
            return drawSpriteXPM(A_SPRITE, x_offset, y_offset);
            break;
        case B:
            return drawSpriteXPM(B_SPRITE, x_offset, y_offset);
            break;
        case C:
            return drawSpriteXPM(C_SPRITE, x_offset, y_offset);
            break;
        case D:
            return drawSpriteXPM(D_SPRITE, x_offset, y_offset);
            break;
        case E:
            return drawSpriteXPM(E_SPRITE, x_offset, y_offset);
            break;
        case F:
            return drawSpriteXPM(F_SPRITE, x_offset, y_offset);
            break;
        case G:
            return drawSpriteXPM(G_SPRITE, x_offset, y_offset);
            break;
        case H:
            return drawSpriteXPM(H_SPRITE, x_offset, y_offset);
            break;
        case I:
            return drawSpriteXPM(I_SPRITE, x_offset, y_offset);
            break;
        case J:
            return drawSpriteXPM(J_SPRITE, x_offset, y_offset);
            break;
        case K:
            return drawSpriteXPM(K_SPRITE, x_offset, y_offset);
            break;
        case L:
            return drawSpriteXPM(L_SPRITE, x_offset, y_offset);
            break;
        case M:
            return drawSpriteXPM(M_SPRITE, x_offset, y_offset);
            break;
        case N:
            return drawSpriteXPM(N_SPRITE, x_offset, y_offset);
            break;
        case O:
            return drawSpriteXPM(O_SPRITE, x_offset, y_offset);
            break;
        case P:
            return drawSpriteXPM(P_SPRITE, x_offset, y_offset);
            break;
        case Q:
            return drawSpriteXPM(Q_SPRITE, x_offset, y_offset);
            break;
        case R:
            return drawSpriteXPM(R_SPRITE, x_offset, y_offset);
            break;
        case S:
            return drawSpriteXPM(S_SPRITE, x_offset, y_offset);
            break;
        case T:
            return drawSpriteXPM(T_SPRITE, x_offset, y_offset);
            break;
        case U:
            return drawSpriteXPM(U_SPRITE, x_offset, y_offset);
            break;
        case V:
            return drawSpriteXPM(V_SPRITE, x_offset, y_offset);
            break;
        case W:
            return drawSpriteXPM(W_SPRITE, x_offset, y_offset);
            break;
        case X:
            return drawSpriteXPM(X_SPRITE, x_offset, y_offset);
            break;
        case Y:
            return drawSpriteXPM(Y_SPRITE, x_offset, y_offset);
            break;
        case Z:
            return drawSpriteXPM(Z_SPRITE, x_offset, y_offset);
            break;
        case COMMA:
            return drawSpriteXPM(COMMA_SPRITE, x_offset, y_offset);
            break;
        case PERIOD:
            return drawSpriteXPM(PERIOD_SPRITE, x_offset, y_offset);
            break;
        default:
            break;
    }

    return 0;
}



