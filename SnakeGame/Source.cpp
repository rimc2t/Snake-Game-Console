#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include "console.h"

using namespace std;

#define frameWidth 50;
#define frameHeight 29;

int w = frameWidth;
int h = frameHeight;

enum STATUS {UP, DOWN, LEFT, RIGHT};

struct COORDINATE
{
	int x, y;				//Toa do
};

struct SNAKE
{
	COORDINATE dot[10000];
	STATUS stt;				//Trang thai cua ran (left, right, up down)
	int len;				//Do dai cua ran
	int speed;				//Toc do cua ran
};

struct PLAYER
{
	string namePlayer;		//Ten nguoi choi
	int score;				//Diem nguoi choi
	int bestScore;			//Diem cao nhat cua nguoi choi
	int timePlay;			//So lan choi
};

struct FOOD
{
	COORDINATE f;			//Toa do cua thuc an
};

void initialize(SNAKE &snake, FOOD &food, PLAYER &player)
{
	snake.len = 1;
	snake.dot[0].x = w / 2;
	snake.dot[0].y = h / 2;
	snake.stt = RIGHT;
	snake.speed = 200;

	if (player.timePlay > 1);
	else
	{
		player.bestScore = 0;
		player.timePlay = 1;
	}
	player.score = 0;

	food.f.x = rand() % (w - 2) + 1;
	food.f.y = rand() % (h - 2) + 1;

	return;
}

void printPlayZoneFrame()
{
	//PRINT TOP
	gotoXY(0, 0);
	putchar(201);
	for (int i = 1; i < w; i++) putchar(205);
	putchar(187);
	
	//PRINT LEFT AND RIGHT
	for (int i = 1; i < h; i++)
	{
		gotoXY(0, i);
		putchar(186);
		gotoXY(w, i);
		putchar(186);
	}

	//PRINT BOTTOM
	gotoXY(0, h);
	putchar(200);
	for (int i = 1; i < w; i++) putchar(205);
	putchar(188);

	return;
}

void typeNamePlayer(PLAYER &player)
{
	TextColor(13);
	clearZone(1, 1, w - 1, h - 1);
	//TYPE NAME
	gotoXY(w / 2 - 10, h / 2);
	printf("Type your name: ");
	cin >> player.namePlayer;

	//COUNTDOWN
	{
		int countDown = 3;
		while (countDown)
		{
			clearZone(1, 1, w - 1, h - 1);
			Sleep(200);
			gotoXY(w / 2 - 10, h / 2);
			printf("THE GAME START IN %d..", countDown);
			countDown--;
			Sleep(1000);
		}
	}

	return;
}

void scrHighestScore()
{
	fstream fileHScore;
	string namePlayer;
	int score = 0;

	fileHScore.open("HighestScore.txt", ios::in || ios::out);
	fileHScore >> namePlayer;
	fileHScore >> score;
	fileHScore.close();

	if (score)
	{
		TextColor(13);
		clearZone(1, 1, w - 1, h - 1);
		gotoXY(w / 2 - 10, h / 2);
		cout << "Name:  " << namePlayer;
		gotoXY(w / 2 - 10, h / 2 + 1);
		cout << "Score: " << score;
		gotoXY(w / 2 - 10, h / 2 + 3);
		cout << "Press 'BLANK SPACE' to BACK";
		while (true)
		{
			if (_kbhit())
			{
				int c = _getch();
				if (c == 32) return;
			}
		}
	}
	else
	{
		TextColor(13);
		clearZone(1, 1, w - 1, h - 1);
		gotoXY(w / 2 - 10, h / 2);
		cout << "NOBODY played";
		gotoXY(w / 2 - 10, h / 2 + 1);
		cout << "Press 'BLANK SPACE' to BACK";
		while (true)
		{
			if (_kbhit())
			{
				int c = _getch();
				if (c == 32) return;
			}
		}
	}
}

void scrLevel(SNAKE &snake)
{
	clearZone(1, 1, w - 1, h - 1);

	TextColor(13);
	gotoXY(w / 2 - 10, h / 2 - 2);
	if (snake.speed <= 100)
		printf("You are playing on Hard level");
	else if (snake.speed <= 200)
		printf("You are playing on Medium level");
	else
		printf("You are playing on Easy level");

	gotoXY(w / 2 - 10, h / 2 - 1);
	printf("EASY		(1)");

	gotoXY(w / 2 - 10, h / 2);
	printf("MEDIUM		(2)");

	gotoXY(w / 2 - 10, h / 2 + 1);
	printf("HARD		(3)");

	gotoXY(w / 2 - 10, h / 2 + 3);
	printf("Press 'NUMBER' to choose");

	gotoXY(w / 2 - 10, h / 2 + 4);
	printf("Press 'BLANK SPACE' to BACK");

	while (true)
	{
		if (_kbhit())
		{
			int c = _getch();
			if (c == 49)
			{
				snake.speed = 300;
				break;
			}
			else if (c == 50)
			{
				snake.speed = 200;
				break;
			}
			else if (c == 51)
			{
				snake.speed = 100;
				break;
			}
			else if (c == 32) break;
		}
	}
}

