#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define NIGHT_MAP_ROW 12
#define NIGHT_MAP_COL 35

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];
int tx[PLAYER_MAX], ty[PLAYER_MAX];
int ix[ITEM_MAX], iy[ITEM_MAX];

void nightgame_init(void) {
    // 맵 초기화
    map_init(NIGHT_MAP_ROW, NIGHT_MAP_COL);
    int x, y;

    // 플레이어 초기 위치 설정
    for (int i = 0; i < n_player; i++) {
        if (!player[i].is_alive)
            continue;
        do {
            px[i] = randint(1, NIGHT_MAP_ROW - 2);
            py[i] = randint(1, NIGHT_MAP_COL - 2);
            period[i] = randint(50, 100);
        } while (back_buf[px[i]][py[i]] != ' ');
        back_buf[px[i]][py[i]] = '0' + i;
    }

    // 아이템 초기 위치 설정
    for (int i = 0; i < n_item; i++) {
        do {
            ix[i] = randint(1, NIGHT_MAP_ROW - 2);
            iy[i] = randint(1, NIGHT_MAP_COL - 2);
        } while (back_buf[ix[i]][iy[i]] != ' ');
        back_buf[ix[i]][iy[i]] = 'I';
    }

    tick = 0;
}

// 아이템을 화면에 그리는 함수
void drawItem() {
    for (int i = 0; i < n_item; i++)
        if (ix[i] != -1 && back_buf[ix[i]][iy[i]] == ' ')
            back_buf[ix[i]][iy[i]] = 'I';
}

// 주어진 좌표가 아이템 위치인지 확인하는 함수
int isItem(int nx, int ny) {
    for (int i = 0; i < n_item; i++)
        if (ix[i] == nx && iy[i] == ny)
            return i;
    return -1;
}

// 아이템 위치를 교환하는 함수
void swapItem(int playerIndex, int itemIndex) {
    int p = playerIndex;

    for (int i = 0; i < n_item; i++) {
        if (!strcmp(item[i].name, player[p].item.name)) {
            ix[i] = ix[itemIndex];
            iy[i] = iy[itemIndex];
            ix[itemIndex] = -1;
            iy[itemIndex] = -1;
        }
    }
    player[p].item = item[itemIndex];
}

// 특정 좌표에서 아이템을 확인하고 획득하는 함수
void checkItem(int playerIndex, int nx, int ny) {
    int p = playerIndex;
    int itemIndex = isItem(nx, ny);

    if (itemIndex != -1) {
        if (!player[p].hasitem) {
            player[p].hasitem = true;
            player[p].item = item[itemIndex];
            ix[itemIndex] = -1;
            iy[itemIndex] = -1;
        }
        else {
            if (p == 0) {
                int k = 5;
                for (int tick = 60000; tick >= 0; tick--) {
                    gotoxy(13, 0);
                    if (tick % 10000 == 0) {
                        printf("[%d] Press C to swap item.", k);
                        k--;
                    }
                    int key = get_key();
                    if (key == K_CONFIRM) {
                        swapItem(0, itemIndex);
                        dialog("Item swapped.", NIGHT_MAP_ROW, NIGHT_MAP_COL);
                        break;
                    }
                }
                gotoxy(13, 0);
                printf("                         ");
            }
            else {
                if (randint(0, 1) == 0)
                    swapItem(p, itemIndex);
            }
        }
    }
}

// 특정 위치에 플레이어나 아이템이 배치 가능한지 확인하는 함수
bool placable2(int row, int col) {
    if (row < 0 || row >= N_ROW ||
        col < 0 || col >= N_COL ||
        (back_buf[row][col] != ' ' && back_buf[row][col] != 'I')) {
        return false;
    }
    return true;
}

// 플레이어가 수동으로 움직이는 함수
void move_manual2(key_t key) {
    static int dx[4] = { -1, 1, 0, 0 };
    static int dy[4] = { 0, 0, -1, 1 };

    int dir;
    switch (key) {
    case K_UP: dir = DIR_UP; break;
    case K_DOWN: dir = DIR_DOWN; break;
    case K_LEFT: dir = DIR_LEFT; break;
    case K_RIGHT: dir = DIR_RIGHT; break;
    default: return;
    }

    int nx, ny;
    nx = px[0] + dx[dir];
    ny = py[0] + dy[dir];

    if (!placable2(nx, ny)) {
        return;
    }

    checkItem(0, nx, ny);
    move_tail(0, nx, ny);

    if (player[0].is_alive == true) {
        player[0].stamina = max(0, player[0].stamina - 1);
    }
}

// 플레이어가 랜덤으로 움직이는 함수
void move_random2(int playerIndex) {
    int p = playerIndex;
    int nx, ny;
    int move_x;
    int move_y;

    if (tx[p] != -1) {
        if (px[p] < tx[p])
            move_x = 1;
        else if (px[p] == tx[p])
            move_x = 0;
        else
            move_x = -1;

        if (py[p] < ty[p])
            move_y = 1;
        else if (py[p] == ty[p])
            move_y = 0;
        else
            move_y = -1;

        nx = px[p] + move_x;
        ny = py[p] + move_y;

        if (!placable2(nx, ny))
            return;
    }
    else {
        do {
            move_x = randint(0, 3);
            move_y = randint(0, 3);

            if (move_x == 0)
                move_x = -1;
            else if (move_x == 1)
                move_x = 1;
            else
                move_x = 0;

            if (move_y == 0)
                move_y = -1;
            else if (move_y == 1)
                move_y = 1;
            else
                move_y = 0;

            nx = px[p] + move_x;
            ny = py[p] + move_y;

        } while (!placable2(nx, ny));
    }

    checkItem(p, nx, ny);
    move_tail(p, nx, ny);

    if (player[p].is_alive == true) {
        player[p].stamina = max(0, player[p].stamina - 1);
    }
}

