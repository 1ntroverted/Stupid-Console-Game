#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#pragma warning(disable:4996)

int mainarr[10][120];
int obstacle[4][7] = { {0,3,0,2,0,3,0}, {3,0,2,2,2,0,3}, {0,2,2,2,2,2,0}, {2,2,2,2,2,2,2} }; // 큰 장애물 
int obstacle2[7][5] = { {2,2,2,2,2}, {2,2,2,2,2}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,3,0,0}, {3,0,2,0,3}, {0,2,2,2,0} }; // 작은 장애물 
int i, j, k, height = 0, updown = 0, waitforstruct = 0, score = 0, curtime, jellywait = 0, health = 3, waitforheal = 0, input = 0;

typedef struct coordin {
	int x;
	int y;
} coord;

coord player[4];

int enemytouchcheck() // 장애물에 닿았는지 확인
{
	for (i = 0; i < 4; i++)
	{
		if (mainarr[player[i].y - 12][player[i].x - 5] == 2 && waitforheal >= 15)
		{
			health--;
			waitforheal = 0;
			if (health <= 0)
			{
				return 1;
			}
		}
	}
	return 0;
}

void itemcheck() // 아이템을 먹었는지 확인
{
	for (i = 0; i < 4; i++)
	{
		if (mainarr[player[i].y - 12][player[i].x - 5] == 3)
		{
			mainarr[player[i].y - 12][player[i].x - 5] = 0;
			if (height > 0) // 장애물에 있는 아이템은 보너스 줌
			{
				score += 500;
				break;
			}
			else // 바닥에 있는 아이템 먹음
			{
				score += 30;
				break;
			}
		}
	}
}

int waveenemytouchcheck() // 장애물에 닿았는지 확인
{
	if (mainarr[player[0].y - 12][player[0].x - 5] == 2)
	{
		health--;
		if (health <= 0)
		{
			return 1;
		}
	}
	return 0;
}

void waveitemcheck() // 아이템을 먹었는지 확인
{
	if (mainarr[player[0].y - 12][player[0].x - 5] == 3)
	{
		score += 300;
		mainarr[player[0].y - 12][player[0].x - 5] = 0;
	}
}

