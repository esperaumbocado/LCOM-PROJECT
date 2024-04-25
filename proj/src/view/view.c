#include "view.h"

uint8_t *main_frame_buffer;
uint8_t *drawing_frame_buffer;
extern int currentKey;

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

            if (draw_pixel(x + col, y + row, color, drawing_frame_buffer) != 0) {
                return 1;
            }
        }
    }
    
    return 0;
}


int drawCurrentLetter() {
    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, WHITE, drawing_frame_buffer);
    int x = 0;
    int y = 0;
    switch(currentKey){
        case A:
            return drawSpriteXPM(A_SPRITE, x, y);
            break;
        case B:
            return drawSpriteXPM(B_SPRITE, x, y);
            break;
        case C:
            return drawSpriteXPM(C_SPRITE, x, y);
            break;
        case D:
            return drawSpriteXPM(D_SPRITE, x, y);
            break;
        case E:
            return drawSpriteXPM(E_SPRITE, x, y);
            break;
        case F:
            return drawSpriteXPM(F_SPRITE, x, y);
            break;
        case G:
            return drawSpriteXPM(G_SPRITE, x, y);
            break;
        case H:
            return drawSpriteXPM(H_SPRITE, x, y);
            break;
        case I:
            return drawSpriteXPM(I_SPRITE, x, y);
            break;
        case J:
            return drawSpriteXPM(J_SPRITE, x, y);
            break;
        case K:
            return drawSpriteXPM(K_SPRITE, x, y);
            break;
        case L:
            return drawSpriteXPM(L_SPRITE, x, y);
            break;
        case M:
            return drawSpriteXPM(M_SPRITE, x, y);
            break;
        case N:
            return drawSpriteXPM(N_SPRITE, x, y);
            break;
        case O:
            return drawSpriteXPM(O_SPRITE, x, y);
            break;
        case P:
            return drawSpriteXPM(P_SPRITE, x, y);
            break;
        case Q:
            return drawSpriteXPM(Q_SPRITE, x, y);
            break;
        case R:
            return drawSpriteXPM(R_SPRITE, x, y);
            break;
        case S:
            return drawSpriteXPM(S_SPRITE, x, y);
            break;
        case T:
            return drawSpriteXPM(T_SPRITE, x, y);
            break;
        case U:
            return drawSpriteXPM(U_SPRITE, x, y);
            break;
        case V:
            return drawSpriteXPM(V_SPRITE, x, y);
            break;
        case W:
            return drawSpriteXPM(W_SPRITE, x, y);
            break;
        case X:
            return drawSpriteXPM(X_SPRITE, x, y);
            break;
        case Y:
            return drawSpriteXPM(Y_SPRITE, x, y);
            break;
        case Z:
            return drawSpriteXPM(Z_SPRITE, x, y);
            break;
        default:
            break;


    }

    return 0;
}








