//俄罗斯方块游戏

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<conio.h>

#define SPEED1 200
#define SPEED2 150
#define SPEED3 100

int wide=20, high=20, speed = SPEED1, page_theme = 1, sound = 1, tetris_theme = 1, line = 1;
int pixel[50][50],shape, next_shape,score,history_score,kill_line, end_game_time, score_record[999], mode_record[999], round;

void welcome_page(void);
void score_board(void);
void failure_page(void);
void game(void);
void print_game(void);
void print_next_shape(void);
void print_pixel(void);
void play_sound(int a);
void new_shape(int);
int change_shape(void);
int move(int);
void kill_line_check(void);
void game_failure_check(void);
int key_check(void);
int my_random(int);
void print_version(void);
void gotoxy(int, int);

void welcome_page(void)
{
	system("cls");
	page_theme == 1?system("color 2F"):system("color 0F");
	printf("\n\t   俄罗斯方块游戏\n\tRussian Tetris Game\n\n\t按 SPACE 键启动游戏\n");
	printf("\n-------------------------------------\n");
	printf(" 记分板：\n 上局得分：%5d\n 历史高分：%5d\n（按 0 键）打开记分板\n", score, history_score);
	printf("\n-------------------------------------\n");
	printf(" 游戏说明：\n A 、D\t左移与右移\n S \t下移到底\n W \t改变形状\n SPACE\t暂停游戏\n 1\t开关辅助网格\n 0\t结束游戏\n");
	printf("\n-------------------------------------\n");
	printf(" 游戏设置：\n\n\n\n\n\n\n\n\n");
	printf("\n-------------------------------------\n");
	print_version();
	for (;;)
	{
		for (int key; key = key_check();)
		{
			if (key == 1)speed = speed == SPEED1 ? SPEED2 : (speed == SPEED2 ? SPEED3 : SPEED1);
			if (key == 2)wide = (wide == 35) ? 10 : (wide + 1);
			if (key == 3)high = (high == 25) ? 10 : (high + 1);
			if (key == 4)
			{
				page_theme = (page_theme == 2) ? 1 : 2;
				welcome_page();
			}
			if (key == 5)sound = sound ? 0 : 1;
			if (key == 6)line = line ? 0 : 1;
			if (key == 7)tetris_theme = (tetris_theme == 3) ? 1 : (tetris_theme + 1);
			if (key == 8)
			{
				wide = 20,high = 20,speed = SPEED1,page_theme = 1,sound = 1,line = 1,tetris_theme = 1,score = 0,history_score = 0,round = 0;
				for (int i = 0; i < 999; i++)
					score_record[i] = mode_record[i] = 0;
				fopen("game.txt", "w");
				welcome_page();
			}
			if (key == 15)game();
			if (key == 10)score_board();
		}
		gotoxy(0,23);
		printf("（按 1 键调整）游戏关卡： <  %s  >\n", (speed == SPEED1) ? "标 准" : (speed == SPEED2 ? "挑 战" : "极 限"));
		printf("（按 2 键调整）画面宽度： <    %2d   >\n", wide);
		printf("（按 3 键调整）画面高度： <    %2d   >\n", high);
		printf("（按 4 键调整）颜色主题： <  %s  >\n",page_theme==1?"绿 色":"黑 色");
		printf("（按 5 键调整）声音音效： <  %s  >\n",sound?"开 启":"关 闭");
		printf("（按 6 键调整）辅助网格： <  %s  >\t注意：如遇显示问题，请关闭此选项\n",line?"开 启":"关 闭");
		printf("（按 7 键调整）方块主题： < %s>\n", (tetris_theme == 1) ? "彩色方块" : (tetris_theme == 2 ? "浙江大学" : "作者姓名"));
		printf("（按 8 键）恢复初始设置并清除记分板\n");
	}
}

