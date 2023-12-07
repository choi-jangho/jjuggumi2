#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>
#pragma warning(disable:4996)

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void mugunghwa_init(void);
void move_manual(key_t key);
void move_random(int i, int dir);
void move_tail(int i, int nx, int ny);
void mugunghwa(void);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�

int row_m = 9;
int col_m = 40;

void mugunghwa_init(void) {
	map_init(row_m, col_m);
	int x, y;
	for (int i = 0; i < n_player; i++) {
		// ���� �ڸ��� ������ �ٽ� ����
		x = 2 + i;
		y = N_COL - 2;
		px[i] = x;
		py[i] = y;
		period[i] = randint(50, 100);

		back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
	}

	tick = 0;
}

void move_manual(key_t key) {
	// �� �������� ������ �� x, y�� delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // ������ ����(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// �������� ���� �ڸ�
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (!placable(nx, ny)) {
		return;
	}

	move_tail(0, nx, ny);

	// ���¹̳� ����
	if (player[0].is_alive == true) {
		player[0].stamina = max(0, player[0].stamina - 1);
	}
}

// 0 <= dir < 4�� �ƴϸ� ����
void move_random(int playerIndex, int dir) {
	int p = playerIndex;  // �̸��� ��...
	int nx, ny;  // �������� ������ ���� �ڸ�

	// ������ ������ ���� ���� ���ٰ� ����(���� ������ ����)	
	int move_x;
	int move_y;
	do {
		move_x = randint(0, 9);
		move_y = randint(0, 9);

		if (move_x == 0)
		{
			move_x = -1;
		}
		else if (move_x == 1)
		{
			move_x = 1;
		}
		else
		{
			move_x = 0;
		}

		if (move_y < 7)
		{
			move_y = -1;
		}
		else
		{
			move_y = 0;
		}

		nx = px[p] + move_x;
		ny = py[p] + move_y;

	} while (!placable(nx, ny));

	move_tail(p, nx, ny);

	// ���¹̳� ����
	if (player[p].is_alive == true) {
		player[p].stamina = max(0, player[p].stamina - 1);
	}
}

// back_buf[][]�� ���
void move_tail(int player, int nx, int ny) {
	int p = player;  // �̸��� ��...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

// ���� �տ� �����ߴ��� �Ǻ� (1�̸� ���� 0�̸� �̵���)
int is_arrive(int p) {
	if (px[p] == 2 && py[p] == 1)
	{
		return 1;
	}
	else if (px[p] == 3 && py[p] == 2)
	{
		return 1;
	}
	else if (px[p] == 4 && py[p] == 2)
	{
		return 1;
	}
	else if (px[p] == 5 && py[p] == 2)
	{
		return 1;
	}
	else if (px[p] == 6 && py[p] == 1)
	{
		return 1;
	}

	return 0;
}

// Ÿ�̸ӿ� ���� ����ȭ�����Ǿ����ϴ� ���. Ÿ�̸� �������� �ð� ����
void time_stop(int timer)
{
	if (timer == 50)
	{
		gotoxy(10, 0);
		printf("�� ");
	}
	else if (timer == 400)
	{
		gotoxy(10, 4);
		printf("�� ");
	}
	else if (timer == 1000)
	{
		gotoxy(10, 8);
		printf("ȭ ");
	}
	else if (timer == 1700)
	{
		gotoxy(10, 12);
		printf("�� ");
	}
	else if (timer == 2500)
	{
		gotoxy(10, 16);
		printf("�� ");
	}
	else if (timer == 3250)
	{
		gotoxy(10, 20);
		printf("�� ");
	}
	else if (timer == 3750)
	{
		gotoxy(10, 24);
		printf("�� ");
	}
	else if (timer == 4100)
	{
		gotoxy(10, 28);
		printf("�� ");
	}
	else if (timer == 4350)
	{
		gotoxy(10, 32);
		printf("�� ");
	}
	else if (timer == 4500)
	{
		gotoxy(10, 36);
		printf("�� ");
	}
}

// ����ȭ ���� �Ǿ����ϴ� ���� ������
void clear_stop()
{
	for (int i = 0; i < 37; i += 4)
	{
		gotoxy(10, i);
		printf("  ");
	}
}

// ����ȭ�����Ǿ����ϴ� ��� �� 3�� ��� �ð� ������ ������ �÷��̾� ��ǥ ��, �ٸ� ��쿣 �� �÷��̾ �ش��ϴ� �ε����� 1�� �ٲپ ������
int* compare_loc(int temp_px[], int temp_py[])
{
	/*int* result = (int*)malloc(sizeof(int) * PLAYER_MAX);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		result[i] = 0;
	}*/

	int back = 0;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if ((px[i] != temp_px[i] || py[i] != temp_py[i]) || (px[i] != temp_px[i] && py[i] != temp_py[i]))
		{
			if (player[i].is_alive == true) {
				int alive = 0;
				for (int j = 0; j < PLAYER_MAX; j++) {
					if (player[j].is_alive == true) {
						if (px[i] == px[j] && py[i] > py[j]) {
							alive = 1;
							if (temp_px[i] == px[j] && temp_py[i] > py[j]) {
								if (py[j] > 1) {
									alive = 1;
								}
								else {
									alive = 0;
								}
							}
							else if (temp_px[i] != px[j] || temp_py[i] < py[j]) {
								alive = 0;
							}
						}
					}
				}
				if (alive == 0) {
					player[i].is_alive = false;
					n_alive--;
				}
				if (n_alive == 1)
				{
					ending();
				}
			}
		}
	}
}

