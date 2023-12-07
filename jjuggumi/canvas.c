#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "jjuggumi.h"
#include "canvas.h"

#define DIALOG_DURATION_SEC		4

void draw(void);
void print_status(void);

// (zero-base) row��, col���� Ŀ�� �̵�
void gotoxy(int row, int col) {
	COORD pos = { col,row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// row��, col���� ch ���
void printxy(char ch, int row, int col) {
	gotoxy(row, col);
	printf("%c", ch);
}

void map_init(int n_row, int n_col) {
	// �� ���۸��� ������ ����
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		back_buf[i][0] = back_buf[i][N_COL - 1] = '*';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '*' : ' ';
		}
	}
}

void juldarigi_map_init(int n_row, int n_col) {
	// �� ���۸��� ������ ����
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		back_buf[i][0] = back_buf[i][N_COL - 1] = '#';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = ((i == 0 || i == N_ROW - 1) && j != N_COL/2) ? '#' : ' ';
		}
	}
}

// back_buf[row][col]�� �̵��� �� �ִ� �ڸ����� Ȯ���ϴ� �Լ�
bool placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

// ��ܿ� ����, �ϴܿ��� ���� ���¸� ���
void display(void) {
	draw();
	gotoxy(N_ROW + 4, 0);  // �߰��� ǥ���� ������ ������ �ʰ� ����â ������ �� ������ ���
	print_status();
}

void draw(void) {
	for (int row = 0; row < N_ROW; row++) {
		for (int col = 0; col < N_COL; col++) {
			if (front_buf[row][col] != back_buf[row][col]) {
				front_buf[row][col] = back_buf[row][col];
				printxy(front_buf[row][col], row, col);
			}
		}
	}
}

void print_status(void) {
	gotoxy(14, 0);
	printf("no. of players left: %d\n", n_alive);
	printf("                  intl    str  stm\n"); // �÷� ���� �߰�

	for (int p = 0; p < n_player; p++) {
		printf("player %2d: %5s %2d(%s%d) %2d(%s%d) %3d%%\n",
			p,
			player[p].is_alive ? "alive" : " DEAD",
			player[p].intel, (player[p].item.intel_buf >= 0) ? "+" : "", player[p].item.intel_buf, // ���� �� ������ ����
			player[p].str, (player[p].item.str_buf >= 0) ? "+" : "", player[p].item.str_buf,   // �� �� ������ ����
			player[p].stamina                          // ���¹̳�
		);
	}
}

void print_juldarigi_status(int flag[]) {
	gotoxy(14, 0);
	printf("no. of players left: %d\n", n_alive);
	printf("                  intl    str  stm\n"); // �÷� ���� �߰�

	for (int p = 0; p < n_player; p++) {
		if (flag[p] == 0) {
			printf("player %2d: %5s  %3d(%s%d) %2d(%s%d) %3d%%\n",
				p,
				flag[p] ? "alive" : " DEAD",
				player[p].intel, (player[p].item.intel_buf >= 0) ? "+" : "", player[p].item.intel_buf, // ���� �� ������ ����
				player[p].str, (player[p].item.str_buf >= 0) ? "+" : "", player[p].item.str_buf,   // �� �� ������ ����
				player[p].stamina                          // ���¹̳�
			);
		}
		else {
			printf("player %2d: %5s %3d(%s%d) %2d(%s%d) %3d%%\n",
				p,
				player[p].is_alive ? "alive " : "alive*",
				player[p].intel, (player[p].item.intel_buf >= 0) ? "+" : "", player[p].item.intel_buf, // ���� �� ������ ����
				player[p].str, (player[p].item.str_buf >= 0) ? "+" : "", player[p].item.str_buf,   // �� �� ������ ����
				player[p].stamina                          // ���¹̳�
			);
		}
	}
}

void dialog(char message[], int row, int col) {
	int msg_len = strlen(message);
	char temp_buf[ROW_MAX][COL_MAX];
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			temp_buf[i][j] = back_buf[i][j];
		}
	}

	char msg[COL_MAX] = "";
	for (int i = 0; i < msg_len; i++) {
		if (message[i] == '\0')
			break;
		msg[i] = message[i];
	}
	for (int i = DIALOG_DURATION_SEC; i > 0; i--) {
		gotoxy(row / 2 - 1, col / 5);
		for (int i = 0; i < msg_len + 5; i++) {
			printf("*");
		}
		gotoxy(row / 2, col / 5);
		printf("*%d %s *\n", i, msg);
		gotoxy(row / 2 + 1, col / 5);
		for (int i = 0; i < msg_len + 5; i++) {
			printf("*");
		}
		Sleep(1000);
	}

	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++) {
			gotoxy(i, j);
			printf("%c", temp_buf[i][j]);
		}

	draw();
}