void score_board(void)
{
	system("cls");
	printf("\n\t   俄罗斯方块游戏\n\tRussian Tetris Game\n\n\t按  任意  键返回主页\n");
	printf("\n-------------------------------------\n");
	printf(" 记分板：\n");
	if (round == 0)printf("\n\t无游戏记录\n");
	else
	{
		printf(" 上局得分：%5d\n 历史高分：%5d\n\n 序 号\t\t关 卡\t\t得 分\n\n", score, history_score);
		for (int i = 1; i <= round; i++)
			printf(" %5d\t\t%s\t\t%5d\n", i,mode_record[i]==1?"标 准":(mode_record[i]==2?"挑 战":"极 限"), score_record[i]);
	}
	printf("\n-------------------------------------\n");
	print_version();
	_getch();
	welcome_page();
}

void failure_page(void)
{
	system("cls");
	printf("\n\t   俄罗斯方块游戏\n\tRussian Tetris Game\n\n\t按 SPACE 键返回主页\n");
	printf("\n-------------------------------------\n");
	printf(" 记分板：\n 上局得分：%5d\n 历史高分：%5d\n（按 0 键）打开记分板\n", score, history_score);
	printf("\n-------------------------------------\n");
	print_version();
	end_game_time = 0;
	score_record[round] = score;
	if (score > history_score)history_score = score;
	fprintf(fopen("game.txt", "a"), "round:%d\t\tmode:%d\t\tscore:%d\t\tlast round score:%d\t\thistory_score:%d\n", round, mode_record[round], score_record[round], score, history_score);
	int key = _getch();
	if (key == 32)welcome_page();
	if (key == '0')score_board();
}
//更新score_record、history_score

void game(void)
{
	round++;
	mode_record[round] = (speed == SPEED1) ? 1 : (speed == SPEED2 ? 2 : 3);
	score = 0, kill_line = 0, end_game_time = 0;
	for (int y = 0; y < high + 1; y++)
		for (int x = 0; x < wide + 2; x++)
			pixel[x][y] = 0;
	for (int x = 0; x < wide + 2; x++)
		pixel[x][high] = -1;
	for (int y = 0; y < high + 1; y++)
		pixel[0][y] = pixel[wide + 1][y] = -1;
	shape = my_random(13);
	print_game();
	for (;;)
	{
		next_shape = my_random(13);
		print_next_shape();
		new_shape(my_random(4));
		for (;;)
		{
			int r=0,k=0;
			int key = key_check();
			if (key == 15)
			{
				gotoxy(wide * 2, 22);printf("\t游戏暂停中,");
				system("pause");
				gotoxy(wide * 2, 22);printf("\t                              ");
			}
			else if (key == 11)k = r = move(1);
			else if (key == 12)k = r = move(2);
			else if (key == 13)r = change_shape();	
			else if (key == 14)for (r=1; move(3) != 0;);
			else if (key == 1)line = line ? 0 : 1;
			else if (key == 10)
			{
				end_game_time++;
				gotoxy(wide * 2, 21);printf("\t再按一次 0 以结束游戏");
				if (end_game_time == 2)failure_page();
			}
			print_pixel();
			if (k == 1)Sleep(50);
			else Sleep(speed);
			kill_line_check();
			game_failure_check();
			if(r==0 && move(3) == 0)break;
		}
		shape = next_shape;
		score += 10;
		speed = (score >= 1000 && speed == SPEED1) ? SPEED2 : (score >= 2000 && speed == SPEED2 ? SPEED3 : speed);
	}
}

void print_game(void)
{
	system("cls");
	printf("游戏局数： %5d\n当前难度： <         >\n本局得分：\n清除行数：\n历史高分： %5d\n\n下一个方块:\n\n\n\n\n", round, history_score);
	for (int i = 0; i < wide; i++)
	{
		if (i == 0)printf("\n");
		printf("--");
		if (i == wide - 1)printf("\n");
	}
	gotoxy(0, 13 + high);
	for (int i = 0; i < wide; i++)
	{
		printf("--");
		if (i == wide - 1)printf("\n");
	}
	print_version();
	gotoxy(wide * 2, 13);printf("\t游戏说明：");
	gotoxy(wide * 2, 14);printf("\tA 、D\t左移与右移");
	gotoxy(wide * 2, 15);printf("\tS \t下移到底");
	gotoxy(wide * 2, 16);printf("\tW \t改变形状");
	gotoxy(wide * 2, 17);printf("\tSPACE\t暂停游戏");
	gotoxy(wide * 2, 18);printf("\t1\t开关辅助网格");
	gotoxy(wide * 2, 19);printf("\t0\t结束游戏");
}

