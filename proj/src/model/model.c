#include "model.h"

extern uint8_t data;
Key currentKey = NONE_KEY;
GameState currentState = NONE_STATE;
extern uint8_t *main_frame_buffer;
extern uint8_t *secondary_frame_buffer;
extern uint8_t *secondary_frame_buffer_no_mouse;
extern uint32_t frame_size;
extern vbe_mode_info_t mode_info;
int x_offset=0;
int y_offset=0;

extern int bytes_read;
extern struct packet pp;
extern mouse_position mouse_pos;

Sprite *CURSOR_SPRITE;

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

void initialize_sprites(){
    CURSOR_SPRITE = create_sprite_xpm((xpm_map_t)cursor_xpm);

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

}

int offset_handler(int x){
    if (x==0){
        x_offset+=13;
    }
    if (x_offset + 13 >=mode_info.XResolution){
        x_offset=0;
        y_offset+=20;
    }
    return 1;
}

void update_timer(){
    memcpy(secondary_frame_buffer, secondary_frame_buffer_no_mouse, frame_size);
    drawCursor();
    memcpy(main_frame_buffer, secondary_frame_buffer, frame_size);
    (timer_int_handler)();

}

// MOUSE STUFF
void initialize_mouse_data(){
    mouse_pos.last_x = 500;
    mouse_pos.last_y = 300;
    mouse_pos.x = 500;
    mouse_pos.y = 300;
}

void update_mouse(){
    mouse_ih();
    sync();
    if (bytes_read == 3) {
        parse();
        bytes_read = 0;
    }
}

void update_cursor_last_pos(){
    // save position for erasing later
    mouse_pos.last_x = mouse_pos.x;
    mouse_pos.last_y = mouse_pos.y;
}

// -------------

void update_keyboard(){
    (kbc_ih)();
    if (!(data&BIT(7))){ 
    switch (data) {
        case KEY_A:
            currentKey = A;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_B:
            currentKey = B;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_C:
            currentKey = C;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_D:
            currentKey = D;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_E:
            currentKey = E;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_F:
            currentKey = F;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_G:
            currentKey = G;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_H:
            currentKey = H;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_I:
            currentKey = I;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_J:
            currentKey = J;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_K:
            currentKey = K;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_L:
            currentKey = L;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_M:
            currentKey = M;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_N:
            currentKey = N;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_O:
            currentKey = O;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_P:
            currentKey = P;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_Q:
            currentKey = Q;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_R:
            currentKey = R;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_S:
            currentKey = S;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_T:
            currentKey = T;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_U:
            currentKey = U;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_V:
            currentKey = V;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_W:
            currentKey = W;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_X:
            currentKey = X;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_Y:
            currentKey = Y;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_Z:
            currentKey = Z;
            drawCurrentLetter();
            offset_handler(0);
            break;
        case KEY_ENTER:
            printf("ENTER\n");
            break;
        case KEY_SPACE:
            offset_handler(0);
            break;
        case KEY_DELETE:
            printf("DELETE\n");
            break;
        default:
            break;
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

}