void scrBeforePlay(SNAKE &snake, PLAYER &player)
{
	clearZone(1, 1, w - 1, h - 1);

	printPlayZoneFrame(); //Ve khung cho khu vuc choi

	TextColor(11);
	//PRINT HEADER
	{
		gotoXY(w + 1, 0);
		putchar(214);
		for (int i = (w + 2); i < 118; i++) putchar(196);
		putchar(183);

		gotoXY(w + 1, 1);
		putchar(186);

		gotoXY(118, 1);
		putchar(186);

		gotoXY(w + 1, 2);
		putchar(211);
		for (int i = (w + 2); i < 118; i++) putchar(196);
		putchar(189);

		gotoXY((118 + w) / 2 - 3, 1);
		printf("SNAKE GAME");
	}

	TextColor(14);
	//PRINT CONTROL
	{
		gotoXY(w + 1, 9);
		printf("*CONTROL:");
		gotoXY(w + 1, 10);
		printf("  A/D/S/W : LEFT/RIGHT/DOWN/UP");
		gotoXY(w + 1, 11);
		printf("  P       : PAUSE/CONTINUE");
		gotoXY(w + 1, 12);
		printf("  Ctrl + C: EXIT during Playing");
	}

	TextColor(12);
	//PRINT RULES
	{
		gotoXY(w + 1, 14);
		printf("*RULES:");
		gotoXY(w + 1, 15);
		printf("  Eat food (o) to get a point");
		gotoXY(w + 1, 16);
		printf("  Touching the line OR eating yourself will lose");
		gotoXY(w + 1, 17);
		printf("  Speed increase every time you eat");
		gotoXY(w + 1, 18);
		printf("  TRY YOUR BEST!!!");
	}
	
	TextColor(8);
	//ABOUT
	{
		gotoXY(w + 1, 20);
		printf("*ABOUT:");
		gotoXY(w + 1, 21);
		printf("  Lecture - MSc Nguyen Van Toan");
		gotoXY(w + 1, 23);
		printf("  Student - Tran Chi Thien (Leader)");
		gotoXY(w + 1, 24);
		printf("  Student - Le Khac Nguyen Thien");
		gotoXY(w + 1, 25);
		printf("  Student - Huynh Minh Tri");
		gotoXY(w + 1, 26);
		printf("  Student - Le Minh Tai");
		gotoXY(w + 1, 28);
		printf("  Mini project SNAKE GAME beta version was built by '4T' team");
	}

	TextColor(10);
	//PRINT MENU
	{
		gotoXY(w / 2 - 10, h / 2 - 1);
		printf("NEW GAME			(1)");

		gotoXY(w / 2 - 10, h / 2);
		printf("HIGHEST SCORE		(2)");

		gotoXY(w / 2 - 10, h / 2 + 1);
		printf("LEVEL			(3)");

		gotoXY(w / 2 - 10, h / 2 + 3);
		printf("(Press 'NUMBER' to choose)");
		
		while (true)
		{
			if (_kbhit())
			{
				int c = _getch();
				if (c == 50)
				{
					scrHighestScore();
					scrBeforePlay(snake, player);
					return;
				}
				else if (c == 51)
				{
					scrLevel(snake);
					scrBeforePlay(snake, player);
					return;
				}
				else if (c == 49)
				{
					typeNamePlayer(player);
					return;
				}
			}
		}
	}
}

void printPlayZone(SNAKE snake, FOOD food)
{
	clearZone(1, 1, w - 1, h - 1);

	TextColor(9);
	gotoXY(snake.dot[0].x, snake.dot[0].y);
	putchar(233);
	for (int i = 1; i < snake.len; i++)
	{
		gotoXY(snake.dot[i].x, snake.dot[i].y);
		putchar(220);
	}

	TextColor(12);
	gotoXY(food.f.x, food.f.y);
	putchar('o');
}

void controlSnake(SNAKE &snake, FOOD &food)
{
	for (int i = snake.len - 1; i > 0; i--)
		snake.dot[i] = snake.dot[i - 1];

	if (_kbhit())
	{
		int c = _getch();

		if (c == 'A' || c == 'a')
		{
			if (snake.stt != RIGHT) snake.stt = LEFT;
		}
		else if (c == 'D' || c == 'd')
		{
			if (snake.stt != LEFT) snake.stt = RIGHT;
		}
		else if (c == 'W' || c == 'w')
		{
			if (snake.stt != DOWN) snake.stt = UP;
		}
		else if (c == 'S' || c == 's')
		{
			if (snake.stt != UP) snake.stt = DOWN;
		}
	}

	if (snake.stt == UP)
		snake.dot[0].y--;
	else if (snake.stt == DOWN)
		snake.dot[0].y++;
	else if (snake.stt == LEFT)
		snake.dot[0].x--;
	else if (snake.stt == RIGHT)
		snake.dot[0].x++;
}