void print_next_shape(void)
{
	gotoxy(14, 1);
	printf("%s", speed == SPEED1 ? "标 准" : (speed == SPEED2 ? "挑 战" : "极 限"));
	gotoxy(11, 2);printf("%5d", score);
	if (score > history_score&&history_score!=0)printf("\t新高分！");
	gotoxy(11, 3);printf("%5d", kill_line);
	gotoxy(0, 7);
	if (next_shape == 1)printf("        \n囗囗    \n囗囗    \n        \n");
	if (next_shape == 2)printf("        \n  囗    \n囗囗囗  \n        \n");
	if (next_shape == 3)printf("        \n囗      \n囗囗    \n囗      \n");
	if (next_shape == 4)printf("        \n囗囗囗  \n  囗    \n        \n");
	if (next_shape == 5)printf("        \n  囗    \n囗囗    \n  囗    \n");
	if (next_shape == 6)printf("        \n囗囗    \n  囗囗  \n        \n");
	if (next_shape == 7)printf("        \n  囗    \n囗囗    \n囗      \n");
	if (next_shape == 8)printf("  囗    \n  囗    \n  囗    \n  囗    \n");
	if (next_shape == 9)printf("        \n囗囗囗囗\n        \n        \n");
	if (next_shape == 10)printf("        \n囗      \n囗      \n囗囗    \n");
	if (next_shape == 11)printf("        \n囗囗囗  \n囗      \n        \n");
	if (next_shape == 12)printf("        \n囗囗    \n  囗    \n  囗    \n");
	if (next_shape == 13)printf("        \n    囗  \n囗囗囗  \n        \n");
}

void print_pixel(void)
{
	gotoxy(0, 13);
	for (int y = 0; y < high; y++)
		for (int x = 1; x < wide + 1; x++)
		{
			if (pixel[x][y] == 0)printf("%s", line ? "· " : "  ");
			else if (pixel[x][y] == 1 || pixel[x][y] == -1)
			{
				if (tetris_theme == 1)printf("\033[31m囗\033[37m\033[1m");
				if (tetris_theme == 2)printf("浙");
				if (tetris_theme == 3)printf("王");
			}
			else if (pixel[x][y] == 2 || pixel[x][y] == -2)
			{
				if (tetris_theme == 1)printf("\033[33m囗\033[37m\033[1m");
				if (tetris_theme == 2)printf("江");
				if (tetris_theme == 3)printf("王");
			}
			else if (pixel[x][y] == 3 || pixel[x][y] == -3)
			{
				if (tetris_theme == 1)printf("\033[34m囗\033[37m\033[1m");
				if (tetris_theme == 2)printf("大");
				if (tetris_theme == 3)printf("浩");
			}
			else if (pixel[x][y] == 4 || pixel[x][y] == -4)
			{
				if (tetris_theme == 1)printf("\033[35m囗\033[37m\033[1m");
				if (tetris_theme == 2)printf("学");
				if (tetris_theme == 3)printf("雄");
			}
			if (x == wide )printf("\n");
		}
}

void play_sound(int a)
{
}
//欢迎页开始输入1，游戏开始输入2，方块触底输入3，消行输入4，新高分输入5，游戏结束输入6

