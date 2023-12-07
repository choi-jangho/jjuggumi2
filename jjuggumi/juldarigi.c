#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>
#pragma warning(disable:4996)


void juldarigi_init();
void in_game();
int	isduplication(int);
void move();
void player_out();
void print_str();
void print_result_str();

int py[PLAYER_MAX];
char temp_buf[PLAYER_MAX];

int row = 3;
int col = 30;

int left[3] = { 11, 11, 11 };
int right[3] = { 11, 11, 11 };
int result_str = 0;
int left_str, right_str;
int standard_py = 10;
int left_alive = 3, right_alive = 3;
int flag[6];
int temp_item[6];

// 왼쪽-오른쪽 플레이어 중복방지
int isduplication(int num) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (left[j] == num) return 1;
			if (right[j] == num) return 1;
		}
	}
	return 0;
}

void juldarigi_init(void) {
	juldarigi_map_init(row, col);
	
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			int num;
			if (i == 0){
				do {
					num = randint(0, 5);
				} while (isduplication(num));
				left[j] = num;
			}
			else {
				do {
					num = randint(0, 5);
				} while (isduplication(num));
				right[j] = num;
			}
		}
	}


	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 0) {
				py[j] = col / 2 - 4 + j;
				back_buf[1][py[j]] = '0' + left[j];
			}
			if (i == 1) {
				py[j] = col / 2 + 4 - j;
				back_buf[1][py[j]] = '0' + right[j];
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		py[i] = col / 2 - 1 + i;
		back_buf[1][py[i]] = '-';
	}
}

// str: 유효 힘의 합: (-왼쪽팀) + (오른쪽팀)
void print_str() {
	gotoxy(7, 0);
	result_str = 0;
	left_str = 0, right_str = 0;
	for (int i = 0; i < right_alive; i++) {
		if (flag[right[i]] == 1) {
			right_str += player[right[i]].str + player[right[i]].item.str_buf;
		}
	}
	for (int i = 0; i < left_alive; i++) {
		if (flag[left[i]] == 1) {
			left_str += player[left[i]].str + player[left[i]].item.str_buf;
		}
	}
	print_result_str();
}

void print_result_str() {
	result_str = (-left_str) + right_str;
	printf("str: %10d", result_str);
}

void in_game() {
	key_t key;
	PLAYER;
	
	for (int i = 0; i < 100; i++) {
		Sleep(10);
		gotoxy(5, 15);
		key = get_key();
		if (key == K_QUIT) {
			return;
			break;
		}
		switch (key) {
		case K_Z:					//Z+엔터 왼쪽 srt 1증가
			result_str -= 1; 
			gotoxy(7, 0); 
			printf("str: %10d", result_str); 
			for (int i = 0; i < left_alive; i++) {
				player[left[i]].stamina -= 5;
				if (player[left[i]].stamina < 0) {
					player[left[i]].stamina = 0;
				}
			}
			break;
		case K_SLASH:				//'/'+엔터 오른쪽 srt 1증가
			result_str += 1; 
			gotoxy(7, 0); 
			printf("str: %10d", result_str);
			for (int i = 0; i < right_alive; i++) {
				player[right[i]].stamina -= 5;
				if (player[right[i]].stamina < 0) {
					player[right[i]].stamina = 0;
				}
			}
			break;
		case K_X:					//X+엔터 왼쪽 srt 2배 증가
			left_str = 2 * left_str;
			gotoxy(7, 0);
			print_result_str();
			for (int i = 0; i < left_alive; i++) {
				player[left[i]].stamina -= 30;
				if (player[left[i]].stamina < 0) {
					player[left[i]].stamina = 0;
				}
			}
			break;
		case K_DOT:					//.+엔터 오른쪽 srt 2배 증가
			right_str = 2 * right_str;
			gotoxy(7, 0);
			print_result_str();
			for (int i = 0; i < right_alive; i++) {
				player[right[i]].stamina -= 30;
				if (player[right[i]].stamina < 0) {
					player[right[i]].stamina = 0;
				}
			}
			break;
		}
	}
	print_juldarigi_status(flag);
	move();
	print_juldarigi_status(flag);
}

