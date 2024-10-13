#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#pragma warning(disable:4996)

int mainarr[25][120];
int notouch[4][7] = { {0,3,0,2,0,3,0}, {3,0,2,2,2,0,3}, {0,2,2,2,2,2,0}, {2,2,2,2,2,2,2} }; // 큰 장애물 
int notouch2[7][5] = { {2,2,2,2,2}, {2,2,2,2,2}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,3,0,0}, {3,0,2,0,3}, {0,2,2,2,0} }; // 작은 장애물 
int i, j, height = 0, updown = 0, waitforstruct = 0, istherestruct = 0, score = 0, curtime, jellywait = 0, health = 3, waitforheal = 0;
char jumpkey = 'w', jumpkey2 = 'q';

int isitgameover() // 장애물에 닿았는지 확인
{
	if ((mainarr[20 - height][4] == 2 || mainarr[20 - height][3] == 2 || mainarr[19 - height][4] == 2 || mainarr[19 - height][3] == 2) && waitforheal >= 15)
	{
		health--;
		waitforheal = 0;
		if (health == 0)
		{
			return 1;
		}
	}
	return 0;
}

void isititem() // 아이템을 먹었는지 확인
{
	if (mainarr[20 - height][4] == 3 || mainarr[20 - height][3] == 3 || mainarr[19 - height][4] == 3 || mainarr[19 - height][3] == 3) // 아이템이 다음 프레임에 캐릭터한테 닿으면 
	{
		if (height > 0) // 장애물에 있는 젤리는 보너스 줌 
		{
			score += 300;
		}
		else // 젤리 먹음 (장애물 x)
		{
			score += 50;
		}
	}
}

void SetConsole() // 콘솔창 세팅 
{
	system("title Temu Untitled Game");
	system("mode con:cols=130 lines=35");
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 0; // 커서 안보임 
	ConsoleCursor.dwSize = 0; // 사이즈 0 
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

void gotoxy(int x, int y)
{
	HANDLE consoleHandle = GetStdHandle;
	COORD Cur = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void jump() // 점프
{
	if (updown == 1 || updown == 3) // 올라갈때 
	{
		height++; // 높이++ 
		for (j = 0; j < 2; j++)
		{
			mainarr[19 - height + 2][2 + j] = 0; //
		}
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				mainarr[19 - height + i][2 + j] = 4;
			}
			printf("\n");
		}
		if (updown == 3 && height >= 4)
		{
			updown++;
		}
		if (height >= 7)
		{
			updown++;
		}
	}
	if (updown == 2 || updown == 4)
	{
		for (j = 0; j < 2; j++)
		{
			mainarr[19 - height][2 + j] = 0;
		}
		height--;
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				mainarr[19 - height + i][2 + j] = 4;
			}
			printf("\n");
		}
		if (height == 0)
		{
			updown = 0;
		}
	}
};

int randomstruct() // 장애물 랜덤생성 (생성 성공하면 return 1)
{
	if (waitforstruct >= 25 && rand() % 8 == 0)
	{
		waitforstruct = 0;
		istherestruct = 1;
		return 1;
	}
	waitforstruct++;
	return 0;
}

void settingthegame() // 게임 세팅하는거 (변수 초기화, 이차원 배열 초기화)
{
	height = 0;
	updown = 1;
	waitforstruct = 0;
	istherestruct = 0;
	score = 0;

	// 바닥 만들기

	for (i = 0; i < 25; i++)
	{
		for (j = 0; j < 120; j++)
		{
			mainarr[i][j] = 0;
		}
	}
	for (i = 0; i < 120; i++)
	{
		mainarr[21][i] = 1;
	}
	for (i = 0; i < 40; i++)
	{
		mainarr[20][i * 3] = 3;
	}
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 2; j++)
		{
			mainarr[19 + i][2 + j] = 4;
		}
	}
}

void generatejelly() // 젤리 생성 
{
	if (jellywait == 2)
	{
		mainarr[20][118] = 3;
		jellywait = 0;
	}
	else
	{
		jellywait++;
	}
}

