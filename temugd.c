#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<time.h>

// 만들고 싶은거: slide motion (캐릭터 높이가 1칸이 되고, slide 키는 's'가 될 예정. 장애물도 추가 할거임) 

int mainarr[25][120], character[2][2] = { {4,4},{4,4} };
int notouch[4][7] = { {0,0,0,2,0,0,0}, {0,0,2,2,2,0,0}, {0,2,2,2,2,2,0}, {2,2,2,2,2,2,2} };
int i, j, height=0, updown=1, waitforstruct=0, istherestruct=0, getsu = 0, score=0, curtime;

typedef struct character
{
	int char_x;
	int char_y;
} Character;

typedef struct enemy
{
	int char_x;
	int item;
} enemy;

void SetConsole()
{
	system("title Temu Geometry Dash");
	system("mode con:cols=130 lines=35");
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 1;
	ConsoleCursor.dwSize = 1;
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

void gotoxy(int x, int y)
{
	HANDLE consoleHandle= GetStdHandle;
	COORD Cur={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void jump(Character mainone)
{
	if(updown == 1)
	{
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				gotoxy(mainone.char_x + j + 5, mainone.char_y - height + i);
				printf(" ");
				mainarr[mainone.char_y - height + i][mainone.char_x + j] = 0;
			}
		}
		height++;
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				gotoxy(mainone.char_x + j + 5, mainone.char_y - height + i);
				printf("4");
				mainarr[mainone.char_y - height + i][mainone.char_x + j] = 4;
			}
			printf("\n");
		}
		if(height >= 7)
		{
			updown++;
		}
	}
	if(updown == 2)
	{
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				gotoxy(mainone.char_x + j + 5, mainone.char_y - height + i);
				printf(" ");
				mainarr[mainone.char_y - height + i][mainone.char_x + j] = 0;
			}
		}
		height--;
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				gotoxy(mainone.char_x + j + 5, mainone.char_y - height + i);
				printf("4");
				mainarr[mainone.char_y - height + i][mainone.char_x + j] = 4;
			}
			printf("\n");
		}
		if(height==0)
		{
			updown--;
		}
	}
};

void randomstruct(enemy structure[3])
{
	if(waitforstruct >= 25 && rand() % 8 == 0 && getsu < 3)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 7; j++)
			{
				if(i==0 && (j==1 || j==5) && mainarr[17][108] != 3 && rand() % 3 == 0)
				{
					mainarr[17+i][107+j] = 3;
					structure[getsu].item = 107+j;
					continue;
				}
				mainarr[17+i][107+j] = notouch[i][j];
			}
		}
		waitforstruct=0;
		structure[getsu].char_x = 107;
		istherestruct=1;
		getsu++;
	}
	waitforstruct++;
}

int main()
{
	int retry=1;
	SetConsole();
	while (1)
	{
		int gameover = 0;
		height=0;
		updown=1;
		waitforstruct=0;
		istherestruct=0;
		getsu = 0;
		char jumpkey = 'w';
		Character mainone;
		enemy structure[3];
		mainone.char_x = 2;
		mainone.char_y = 19;
		for(i=0;i<3;i++)
		{
			structure[i].item = 110;
		}
		structure[0].char_x = 106;
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
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				mainarr[mainone.char_y+i][mainone.char_x+j] = 4;
			}
		}
		printf("Press any key to start the game.\nJumping = 'w'\nif you can't jump, chect if caps lock is on.\n\n");
		system("pause");
		system("cls");
		Sleep(100);
		while (retry == 1)
		{
			gotoxy(1,1);
			printf("score: %d", score);
			for (i = 0; i < 25; i++)
			{
				for (j = 0; j < 120; j++)
				{
					if (mainarr[i][j] == 0)
					{
						continue;
					}
					gotoxy(j + 5, i);
					printf("%d", mainarr[i][j]);
				}
				printf("\n");
			}
			if ((kbhit() == 1 && getch() == jumpkey) || height > 0)
			{
				jump(mainone);
			}
			randomstruct(structure);
			if(height == 0) // 플레이어의 높이에 따라서 장애물에 닿았는지 보기 
			{
				if(structure[0].char_x <= 4 && structure[0].char_x >= -3)
				{
					gameover = 1;
					break;
				}
			}
			else if(height == 1) // 플레이어의 높이에 따라서 장애물에 닿았는지 보기 
			{
				if(structure[0].char_x <= 3 && structure[0].char_x >= -2)
				{
					gameover = 1;
					break;
				}
			}
			else if(height == 2) // 플레이어의 높이에 따라서 장애물에 닿았는지 보기 
			{
				if(structure[0].char_x <= 2 && structure[0].char_x >= -1)
				{
					gameover = 1;
					break;
				}
			}
			else if(height == 3) // 플레이어의 높이에 따라서 장애물에 닿았는지 보기 
			{
				if(structure[0].char_x <= 1 && structure[0].char_x >= 0)
				{
					gameover = 1;
					break;
				}
			}
			for (i = 0; i < 25; i++)
			{
				for (j = 0; j < 119; j++)
				{
					if(mainarr[i][j] == 4 || mainarr[i][j+1] == 4)
					{
						continue;
					}
					mainarr[i][j] = mainarr[i][j+1];
				}
			}
			for(i=0;i<2;i++) // 플레이어 칸은 유지시키기 
			{
				mainarr[19+i-height][2] = 4;
				mainarr[19+1-height][0] = 0;
			}
			if((structure[0].item == 3 || structure[0].item == 4) && (height == 2 || height == 3)) // 아이템이 플레이어한테 닿았는지 여부 
			{
				score += 200; // 스코어 추가 
				structure[0].item = 1000; // 한번 먹으면 없어짐 
			}
			if(gameover == 1) // 게임오버면 while문 나가기 
			{
				break;
			}
			if(structure[0].char_x <= -4) // 장애물이 플레이어한테서 벗어나면 없애기 
			{
				getsu--;
				if(getsu == 0)
				{
					istherestruct = 0;
				}
				for(i=0;i<getsu;i++)
				{
					structure[i].char_x = structure[i+1].char_x;
					structure[i].item = structure[i+1].item;
				}
				structure[getsu+1].char_x = 107;
				structure[getsu+1].item = 110;
			}
			if(istherestruct == 1) // 장애물이 있을때 플레이어에게 가까워지게 하기 
			{
				for(i=0;i<getsu;i++)
				{
					structure[i].char_x--;
					structure[i].item--;
				}
			}
			curtime += 1;
			if(curtime > 8)
			{
				score++;
			}
			Sleep(10);
			system("cls");
		}
		system("cls");
		Sleep(300);
		printf("Game Over\nyour score: %d\n\nretry: 'r'\nend game: esc", score);
		while(1)
		{
			if(getch() == 'r' || getch() == 'R')
			{
				system("cls");
				retry = 1;
				score=0;
				break;
			}
			if(getch() == 27)
			{
				return 0;
			}
		}
	}
}