//플레이어 이동처리
void move() {
	int n_py = standard_py;
	for (int i = 0; i < PLAYER_MAX + 1; i++, n_py++) {
		temp_buf[i] = back_buf[1][n_py];
	}
	if (result_str > 0) {
		n_py = standard_py;
		for (int i = 0; i < PLAYER_MAX + 1; i++,n_py++) {
			back_buf[1][n_py + 1] = temp_buf[i];
		}
		standard_py++;
	}
	else if (result_str < 0) {
		n_py = standard_py;
		for (int i = 0; i < PLAYER_MAX + 1; i++, n_py++) {
			back_buf[1][n_py - 1] = temp_buf[i];
		}
		standard_py--;
	}
	n_py = standard_py;
	//탈락한 플레이어 처리
	if (back_buf[1][15] != '-') {
		for (int i = 0; i < 6; ++i) {
			if (back_buf[1][15] == i + 48) {
				print_juldarigi_status(flag);
				for (int j = 0; j < 3; j++) {
					if (i == left[j]) {
						flag[left[j]] = 0;
						left_alive--;
						//dialog 출력
						char* str = (char*)malloc(sizeof(char) * 100);
						char* str_ = (char*)malloc(sizeof(char) * 50);
						if (!str || !str_) {
							// 메모리 할당 오류 처리
							exit(1);
						}
						sprintf(str, "player ");
						sprintf(str_, "%d", left[j]);
						strcat(str, str_);
						strcat(str, " dead!");
						dialog(str, row, col);
						player_out();
					}
					else if (i == right[j]) {
						flag[right[j]] = 0;
						right_alive--;
						//dialog 출력
						char* str = (char*)malloc(sizeof(char) * 100);
						char* str_ = (char*)malloc(sizeof(char) * 50);
						if (!str || !str_) {
							// 메모리 할당 오류 처리
							exit(1);
						}
						sprintf(str, "player ");
						sprintf(str_, "%d", right[j]);
						strcat(str, str_);
						strcat(str, " dead!");
						dialog(str, row, col);
						player_out();
					}
				}
			}
		}
	}
	char* str = (char*)malloc(sizeof(char) * 100);
	char* str_ = (char*)malloc(sizeof(char) * 50);
	if (!str || !str_) {
		// 메모리 할당 오류 처리
		exit(1);
	}
	for (int i = 0; i < 6; i++) {
		if (player[i].stamina <= 0 && flag[i] == 1) {
			if (i == left[0] || i == left[1] || i == left[2]) {
				left_alive--;
			}
			if (i == right[0] || i == right[1] || i == right[2]) {
				right_alive--;
			}
			sprintf(str, "player ");
			sprintf(str_, "%d", i);
			strcat(str, str_);
			strcat(str, "dead!");
			dialog(str, row, col);
			flag[i] = 0;
			player_out();
		}
	}
	draw();
	print_str();
	Sleep(20);
}

//탈락한 플레이어자리 '-'로 바꿈

void player_out() {
	for (int i = 0; i < 6; i++) {
		if (flag[i] == 0) {
			for (int j = standard_py; j < standard_py + PLAYER_MAX; j++) {
				if (i == back_buf[1][j] - 48) {
					back_buf[1][j] = '-';
				}
			}
		}
	}
}

void player_resurrection() {
	// 탈락했던 플레이어가 게임에서 승리했다면 부활
	if (left_alive != 0) {
		for (int i = 0; i < left_alive; i++) {
			player[left[i]].is_alive = true;
		}
	}
	else if (right_alive != 0) {
		for (int i = 0; i < right_alive; i++) {
			player[right[i]].is_alive = true;
		}
	}
	//탈락하지 않았던 플레이어가 게임에서 탈락했다면 힘과 지능스텟 하락
	for (int i = 0; i < 3; i++) {
		if (player[left[i]].is_alive == true && flag[left[i]] == 0) {
			player[left[i]].str = player[left[i]].str / 2;
			player[left[i]].intel = player[left[i]].intel / 2;
			if (player[left[i]].hasitem == true) {
				player[left[i]].item.intel_buf = 0;
				player[left[i]].item.str_buf = 0;
				player[left[i]].item.stamina_buf = 0;
			}
		}
		if (player[right[i]].is_alive == true && flag[right[i]] == 0) {
			player[right[i]].str = player[right[i]].str / 2;
			player[right[i]].intel = player[right[i]].intel / 2;
			if (player[right[i]].hasitem == true) {
				player[right[i]].item.intel_buf = 0;
				player[right[i]].item.str_buf = 0;
				player[right[i]].item.stamina_buf = 0;
			}
		}
	}
	//탈락했었던 플레이어가 게임에서 탈락했다면 탈락
}

void juldarigi() {
	system("cls");
	for (int i = 0; i < 6; i++) {
		if (player[i].is_alive == false) {
			player[i].stamina = 40;
		}
		if (player[i].is_alive == false && player[i].hasitem == true) {
			player[i].item.str_buf = 0;
			player[i].item.intel_buf = 0;
			player[i].item.stamina_buf = 0;
		}
		flag[i] = 1;
	}
	juldarigi_init();
	draw();
	print_juldarigi_status(flag);
	print_str();
	for (int i = 1; i < 4; i++) {
		gotoxy(10, 10);
		printf("%d", i);
		Sleep(1000);
	}
	gotoxy(10, 10);
	printf("  ");
	while (1) {
		in_game();
		if (left_alive == 0 || right_alive == 0) {
			player_resurrection();
			return;
			break;
		}
	}
}