void reprint()
{
	for (int i = 0; i < n_player; i++)
	{
		if (player[i].is_alive == true)
		{
			gotoxy(px[i], py[i]);
			printf("%d", i);
		}
		else
		{
			back_buf[px[i]][py[i]] = ' ';
		}
	}

	for (int i = 0; i < N_ROW; i++)
	{
		gotoxy(i, 0);
		if (i == 0)
		{
			for (int j = 1; j < N_COL - 1; j++)
			{
				printf("*");
			}
		}
		else if (i == N_ROW - 1)
		{
			for (int j = 1; j < N_COL - 1; j++)
			{
				printf("*");
			}
		}
		else
		{
			printf("*");
			gotoxy(i, N_COL - 1);
			printf("*");
		}
	}
}

void mugunghwa(void) {
	mugunghwa_init();
	int flag = 0;

	int timer = 0;

	int temp_px[PLAYER_MAX];
	int temp_py[PLAYER_MAX];

	int* diff = (int*)malloc(sizeof(int) * PLAYER_MAX);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		temp_px[i] = px[i];
		temp_py[i] = py[i];
	}

	system("cls");
	display();

	key_t key;
	int player_log[PLAYER_MAX] = { 0 }; // dialog�� ���� Ż���ߴ� �÷��̾� ����� ����

	while (1) {
		// ���� ��� ���
		gotoxy(3, 1);
		printf("#");
		gotoxy(4, 1);
		printf("#");
		gotoxy(5, 1);
		printf("#");
		// player 0�� ������ ������(4����)
		key = get_key();
		if (key == K_QUIT) {
			ending();
			exit(1);
			break;
		}
		else if (key != K_UNDEFINED) {
			if (player[0].is_alive != false)
			{
				move_manual(key);
			}
		}

		int arrive_count = 0;
		for (int i = 0; i < n_player; i++)
		{
			if (is_arrive(i) == 1)
			{
				arrive_count++;
			}
		}

		if (n_alive == arrive_count)
		{
			return;
		}

		// player 1 ���ʹ� �������� ������(8����)
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				if (is_arrive(i) == 1)
				{
					continue;
				}

				if (player[i].is_alive == true)
					move_random(i, -1);
			}
		}

		display();
		Sleep(10);
		timer += 10; // �� �ݺ����� Ÿ�̸� 10�� ����
		time_stop(timer);

		// Ÿ�̸Ӱ� 4500�� ������ ��� ����ȭ�����Ǿ�����'��'�� ��µǸ� ���� ����� �ٲ�� �����̴� ��� ����
		if (timer >= 4500)
		{
			gotoxy(3, 1);
			printf("@");
			gotoxy(4, 1);
			printf("@");
			gotoxy(5, 1);
			printf("@");

			// 3�� ���� ����ϸ� �������� 10�ۼ�Ʈ Ȯ���� �̵���Ŵ
			for (int i = 0; i < 300; i++)
			{

				for (int i = 0; i < PLAYER_MAX; i++)
				{
					temp_px[i] = px[i];
					temp_py[i] = py[i];
				}

				key = get_key();
				if (key == K_QUIT) {
					ending();
					exit(1);
					break;
				}
				else if (key != K_UNDEFINED) {
					if (player[0].is_alive != false)
					{
						move_manual(key);
					}
				}

				for (int i = 1; i < n_player; i++) {
					if (tick % period[i] == 0) {
						if (is_arrive(i) == 1)
						{
							continue;
						}

						int num = randint(0, 9);

						if (num == 0 && player[i].is_alive == true)
						{
							move_random(i, -1);
						}
						else
						{
							continue;
						}
					}
				}
				// ��ġ ��
				compare_loc(temp_px, temp_py);
				display();
				tick += 10;
				Sleep(10);
			}

			// Ż���� �÷��̾ ������ �÷��� ǥ�� �� break
			for (int i = 0; i < n_player; i++)
			{
				if (player[i].is_alive == false && player_log[i] == 0)
				{
					diff[i] = 1;
					flag = 1;
				}
			}
			// Ÿ�̸� �ʱ�ȭ �� ���� �����
			timer = 0;
			clear_stop();
		}
		// Ż���� �÷��̾ �ִ� ���
		if (flag == 1)
		{
			// ����� �� ����
			int die_count = 0;
			// ����� �� ���� Ȯ���ϱ�, diff[i]�� 1�� ���� i ��° �÷��̾ ��ġ�� �޶��� ����Ǿ��ٰ� �Ǻ�
			for (int i = 0; i < PLAYER_MAX; i++)
			{
				if (diff[i] == 1)
				{
					die_count++;
				}
			}
			printf("\n");

			// dialog�� ���ڿ��� ����ϱ� ���� ���ڿ� ���� ����
			char* str = (char*)malloc(sizeof(char) * 100); // ũ�⸦ ����� �ø�
			char* str_ = (char*)malloc(sizeof(char) * 50); // ũ�⸦ ����� �ø�
			if (!str || !str_) {
				// �޸� �Ҵ� ���� ó��
				exit(1);
			}
			sprintf(str, "player "); // sprintf ���
			int init = 0;
			for (int i = 0; i < PLAYER_MAX; i++)
			{
				if (diff[i] == 1 && player_log[i] == 0)
				{
					if (init == 0)
					{
						sprintf(str_, "%d", i);
						init = 1;
						player_log[i] = 1;
					}
					else
					{
						char* temp = (char*)malloc(sizeof(char) * 50);
						sprintf(temp, ", %d", i);
						strcat(str_, temp);
						player_log[i] = 1;
					}
				}
			}
			strcat(str, str_);
			strcat(str, " dead!");
			dialog(str, row_m, col_m);
			flag = 0;

			reprint();

			free(str);
			free(str_);

			display();
		}

		tick += 10;
	}
}