void new_shape(int i)
{
	int a = my_random(wide - 3);
	if (shape == 1)pixel[a][0] = pixel[a][1] = pixel[a + 1][0] = pixel[a + 1][1] = i;
	if (shape == 2)pixel[a + 1][0] = pixel[a][1] = pixel[a + 1][1] = pixel[a + 2][1] = i;
	if (shape == 3)pixel[a][0] = pixel[a][1] = pixel[a + 1][1] = pixel[a][2] = i;
	if (shape == 4)pixel[a][0] = pixel[a + 1][0] = pixel[a + 2][0] = pixel[a + 1][1] = i;
	if (shape == 5)pixel[a + 1][0] = pixel[a][1] = pixel[a + 1][1] = pixel[a + 1][2] = i;
	if (shape == 6)pixel[a][0] = pixel[a + 1][0] = pixel[a + 1][1] = pixel[a + 2][1] = i;
	if (shape == 7)pixel[a + 1][0] = pixel[a][1] = pixel[a + 1][1] = pixel[a][2] = i;
	if (shape == 8)pixel[a][0] = pixel[a][1] = pixel[a][2] = pixel[a][3] = i;
	if (shape == 9)pixel[a][0] = pixel[a + 1][0] = pixel[a + 2][0] = pixel[a + 3][0] = i;
	if (shape == 10)pixel[a][0] = pixel[a][1] = pixel[a][2] = pixel[a + 1][2] = i;
	if (shape == 11)pixel[a][0] = pixel[a + 1][0] = pixel[a + 2][0] = pixel[a][1] = i;
	if (shape == 12)pixel[a][0] = pixel[a + 1][0] = pixel[a + 1][1] = pixel[a + 1][2] = i;
	if (shape == 13)pixel[a + 2][0] = pixel[a][1] = pixel[a + 1][1] = pixel[a + 2][1] = i;
}

int change_shape(void)
{
	int x, y, temp[50][50] = { 0 }, x0, y0, mark = 0, r;
	for (y = 0; y < high + 1; y++)
		for (x = 0; x < wide + 2; x++)
			if (pixel[x][y] > 0)
			{
				x0 = x;
				y0 = y;
				r = pixel[x][y];
			}
	if (shape == 1)return 0;
	if (shape == 2)temp[x0 - 1][y0] = temp[x0 - 1][y0 + 1] = temp[x0][y0 + 1] = temp[x0 - 1][y0 + 2] = 1;
	if (shape == 3)temp[x0 - 1][y0 + 1] = temp[x0][y0 + 1] = temp[x0 + 1][y0 + 1] = temp[x0][y0 + 2] = 1;
	if (shape == 4)temp[x0 - 1][y0] = temp[x0][y0 + 1] = temp[x0][y0] = temp[x0][y0 - 1] = 1;
	if (shape == 5)temp[x0][y0] = temp[x0 - 1][y0 + 1] = temp[x0][y0 + 1] = temp[x0 + 1][y0 + 1] = 1;
	if (shape == 6)temp[x0 - 1][y0] = temp[x0][y0] = temp[x0 - 1][y0 + 1] = temp[x0][y0 - 1] = 1;
	if (shape == 7)temp[x0 - 1][y0 + 1] = temp[x0][y0 + 1] = temp[x0][y0 + 2] = temp[x0 + 1][y0 + 2] = 1;
	if (shape == 8)temp[x0 - 1][y0 + 1] = temp[x0][y0 + 1] = temp[x0 + 1][y0 + 1] = temp[x0 + 2][y0 + 1] = 1;
	if (shape == 9)temp[x0 - 2][y0] = temp[x0 - 2][y0 + 1] = temp[x0 - 2][y0 + 2] = temp[x0 - 2][y0 - 1] = 1;
	if (shape == 10)temp[x0 - 2][y0 + 1] = temp[x0 - 1][y0 + 1] = temp[x0][y0 + 1] = temp[x0 - 2][y0 + 2] = 1;
	if (shape == 11)temp[x0][y0 - 1] = temp[x0 + 1][y0 - 1] = temp[x0 + 1][y0 + 1] = temp[x0 + 1][y0] = 1;
	if (shape == 12)temp[x0 + 1][y0] = temp[x0 - 1][y0 + 1] = temp[x0][y0 + 1] = temp[x0 + 1][y0 + 1] = 1;
	if (shape == 13)temp[x0 - 1][y0] = temp[x0 - 1][y0 + 1] = temp[x0 - 1][y0 + 2] = temp[x0][y0 + 2] = 1;
	for (y = 0; y < high + 1; y++)
		for (x = 0; x < wide + 2; x++)
			if (pixel[x][y] >= 0 && temp[x][y] == 1)mark++;
	if (mark == 4)
	{
		for (y = 0; y < high + 1; y++)
			for (x = 0; x < wide + 2; x++)
			{
				if (pixel[x][y] > 0)pixel[x][y] = 0;
				if (temp[x][y] == 1)pixel[x][y] = r;
			}
		if (shape != 5 && shape != 7 && shape != 9 && shape != 13)shape++;
		else if (shape == 7 || shape == 9)shape--;
		else if (shape == 5 || shape == 13)shape -= 3;
		return 1;
	}
	return 0;
}
//变形成功，返回1，否则返回0