void generatebadstuff() // 장애물 생성 
{
	if (rand() % 2 == 0) // 큰 장애물
	{
		for (i = 0; i < 9; i++) // 장애물 자리에 있는 젤리 없애기
		{
			mainarr[20][110 + i] = 0;
		}
		for (i = 0; i < 4; i++) // 장애물 생성
		{
			for (j = 0; j < 7; j++)
			{
				mainarr[17 + i][111 + j] = notouch[i][j];
			}
		}
	}
	else if (rand() % 2 == 1) // 작은 장애물
	{
		for (i = 0; i < 7; i++) // 장애물 자리에 있는 젤리 없애기
		{
			mainarr[20][111 + i] = 0;
		}
		for (i = 0; i < 7; i++) // 장애물 생성
		{
			for (j = 0; j < 5; j++)
			{
				mainarr[14 + i][112 + j] = notouch2[i][j];
			}
		}
	}
}

int main()
{
	srand(time(NULL));
	int random;
	SetConsole();
	while (1)
	{
		// 게임 시작할때 정수 초기화 
		int gameover = 0;
		generatejelly();
		printf("Press any key to start the game.\nJumping = 'q' or 'w'\nif you can't jump, chect if caps lock is on.\n\n"); // 설명 
		system("pause");
		system("cls");
		Sleep(100);
		settingthegame();
		while (1)
		{
			gotoxy(1, 1);
			printf("score: %d", score); // score 출력 
			for (i = 0; i < health; i++)
			{
				gotoxy(1+i*2, 3);
				printf("♥");
			}
			for (i = 0; i < 25; i++) // mainarr 출력 
			{
				for (j = 0; j < 120; j++)
				{
					if (mainarr[i][j] == 0)
					{
						continue;
					}
					gotoxy(j + 5, i);
					if (mainarr[i][j] == 1)
					{
						printf("￣");
					}
					if (mainarr[i][j] == 2)
					{
						printf("!");
					}
					if (mainarr[i][j] == 3)
					{
						printf("*");
					}
					if (mainarr[i][j] == 4)
					{
						printf("@");
					}
				}
				printf("\n");
			}
			int a = 0;
			if (_kbhit())
			{
				a = _getch();
			}
			if ((a == jumpkey) && height == 0) // 점프조건 
			{
				updown = 1;
			}
			if ((a == jumpkey2) && height == 0) // 작은점프조건 
			{
				updown = 3;
			}
			jump(); // 점프 신호에 맞춰서 캐릭터 위치 조정 
			generatejelly(); 
			if (randomstruct() == 1) // 장애물 생성해야할때
			{
				generatebadstuff();
			}
			gameover = isitgameover(); // 장애물에 부딪혔는지 체크 
			isititem(); // 아이템을 먹었는지 체크 
			for (i = 0; i < 25; i++) // 오브젝트의 x좌표를 -1씩 바꾸기 
			{
				for (j = 0; j < 119; j++)
				{
					if (mainarr[i][j] == 4 || mainarr[i][j + 1] == 4)
					{
						continue;
					}
					mainarr[i][j] = mainarr[i][j + 1];
			
				}
			}
			for (i = 0; i < 2; i++) // 플레이어 칸은 유지시키기 
			{
				mainarr[19 + i - height][2] = 4;
				mainarr[19 + 1 - height][0] = 0;
			}
			for (j = 0; j < 6; j++) // 플레이어를 지난 오프젝트는 삭제됨
			{
				mainarr[15 + j][1] = 0;
			}
			if (gameover == 1) // 게임오버면 while문 나가기 
			{
				break;
			}
			curtime += 1;
			waitforheal += 1;
			if (curtime > 2) // 일정프레임마다 스코어 추가 
			{
				score++;
			}
			system("cls");
		}
		system("cls");
		printf("Game Over\n");
		Sleep(1000);
		printf("your score : % d\n\nretry: 'r'\nend game : any key except 'r'", score);
		while (1)
		{
			char b = 0;
			b = _getch();
			if (b == 'r' || b == 'R') // retry
			{
				system("cls");
				break;
			}
			if (b != 'r' && b != 'R') // 딴거 누르면 끝남 
			{
				return 0;
			}
		}
	}
}