void eatFood(SNAKE &snake, FOOD &food)
{
	if (snake.dot[0].x == food.f.x && snake.dot[0].y == food.f.y)
	{
		Beep(784, 100);
		for (int i = snake.len; i > 0; i--)
			snake.dot[i] = snake.dot[i - 1];
		snake.len++;

		if (snake.speed > 10) snake.speed -= 10;

		if (snake.stt == UP)
			snake.dot[0].y--;
		else if (snake.stt == DOWN)
			snake.dot[0].y++;
		else if (snake.stt == LEFT)
			snake.dot[0].x--;
		else if (snake.stt == RIGHT)
			snake.dot[0].x++;

		food.f.x = rand() % (w - 2) + 1;
		food.f.y = rand() % (h - 2) + 1;
	}
}

void scrScore(SNAKE &snake, PLAYER &player)
{
	clearZone(w + 32, 3, w + 52, 7);

	player.score = snake.len - 1;

	TextColor(7);
	gotoXY(w + 17, 3);
	cout << "Hello " << player.namePlayer << "!!!";
	gotoXY(w + 17, 4);
	printf("Your times play:   %d", player.timePlay);
	gotoXY(w + 17, 5);
	printf("Your score:        %d", player.score);
	gotoXY(w + 17, 6);
	printf("Your best score:   %d", player.bestScore);
	gotoXY(w + 17, 7);
	if (snake.speed <= 100)
	printf("Level:          Hard");
	else if (snake.speed <=200) 
		printf("Level:        Medium");
	else 
		printf("Level:          Easy");
	return;
}

bool isDead(SNAKE snake)
{
	for (int i = 1; i < snake.len; i++)
		if (snake.dot[0].x == snake.dot[i].x && snake.dot[0].y == snake.dot[i].y)
			return true;
	if (snake.dot[0].x < 1 || snake.dot[0].x >= w || snake.dot[0].y < 1 || snake.dot[0].y >= h)
		return true;
	return false;
}

int snakeDead(SNAKE &snake, PLAYER &player)
{
	Beep(250, 1000);

	fstream fileHScore;
	string namePlayer;
	int score = 0;

	fileHScore.open("HighestScore.txt", ios::in);
	fileHScore >> namePlayer;
	fileHScore >> score;
	fileHScore.close();

	fileHScore.open("HighestScore.txt", ios::out);
	if (score)
	{
		if (score < player.score)
		{
			fileHScore << player.namePlayer;
			fileHScore << endl;
			fileHScore << player.score;
		}
	}
	else
	{
		fileHScore << player.namePlayer;
		fileHScore << endl;
		fileHScore << player.score;
	}
	fileHScore.close();
	
	if (player.score > player.bestScore)
	{
		player.bestScore = player.score;
		gotoXY(w / 2 - 12, h / 2 - 1);
		printf("CONGRATULATION");
		gotoXY(w / 2 - 12, h / 2);
		printf("%d is your best score", player.score);
		gotoXY(w / 2 - 12, h / 2 + 1);
		printf("DO YOU WANT TO PLAY AGAIN? Y/N");

		while (true)
		{
			if (_kbhit())
			{
				int c = _getch();
				if (c == 'n' || c == 'N') return 0;
				else if (c == 'y' || c == 'Y') return 1;
			}
		}
	}
	else
	{
		gotoXY(w / 2 - 12, h / 2 - 1);
		printf("COME ON!!! AGAINNN");
		gotoXY(w / 2 - 12, h / 2);
		printf("%d is your score", player.score);
		gotoXY(w / 2 - 12, h / 2 + 1);
		printf("DO YOU WANT TO TRY AGAIN? Y/N");

		while (true)
		{
			if (_kbhit())
			{
				int c = _getch();
				if (c == 'n' || c == 'N') return 0;
				else if (c == 'y' || c == 'Y') return 1;
			}
		}
	}

}

int main()
{
	while (true)
	{
		srand(time(NULL));
		SNAKE snake;
		FOOD food;
		PLAYER player;

		clrscr();					//xoa man hinh
		noCursorType();				//xoa con tro chuot

		initialize(snake, food, player);	//KHOI TAO GIA TRI BAN DAU
		scrBeforePlay(snake, player);		//MAN HINH TRUOC KHI CHOI
		while (true)
		{
			printPlayZone(snake, food);		//in ra Snake va Food
			controlSnake(snake, food);		//Dieu khien Snake
			eatFood(snake, food);			//Snake an Food
			scrScore(snake, player);		//Man hinh diem
			if (isDead(snake))				//Kiem tra dieu kien thua
				break;
			Sleep(snake.speed);
		}
		if (snakeDead(snake, player))		//Ket thuc game
		{
			player.timePlay++;
			continue;		//Player want to play again.
		}
		else break;

	}
	return 0;
}