int move(int i)
{
	int x, y, temp[50][50] = { 0 }, mark = 0;
	for (y = 0; y < high + 1; y++)
		for (x = 0; x < wide + 2; x++)
			if (pixel[x][y] > 0)
			{
				if (i == 1)temp[x - 1][y] = pixel[x][y];
				if (i == 2)temp[x + 1][y] = pixel[x][y];
				if (i == 3)temp[x][y + 1] = pixel[x][y];
			}
	for (y = 0; y < high + 1; y++)
		for (x = 0; x < wide + 2; x++)
			if (pixel[x][y] >= 0 && temp[x][y] > 0)mark++;
	if (mark == 4)
	{
		for (y = 0; y < high + 1; y++)
			for (x = 0; x < wide + 2; x++)
			{
				if (pixel[x][y] > 0)pixel[x][y] = 0;
				if (temp[x][y] > 0)pixel[x][y] = temp[x][y];
			}
		return 1;
	}
	if(i!=3)return 0;
	if(i==3)
	{
		for (y = 0; y < high + 1; y++)
			for (x = 0; x < wide + 2; x++)
				if (pixel[x][y] > 0)pixel[x][y] *= -1;
		return 0;
	}
}
//左移输入1，右移输入2，下移输入3，移动成功返回1，否则返回0

void kill_line_check(void)
{
	int x, y, temp[50][50] = { 0 }, mark, y0;
	for (y = 0; y < high; y++)
	{
		mark = 0;
		for (x = 1; x < wide + 1; x++)
		{
			if (pixel[x][y] < 0)
			{
				mark++;
				temp[x][y + 1] = pixel[x][y];
			}
			y0 = y;
		}
		if (mark == wide)break;
	}
	if (mark == wide)
	{
		score += 100;
		kill_line++;
		for (x = 1; x < wide + 1; x++)
			pixel[x][y0] = 0;
		for (y = 0; y <= y0; y++)
			for (x = 1; x < wide + 1; x++)
				pixel[x][y] = temp[x][y];
	}
}
//更新score、kill_line

void game_failure_check(void)
{
	for (int x = 1; x < wide + 1; x++)
		if (pixel[x][0] < 0)failure_page();
}

int key_check(void)
{
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))return 11;
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))return 12;
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))return 13;
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))return 14;
	if (_kbhit())
	{
		int c = _getch();
		if (c >= '1' && c <= '9')return (c - '0');
		if (c == '0')return 10;
		if (c == 32)return 15;
	}
	return 0;
}
//0返回10，左键返回11，右键返回12，上键返回13，下键返回14，空格返回15，无输入返回0

int my_random(int max)
{
	srand(time(0));
	return rand() % max + 1;
}

void print_version(void)
{
	printf(" Version 2023.12.4\n Copyright 2023 WangHX and YuanH. All rights reserved.\n");
}

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main()
{





	SetConsoleTitle(L"俄罗斯方块游戏");//窗口标题
	HWND hwnd = GetForegroundWindow();//窗口全屏
	LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);
	SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP )   & ~WS_SIZEBOX);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);
	

	
	
	
	
	
	
	CONSOLE_CURSOR_INFO curInfo;//隐藏光标
	curInfo.dwSize = 1;
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	fopen("game.txt", "a");//读取文件
	FILE* fp = fopen("game.txt", "r");
	for (; !feof(fp);)
	{
		fscanf(fp, "round:%d\t\t", &round);
		fscanf(fp, "mode:%d\t\tscore:%d\t\tlast round score:%d\t\thistory_score:%d\n", &mode_record[round], &score_record[round], &score, &history_score);
	}
	Sleep(1000);
	welcome_page();
}