void SetConsole() // 콘솔창 세팅 
{
	system("title ");
	system("mode con:cols=130 lines=28");
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 1;
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

void gotoxy(int x, int y)
{
	HANDLE consoleHandle = GetStdHandle;
	COORD Cur = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void jump() // 점프 신호에 맞춰서 캐릭터가 올라가거나 내려가는 함수 
{
	if (updown == 1 || updown == 3) // 올라갈때 (홀수)
	{
		height++; // 높이 올라감
		for (i = 0; i < 4; i++)
		{
			player[i].y--; // 플레이어 y좌표 바꾸기
		}
		if (updown == 3 && height >= 4) // 최고 높이 도달 (작은 점프)
			updown++;
		if (height >= 7) // 최고 높이 도달 
			updown++;
	}
	if (updown == 2 || updown == 4) // 내려갈때 (짝수) 
	{
		height--; // 높이 내려감 
		for (i = 0; i < 4; i++)
		{
			player[i].y++; // 플레이어 y좌표 바꾸기 
		}
		if (height == 0) // 바닥까지 떨어졌을때 
			updown = 0; // 점프 신호 초기화 
	}
}

void wave() // jump 함수 웨이브 모드 버전 
{
	if (updown == 1) // 올라갈때
	{
		if (height < 8) // 천장에 닿으면 안 올라가도록 높이제한 걸기
		{
			player[0].y--;
			height++;
		}
	}
	if (updown == 2) // 내려갈때
	{
		if (height > 0) // 바닥에 닿으면 더 내려가지 않게 설정
		{
			player[0].y++;
			height--;
		}
	}
}

int randomstruct() // 장애물 랜덤생성 (생성 성공하면 return 1)
{
	if (waitforstruct >= 25 && rand() % 8 == 0)
	{
		waitforstruct = 0;
		return 1;
	}
	waitforstruct++;
	return 0;
}

void settingthegame() // 게임 세팅하는거 (변수 초기화, 이차원 배열 초기화)
{
	height = 0; // 높이값 
	updown = 1; // 점프 여부 
	waitforstruct = 0;
	score = 0;
	health = 3;

	// 바닥 만들기

	for (i = 0; i < 10; i++) // mainarr 세팅 
	{
		for (j = 0; j < 120; j++)
		{
			mainarr[i][j] = 0;
		}
	}
	for (i = 0; i < 120; i++) // 바닥 생성 
	{
		mainarr[9][i] = 1;
	}
}

void generate_jelly() // 아이템 생성 
{
	if (jellywait == 2) // 일정시간 지남 
	{
		mainarr[8][118] = 3;
		jellywait = 0;
	}
	else
	{
		jellywait++;
	}
}

void generate_waveobstacle() // wave 장애물, 아이템 생성 
{
	if (waitforstruct >= 10) // 조건 1
	{
		waitforstruct = 0;
		if (rand() % 2 == 0) // 조건 2 (1/2 확률로 생성됨) 
		{
			int waveobstapos = rand() % 6;
			for (i = 0; i < waveobstapos; i++)
			{
				mainarr[i][111] = 2;
			}
			for (i = waveobstapos + 3; i < 9; i++)
			{
				mainarr[i][111] = 2;
			}
		}
	}
	else
	{
		if (rand() % 3 == 0)
		{
			mainarr[rand() % 9][111] = 3;
		}
		waitforstruct++;
	}
}

void generate_obstacle() // 장애물 생성 
{
	if (rand() % 2 == 0) // 큰 장애물
	{
		for (i = 0; i < 9; i++) // 장애물 자리에 있는 아이템 없애기
		{
			mainarr[8][110 + i] = 0;
		}
		for (i = 0; i < 4; i++) // 장애물 생성 (아이템 포함) 
		{
			for (j = 0; j < 7; j++)
			{
				mainarr[5 + i][111 + j] = obstacle[i][j];
			}
		}
	}
	else if (rand() % 2 == 1) // 작은 장애물
	{
		for (i = 0; i < 7; i++) // 장애물 자리에 있는 아이템 없애기
		{
			mainarr[8][111 + i] = 0;
		}
		for (i = 0; i < 7; i++) // 장애물 생성 (아이템 포함) 
		{
			for (j = 0; j < 5; j++)
			{
				mainarr[2 + i][112 + j] = obstacle2[i][j];
			}
		}
	}
}

void health_and_score()
{
	gotoxy(1, 1);
	printf("score: \033[96m%d\033[0m", score); // score 출력 
	for (i = 0; i < 3; i++)
	{
		gotoxy(1 + i * 2, 3);
		printf("  ");
	}
	for (i = 0; i < health; i++) // 남은 목숨 출력 
	{
		gotoxy(1 + i * 2, 3);
		printf("\033[31m♥\033[0m");
	}
	if (input == 1) // 점프모드 
	{
		gotoxy(5, 23);
		printf("Jump Keys : 'q' for small jump, 'w' for normal jump.");
		gotoxy(5, 24);
		printf("\033[33mif you can't jump, check if caps lock is on.\033[0m");
	}
	else if (input == 2) // 웨이브모드 
	{
		gotoxy(5, 23);
		printf("Jump Key : hold 'w' or space bar to go up");
		gotoxy(5, 24);
		printf("\033[33mif you can't go up, check if caps lock is on.\033[0m");
	}
}

int main()
{
	srand(time(NULL));
	SetConsole();
	while (1)
	{
		printf("choose the mode by typing 1 or 2.\n");
		printf("1. runner mode\n");
		printf("2. wave mode\n");
		while (1) // 입력 받기 
		{
			if (GetAsyncKeyState(0x31) & 0x8000)
			{
				input = 1;
				break;
			}
			else if (GetAsyncKeyState(0x32) & 0x8000)
			{
				input = 2;
				break;
			}
		}
		if (input == 1) // 러너 모드 
		{
			system("cls");
			// 플레이어 좌표 기본 설정 
			for (i = 0; i < 2; i++)
			{
				player[i].x = 7;
				player[i].y = 19 + i;
			}
			for (i = 0; i < 2; i++)
			{
				player[i + 2].x = 8;
				player[i + 2].y = 19 + i;
			}
			int gameover = 0;
			settingthegame(); // 이차원 배열, 정수를 기본 설정으로 설정하기 
			while (1)
			{
				if ((GetAsyncKeyState(0x57) & 0x8000) && height == 0) // 점프조건 
				{
					updown = 1;
				}
				if ((GetAsyncKeyState(0x51) & 0x8000) && height == 0) // 작은점프조건 
				{
					updown = 3;
				}
				if (updown != 0)
				{
					jump(); // 점프 신호에 맞춰서 캐릭터 위치 조정 
				}
				generate_jelly(); // 일정 시간마다 아이템 추가 
				if (randomstruct() == 1) // 장애물 생성해야할때
				{
					generate_obstacle(); // 장애물을 생성시키는 함수 실행 
				}
				itemcheck(); // 아이템을 먹었는지 체크 
				for (i = 0; i < 10; i++) // 오브젝트의 x좌표를 -1씩 바꾸기 
				{
					for (j = 0; j < 119; j++)
					{
						mainarr[i][j] = mainarr[i][j + 1];
					}
				}
				for (j = 0; j < 9; j++)
				{
					mainarr[j][0] = 0; // 플레이어를 지난 오브젝트는 삭제됨
				}
				for (i = 0; i < 10; i++) // 이차원 배열(스크린) 출력 
				{
					for (j = 0; j < 120; j++)
					{
						int skip = 0;
						gotoxy(j + 5, i + 12);
						for (k = 0; k < 4; k++)
						{
							if (j + 5 == player[k].x && i + 12 == player[k].y) // 이차원 배열이랑 함께 플레이어 출력 
							{
								printf("@");
								skip = 1;
								break;
							}
						}
						if (skip == 1) // 플레이어 칸은 이미 출력했으니 다음 칸으로 가기 
						{
							continue;
						}
						if (mainarr[i][j] == 0) // 아무것도 없음 
						{
							printf("  ");
							continue;
						}
						if (mainarr[i][j] == 1) // 바닥 
						{
							printf("￣");
							continue;
						}
						if (mainarr[i][j] == 2) // 장애물 
						{
							printf("\033[91m!\033[0m");
							continue;
						}
						if (mainarr[i][j] == 3) // 아이템 
						{
							printf("\033[33m*\033[0m");
							continue;
						}
					}
					printf("\n");
				}
				gameover = enemytouchcheck(); // 장애물에 부딪혔는지 체크 
				health_and_score();
				if (gameover == 1) // 게임오버면 while문 나가기 
				{
					break;
				}
				curtime += 1; // 프레임마다 추가되는 변수 
				waitforheal += 1; // 장애물에 닿으면 일정 시간동안 장애물 판정을 무시하는 변수 
				if (curtime > 2) // 일정프레임마다 스코어 추가 
				{
					score++;
				}
			}
			system("cls");
			Sleep(300);
			printf("Game Over\n");
			printf("your score : \033[96m%d\033[0m\n\nretry: 'r'\n\n", score);
			printf("quit: 'esc'");
			while (1)
			{
				if (GetAsyncKeyState(0x52) & 0x8000)
				{
					system("cls");
					break;
				}
				else if (GetAsyncKeyState(0x1B) & 0x8000) // esc 누르면 게임 끝 
				{
					return 0;
				}
			}
			Sleep(200);
		}

		if (input == 2) // 웨이브 모드 
		{
			system("cls");
			// 플레이어 좌표 기본 설정
			player[0].x = 8;
			player[0].y = 20;
			int gameover = 0;
			settingthegame(); // 이차원 배열, 정수를 기본 설정으로 설정하기
			while (1)
			{
				if (GetAsyncKeyState(0x57) & 0x8000) // 점프조건 
				{
					updown = 1;
				}
				else if (GetAsyncKeyState(0x20) & 0x8000) // 점프조건 2
				{
					updown = 1;
				}
				else
				{
					updown = 2;
				}
				generate_waveobstacle(); // 장애물, 아이템 생성 
				waveitemcheck(); // 아이템을 먹었는지 체크 
				if (updown != 0)
				{
					wave(); // 점프 신호에 맞춰서 캐릭터 위치 조정 
				}
				for (i = 0; i < 10; i++) // 오브젝트의 x좌표를 -1씩 바꾸기 
				{
					for (j = 0; j < 119; j++)
					{
						mainarr[i][j] = mainarr[i][j + 1];
					}
				}
				for (j = 0; j < 9; j++)
				{
					mainarr[j][0] = 0; // 플레이어를 지난 오브젝트는 삭제됨
				}
				for (i = 0; i < 120; i++)
				{
					gotoxy(i + 5, 11);
					printf("_");
				}
				for (i = 0; i < 10; i++) // 이차원 배열(스크린) 출력 
				{
					for (j = 0; j < 120; j++)
					{
						gotoxy(j + 5, i + 12);
						if (j + 5 == player[0].x && i + 12 == player[0].y) // 이차원 배열이랑 함께 플레이어 출력 
						{
							printf("@");
							continue;
						}
						if (mainarr[i][j] == 0) // 아무것도 없음 
						{
							printf("  ");
							continue;
						}
						if (mainarr[i][j] == 1) // 바닥 
						{
							printf("￣");
							continue;
						}
						if (mainarr[i][j] == 2) // 장애물 
						{
							printf("\033[91m!\033[0m");
							continue;
						}
						if (mainarr[i][j] == 3) // 아이템 
						{
							printf("\033[33m*\033[0m");
							continue;
						}
					}
					printf("\n");
				}
				gameover = waveenemytouchcheck(); // 장애물에 부딪혔는지 체크
				health_and_score();
				if (gameover == 1) // 게임오버면 while문 나가기 
				{
					break;
				}
				score += 2; // 점수 올리기 
			}
			system("cls");
			Sleep(300);
			printf("Game Over\n");
			printf("your score : \033[96m%d\033[0m\n\nretry: 'r'\n", score);
			printf("quit: 'esc'");
			while (1)
			{
				if (GetAsyncKeyState(0x52) & 0x8000)
				{
					system("cls");
					break;
				}
				else if (GetAsyncKeyState(0x1B) & 0x8000) // esc 누르면 게임 끝 
				{
					return 0;
				}
			}
			Sleep(200);
		}
		else // 잘못 골랐을때 
		{
			printf("choose 1 or 2");
			input = 0;
			Sleep(500);
			system("cls");
		}
	}
}
