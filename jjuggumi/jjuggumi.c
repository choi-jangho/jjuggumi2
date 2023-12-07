// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);
void intro(void);
void ending();
void playerRest();

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init() {
	srand((unsigned int)time(NULL));

	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1; // -1 리턴하면 메인함수에서 처리하고 종료
	}

	// 플레이어 데이터 load
	fscanf_s(fp, "%d", &n_player);
	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player[i];
		// 파일에서 각 스탯 최댓값 읽기
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name),
			&(p->intel), &(p->str));
		p->stamina = 100; // 100%
		// 현재 상태
		p->is_alive = true;
		p->hasitem = false;
	}

	// 아이템 데이터 load
	fscanf_s(fp, "%d", &n_item);
	for (int i = 0; i < n_item; i++) {
		fscanf_s(fp, "%s%d%d%d",
			item[i].name, (unsigned int)sizeof(item[i].name),
			&(item[i].intel_buf),
			&(item[i].str_buf),
			&(item[i].stamina_buf));
	}
	
	fclose(fp);
	return 0;
}

void printAsciiArt(const char* art[]) {
	for (int i = 0; i < 5; i++) {
		printf("%s\n", art[i]);
	}
}

void intro(void) {
	system("cls");
	const char* countdownArt[3][5][5] = {
		{
		"  @ ",
		" @@ ",
		"  @ ",
		"  @ ",
		"@@@@"
		},{
		"@@@@",
		"   @",
		" @@@",
		"@   ",
		"@@@@"
		},{
		"@@@@",
		"   @",
		" @@@",
		"   @",
		"@@@@"
		}
	};

	// 배열 출력
	system("cls");
	for (int i = 2; i >= 0; i--) {
		printAsciiArt(countdownArt[i]);
		Sleep(1000); // 1초 대기
		system("cls"); // 화면 지우기 (Windows 환경에서 동작)
	}
	for (int i = 0; i < 3; i++)
	{
		printf(
		"  ####   ####  ##   ##   #####    #####   ##   ##  ##   ##   ######            #####    #####   ##   ##  #######\n"
		"   ##     ##   ##   ##  ##   ##  ##   ##  ##   ##  ### ###     ##             ##   ##  ##   ##  ### ###   ##  ##\n"
		"   ##     ##   ##   ##  ##       ##       ##   ##  #######     ##             ##       ##   ##  #######   ##\n"
		"   ##     ##   ##   ##  ##  ###  ##  ###  ##   ##  ## # ##     ##             ##  ###  #######  ## # ##   ####\n"
		"   ##     ##   ##   ##  ##   ##  ##   ##  ##   ##  ##   ##     ##             ##   ##  ##   ##  ##   ##   ##\n"
		"## ##  ## ##   ##   ##  ##   ##  ##   ##  ##   ##  ##   ##     ##             ##   ##  ##   ##  ##   ##   ##  ##\n"
		" ###    ###     #####    #####    #####    #####   ##   ##   ######            #####   ##   ##  ##   ##  #######\n"
		);
		Sleep(1000);
		system("cls");
	}
}

void ending()
{
	system("cls");
	printf("Game Finished!\n");

	if (n_alive == 1)
	{
		int winner;
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			if (player[i].is_alive)
			{
				winner = i;
				break;
			}
		}
		printf("The WINNER is ( %d )", winner);
		exit(1);
	}
}

void playerRest() {
	for (int i = 0; i < n_player; i++) {
		player[i].stamina += 40;
		if (player[i].stamina > 100) {
			player[i].stamina = 100;
		}
		if (player[i].stamina < 0 && player[i].is_alive == true) {
			player[i].stamina = 40;
		}
	}
}

int main(void) {
	jjuggumi_init();
	intro();
	/*sample();*/
	mugunghwa();
	playerRest();
	nightgame();
	playerRest();
	juldarigi();
	playerRest();
	jebi();
	ending();
	return 0;
}