// 가장 가까운 아이템 또는 플레이어를 찾는 함수
void find_nearest() {
    int dist = 1000;

    for (int n = 1; n < n_player; n++) {
        for (int i = 0; i < n_player; i++) {
            if (n == i)
                continue;
            if (!player[i].hasitem)
                continue;

            int d = (px[n] - px[i]) * (px[n] - px[i]) + (py[n] - py[i]) * (py[n] - py[i]);
            if (d != 0 && d < dist) {
                tx[n] = px[i];
                ty[n] = py[i];
            }
        }

        for (int i = 0; i < n_item; i++) {
            int d = (px[n] - ix[i]) * (px[n] - ix[i]) + (py[n] - ix[i]) * (py[n] - iy[i]);
            if (d != 0 && d < dist) {
                tx[n] = ix[i];
                ty[n] = iy[i];
            }
        }
    }
}

// 플레이어 간 상호 작용 함수
void interact(int playerIndex1, int playerIndex2) {
    int p1 = playerIndex1;
    int p2 = playerIndex2;

    key_t key;
    if (p1 == 0 || p2 == 0) {
        int k = 5;
        for (int tick = 60000; tick >= 0; tick--) {
            gotoxy(13, 0);
            if (tick % 10000 == 0) {
                printf("[%d] Press a to steal, s to conciliate.", k);
                k--;
            }
            key = get_key();
        }
        gotoxy(13, 0);
        printf("                                        ");
    }
    else {
        if (player[p1].hasitem && !player[p2].hasitem) {
            int temp = p1;
            p1 = p2;
            p2 = temp;
        }
        if (randint(0, 1) == 0)
            key = K_STEAL;
        else
            key = K_CONCIL;
    }

    if (key == K_STEAL) {
        if (player[p1].stamina == 0 || !player[p2].hasitem)
            return;
        if (player[p1].str > player[p2].str) {
            char message[] = "0 stole item from 0.";
            message[0] = p1 + '0'; message[18] = p2 + '0';

            ITEM tmp = player[p1].item;
            player[p1].item = player[p2].item;
            player[p2].item = tmp;

            if (player[p1].stamina >= 40)
                player[p1].stamina -= 40;
            else
                player[p1].stamina = 0;
            dialog(message, NIGHT_MAP_ROW, NIGHT_MAP_COL);
        }
        else {
            char message[] = "0 fail to steal.";
            message[0] = '0' + p1;
            if (player[p1].stamina >= 60)
                player[p1].stamina -= 60;
            else
                player[p1].stamina = 0;
            dialog(message, NIGHT_MAP_ROW, NIGHT_MAP_COL);
        }
    }
    else if (key == K_CONCIL) {
        if (player[p1].stamina == 0 || !player[p2].hasitem)
            return;
        if (player[p1].str > player[p2].str) {
            char message[] = "0 conciliate item from 0.";
            message[0] = '0' + p1; message[23] = '0' + p2;

            ITEM tmp = player[p1].item;
            player[p1].item = player[p2].item;
            player[p2].item = tmp;

            if (player[p1].stamina >= 20)
                player[p1].stamina -= 20;
            else
                player[p1].stamina = 0;
            dialog(message, NIGHT_MAP_ROW, NIGHT_MAP_COL);
        }
        else {
            char message[] = "0 fail to conciliate.";
            message[0] = '0' + p1;
            if (player[p1].stamina >= 40)
                player[p1].stamina -= 40;
            else
                player[p1].stamina = 0;
            dialog(message, NIGHT_MAP_ROW, NIGHT_MAP_COL);
        }
    }
}

// 주변에 플레이어가 있는지 확인하고 상호 작용하는 함수
bool checkNear() {
    int ci = 6, cn = 6;
    for (int n = 0; n < n_player; n++) {
        for (int i = 0; i < n_player; i++) {
            if (n == i || (cn == i && ci == n) || player[i].is_alive == false || player[n].is_alive == false)
                continue;
            int d = (px[n] - px[i]) * (px[n] - px[i]) + (py[n] - py[i]) * (py[n] - py[i]);
            if (d == 1) {
                interact(n, i);
                n = cn;
                i = ci;
            }
        }
    }
}

// 게임 루프 함수
void nightgame(void) {
    nightgame_init();

    int temp_px[PLAYER_MAX];
    int temp_py[PLAYER_MAX];

    system("cls");
    display();

    key_t key;
    while (1) {
        key = get_key();
        if (key == K_QUIT) {
            return;
            break;
        }
        else if (key != K_UNDEFINED) {
            if (player[0].is_alive != false)
            {
                move_manual2(key);
                checkNear();
            }
        }

        for (int i = 1; i < n_player; i++) {
            if (player[i].is_alive == true) {
                if (tick % period[i] == 0) {
                    if (is_arrive(i) == 1)
                    {
                        continue;
                    }
					move_random2(i);
					checkNear();
                }
            }
        }

        for (int i = 0; i < n_alive; i++) {
            if (player[i].stamina == 0) {
                if (player[i].is_alive == true) {
                    char message[] = "player   dead";
                    message[7] = '0' + i;
                    dialog(message, NIGHT_MAP_ROW, NIGHT_MAP_COL);
                }
                player[i].is_alive = false;
            }
        }

        find_nearest();
        drawItem();
        display();
        Sleep(10);
        tick++;
    }
}
