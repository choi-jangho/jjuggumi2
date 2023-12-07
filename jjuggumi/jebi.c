#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#pragma warning(disable:4996)

#define MAX_LOTS 10

void shuffle_lots(bool lots[], int n, bool picked[]);
bool pick_lot(bool lots[], int selected_lot, bool picked[]);
void display_game_state(int round, int turn, int remaining_players, int selected_lot, bool picked[]);
void jebi(void);
int visited[PLAYER_MAX] = { 0 };
int selected_lot = 0;
int turn = -1;

void jebi(void) {
    srand((unsigned int)time(NULL));
    bool lots[MAX_LOTS], picked[MAX_LOTS] = { false };
    int round = 1, last_alive = -1;
    key_t key;
    selected_lot = 0;
    n_alive = 0;
    for (int i = 0; i < 6; i++) {
        if (player[i].is_alive == true) {
            n_alive++;
        }
    }

    while (n_alive > 1) {
        for (int i = 0; i < n_alive; i++) {
            picked[i] = false;
        }

        shuffle_lots(lots, n_alive, picked);
        selected_lot = 0;

        for (int i = 0; i < PLAYER_MAX; i++) {
            if (!player[i].is_alive) {
                visited[i] = 1;
            }
        }

        while (1) {
            // 다음 살아있는 플레이어 찾기
            for (int i = 0; i < PLAYER_MAX; i++) {
                if (player[i].is_alive && visited[i] == 0) {
                    turn = i;
                    break;
                }
            }

            display_game_state(round, turn, n_alive, selected_lot, picked);

            do {
                key = get_key();
                if (key == K_LEFT) {
                    do {
                        selected_lot = (selected_lot + n_alive - 1) % n_alive;
                    } while (picked[selected_lot]);
                    display_game_state(round, turn, n_alive, selected_lot, picked);
                }
                else if (key == K_RIGHT) {
                    do {
                        selected_lot = (selected_lot + 1) % n_alive;
                    } while (picked[selected_lot]);
                    display_game_state(round, turn, n_alive, selected_lot, picked);
                }
                else if (key == K_QUIT) {
                    ending();
                    exit(1);
                }
            } while (key != K_SPACE);

            bool result = pick_lot(lots, selected_lot, picked);
            char* dialog_msg = (char*)malloc(sizeof(char) * 50);
            int row_m = 4;
            int col_m = 11;
            if (result) {
                sprintf(dialog_msg, "player %d pass!", turn);
                dialog(dialog_msg, row_m, col_m);
                last_alive = turn;  // 살아남은 플레이어 업데이트
                visited[turn] = 1;
            }
            else {
                sprintf(dialog_msg, "player %d fail!", turn);
                dialog(dialog_msg, row_m, col_m);
                player[turn].is_alive = false;
                n_alive--;
                for (int i = 0; i < PLAYER_MAX; i++) {
                    visited[i] = 0;
                }
                for (int i = 0; i < PLAYER_MAX; i++) {
                    if (player[i].is_alive && visited[i] == 0) {
                        turn = i;
                        break;
                    }
                }
                break; // 라운드 종료
            }

            // 자동으로 이미 선택된 다음의 제비로 옮기기
            do {
                selected_lot = (selected_lot + 1) % n_alive;
            } while (picked[selected_lot] && n_alive > 1);
        }
        round++;
    }

    printf("Game Over. Winner is player %d\n", last_alive);
}

void shuffle_lots(bool lots[], int n, bool picked[]) {
    for (int i = 0; i < n; i++) {
        if (!picked[i]) {
            lots[i] = true;
        }
    }
    int fail_index = rand() % n;
    while (picked[fail_index]) {
        fail_index = rand() % n;
    }
    lots[fail_index] = false;  // 하나의 제비만 'fail'로 설정
}

bool pick_lot(bool lots[], int selected_lot, bool picked[]) {
    picked[selected_lot] = true;
    return lots[selected_lot];
}

void display_game_state(int round, int turn, int remaining_players, int selected_lot, bool picked[]) {
    system("cls");
    printf("**************************\n");
    printf("*                        *\n");
    printf("* ");
    for (int i = 0; i < MAX_LOTS; i++) {
        if (i < remaining_players) {
            if (picked[i]) {
                printf("X ");
            }
            else if (i == selected_lot) {
                printf("@ ");
            }
            else {
                printf("? ");
            }
        }
        else {
            printf("  ");
        }
    }

    printf("   *\n");
    printf("*                        *\n");
    printf("**************************\n");
    printf("round %d, turn: player %d\n", round, turn);
    gotoxy(N_ROW + 4, 0);
    print_status();
}
