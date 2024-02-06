//俄罗斯方块游戏

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>

int WIDTH=20, HEIGHT=20;
int pixel[50][50];
int shape, next_shape,score,history_score,kill_line, end_game_time;
int score_record[50], mode_record[50], round_record;
int speed = 150,theme = 1,sound = 1,shape_word = 1,line = 1;

void welcome_page(void);
void score_board(void);
void failure_page(void);
void game(void);
void reset_before_game(void);
void print_pixel(void);
void play_sound(int a);
void new_shape(int);
int change_shape(void);
int move_left_right(int);
int move_down_slow(void);
void move_down_direct(void);
void kill_line_check(void);
void game_failure_check(void);
int key_check(void);
int my_random(int);
void print_version(void);
void hide_cursor(void);

void welcome_page(void)
{
	for (;;)
	{
		for (;;)
		{
			int key = key_check();
			if (key == 1)
			{
				if (speed == 150)speed = 100;
				else if (speed == 100)speed = 50;
				else if (speed == 50)speed = 150;
			}
			else if (key == 2)
				WIDTH = (WIDTH == 35) ? 10 : (WIDTH + 1);
			else if (key == 3)
				HEIGHT = (HEIGHT == 25) ? 10 : (HEIGHT + 1);
			else if (key == 4)
				theme = (theme == 3) ? 1 : (theme + 1);
			else if (key == 5)
				sound = sound ? 0 : 1;
			else if (key == 6)
				line = line ? 0 : 1;
			else if (key == 7)
				shape_word = (shape_word == 3) ? 1 : (shape_word + 1);
			else if (key == 8)
			{
				WIDTH = 10;
				HEIGHT = 20;
				speed = 150;
				theme = 1;
				sound = 1;
				line = 1;
				shape_word = 1;
			}
			else if (key == 15)
			{
				play_sound(2);
				return;
			}
			else if (key == 10)
				score_board();
			else break;
		}
		system("cls");
		if (theme == 1)system("color 2F");
		if (theme == 2)system("color 8F");
		if (theme == 3)system("color 0F");
		printf("\n\t   俄罗斯方块游戏\n");
		printf("\tRussian Tetris Game\n\n");
		printf("\t按 SPACE 键启动游戏\n");
		printf("\n-------------------------------------\n");
		printf(" 记分板：\n");
		printf(" 上局得分：%5d\n", score);
		printf(" 历史高分：%5d\n", history_score);
		printf("（按 0 键）打开记分板\n");
		printf("\n-------------------------------------\n");
		printf(" 游戏说明：\n");
		printf(" A 、D\t左移与右移\n");
		printf(" S \t下移到底\n");
		printf(" W \t改变形状\n");
		printf(" SPACE\t暂停游戏\n");
		printf(" 1\t开关辅助网格\n");
		printf(" 0\t结束游戏\n");
		printf("\n-------------------------------------\n");
		printf(" 游戏设置：\n");
		if (speed == 150)
			printf("（按 1 键调整）游戏关卡： <  标 准  >\n");
		if (speed == 100)
			printf("（按 1 键调整）游戏关卡： <  挑 战  >\n");
		if (speed == 50)
			printf("（按 1 键调整）游戏关卡： <  极 限  >\n");
		printf("（按 2 键调整）画面宽度： <    %2d   >\n", WIDTH);
		printf("（按 3 键调整）画面高度： <    %2d   >", HEIGHT);
		if (HEIGHT > 20||WIDTH>20)printf("\t注意：为正常显示，请将窗口调至全屏\n");
		else printf("\n");
		if (theme == 1)
			printf("（按 4 键调整）颜色主题： <  绿 色  >\n");
		if (theme == 2)
			printf("（按 4 键调整）颜色主题： <  灰 色  >\n");
		if (theme == 3)
			printf("（按 4 键调整）颜色主题： <  黑 色  >\n");
		if (sound == 1)
			printf("（按 5 键调整）声音音效： <  开 启  >\t功能待开发\n");
		if (sound == 0)
			printf("（按 5 键调整）声音音效： <  关 闭  >\n");
		if (line == 1)
			printf("（按 6 键调整）辅助网格： <  开 启  >\t注意：在 Windows 10 及以下系统如遇显示问题，请关闭此选项\n");
		if (line == 0)
			printf("（按 6 键调整）辅助网格： <  关 闭  >\n");
		if (shape_word == 1)
			printf("（按 7 键调整）方块文案： < 浙江大学>\n");
		if (shape_word == 2)
			printf("（按 7 键调整）方块文案： < 作者姓名>\n");
		if (shape_word == 3)
			printf("（按 7 键调整）方块文案： <  关 闭  >\n");
		printf("（按 8 键）恢复初始设置\n");
		printf("\n-------------------------------------\n");
		print_version();
		Sleep(150);
	}
}

void score_board(void)
{
	system("cls");
	printf("\n\t   俄罗斯方块游戏\n");
	printf("\tRussian Tetris Game\n\n");
	printf("\t按  任意  键返回主页\n");
	printf("\n-------------------------------------\n");
	printf(" 记分板：\n");
	if (round_record == 0)
		printf("\n\t无游戏记录\n");
	else
	{
		printf(" 上局得分：%5d\n", score);
		printf(" 历史高分：%5d\n\n", history_score);
		printf(" 序 号\t\t关 卡\t\t得 分\n\n");
		for (int i = 1; i <= round_record; i++)
		{
			printf(" %5d\t\t", i);
			if (mode_record[i] == 1) printf("标 准\t\t");
			if (mode_record[i] == 2) printf("挑 战\t\t");
			if (mode_record[i] == 3) printf("极 限\t\t");
			printf("%5d\n", score_record[i]);
		}
	}
	printf("\n-------------------------------------\n");
	print_version();
	getch();
}

void failure_page(void)
{
	system("cls");
	play_sound(6);
	printf("\n\t   俄罗斯方块游戏\n");
	printf("\tRussian Tetris Game\n\n");
	printf("\t按 SPACE 键返回主页\n");
	printf("\n-------------------------------------\n");
	printf(" 记分板：\n");
	printf(" 本局得分：%5d\n", score);
	printf(" 历史高分：%5d\n", history_score);
	printf("（按 0 键）打开记分板\n");
	printf("\n-------------------------------------\n");
	print_version();
	score_record[round_record] = score;
	if (score > history_score)history_score = score;
	for (;;)
	{
		int key = key_check();
		if (key == 15)
		{
			system("cls");
			main();
		}
		if (key == 10)
		{
			score_board();
			main();
		}
	}
}
//更新score_record、history_score，音乐接口6

void game(void)
{
	reset_before_game();
	int r;
	for (;;)
	{
		next_shape = my_random(13);
		new_shape(my_random(4));
		for (;;)
		{
			Sleep(speed);
			for (;;)
			{
				r = 0;
				int key = key_check();
				if (key == 15)
					getch();
				else if (key == 11)
				{
					r = move_left_right(1);
					break;
				}
				else if (key == 12)
				{
					r = move_left_right(2);
					break;
				}
				else if (key == 13)
				{
					r = change_shape();
					break;
				}
				else if (key == 14)
				{
					move_down_direct();
					r = 1;
					break;
				}
				else if (key == 1)
				{
					if (line == 1)
						line = 0;
					else line = 1;
				}
				else if (key == 10)
				{
					end_game_time++;
					if (end_game_time == 2)
					{
						end_game_time = 0;
						failure_page();
					}
				}
				else break;
			}
			print_pixel();
			kill_line_check();
			game_failure_check();
			if (r != 1)
				if (move_down_slow() == 0)break;
		}
		shape = next_shape;
		score += 10;
		if (score >= 1000 && speed == 150)
			speed = 100;
		if (score >= 2000 && speed == 100)
			speed = 50;
		Sleep(speed);
	}
}

void reset_before_game(void)
{
	round_record++;
	if (speed == 150)mode_record[round_record] = 1;
	if (speed == 100)mode_record[round_record] = 2;
	if (speed == 50)mode_record[round_record] = 3;
	int x, y;
	score = 0;
	kill_line = 0;
	end_game_time = 0;
	for (y = 0; y < HEIGHT + 1; y++)
		for (x = 0; x < WIDTH + 2; x++)
			pixel[x][y] = 0;
	for (x = 0; x < WIDTH + 2; x++)
		pixel[x][HEIGHT] = -1;
	for (y = 0; y < HEIGHT + 1; y++)
	{
		pixel[0][y] = -1;
		pixel[WIDTH + 1][y] = -1;
	}
	shape = my_random(13);
}
//更新round_record、mode_record，重置score、kill_line、end_game_time，清空所有pixel并三围筑墙，生成第一个shape

void print_pixel(void)
{
	int x, y;
	system("cls");
	printf("游戏局数： %5d\n", round_record);
	if (speed == 150)
		printf("当前难度： <  标 准  >\n");
	if (speed == 100)
		printf("当前难度： <  挑 战  >\n");
	if (speed == 50)
		printf("当前难度： <  极 限  >\n");
	printf("本局得分： %5d\n", score);
	printf("清除行数： %5d\n", kill_line);
	printf("历史高分： %5d\t\t", history_score);
	if (score > history_score && history_score != 0)printf("新高分！");
	printf("\n\n");
	printf("下一个方块:\n");
	if (next_shape == 1)
		printf("\n囗囗\n囗囗\n\n");
	if (next_shape == 2)
		printf("\n\ \ 囗\n囗囗囗\n\n");
	if (next_shape == 3)
		printf("\n囗\n囗囗\n囗\n");
	if (next_shape == 4)
		printf("\n囗囗囗\n\ \ 囗\n\n");
	if (next_shape == 5)
		printf("\n\ \ 囗\n囗囗\n\ \ 囗\n");
	if (next_shape == 6)
		printf("\n囗囗\n\ \ 囗囗\n\n");
	if (next_shape == 7)
		printf("\n\ \ 囗\n囗囗\n囗\n");
	if (next_shape == 8)
		printf("囗\n囗\n囗\n囗\n");
	if (next_shape == 9)
		printf("\n囗囗囗囗\n\n\n");
	if (next_shape == 10)
		printf("\n囗\n囗\n囗囗\n");
	if (next_shape == 11)
		printf("\n囗囗囗\n囗\n\n");
	if (next_shape == 12)
		printf("\n囗囗\n\ \ 囗\n\ \ 囗\n");
	if (next_shape == 13)
		printf("\n\ \ \ \ 囗\n囗囗囗\n\n");
	for (int i = 0; i < WIDTH; i++)
	{
		if (i == 0)printf("\n");
		printf("--");
		if (i == WIDTH - 1)printf("\n");
	}
	for (y = 0; y < HEIGHT; y++)
		for (x = 1; x < WIDTH + 1; x++)
		{
			if (pixel[x][y] == 0)
			{
				if (line == 1)
					printf("· ");
				else printf("  ");
			}
			else if (pixel[x][y] == 1 || pixel[x][y] == -1)
			{
				if (shape_word == 1)printf("浙");
				if (shape_word == 2)printf("王");
				if (shape_word == 3)printf("囗");
			}
			else if (pixel[x][y] == 2 || pixel[x][y] == -2)
			{
				if (shape_word == 1)printf("江");
				if (shape_word == 2)printf("王");
				if (shape_word == 3)printf("囗");
			}
			else if (pixel[x][y] == 3 || pixel[x][y] == -3)
			{
				if (shape_word == 1)printf("大");
				if (shape_word == 2)printf("浩");
				if (shape_word == 3)printf("囗");
			}
			else if (pixel[x][y] == 4 || pixel[x][y] == -4)
			{
				if (shape_word == 1)printf("学");
				if (shape_word == 2)printf("雄");
				if (shape_word == 3)printf("囗");
			}
			if (x == WIDTH && y > 6)printf("\n");
			if (x == WIDTH && y == 0)printf("\t游戏说明：\n");
			if (x == WIDTH && y == 1)printf("\tA 、D\t左移与右移\n");
			if (x == WIDTH && y == 2)printf("\tS \t下移到底\n");
			if (x == WIDTH && y == 3)printf("\tW \t改变形状\n");
			if (x == WIDTH && y == 4)printf("\tSPACE\t暂停游戏\n");
			if (x == WIDTH && y == 5)printf("\t1\t开关辅助网格\n");
			if (x == WIDTH && y == 6)
			{
				printf("\t0\t结束游戏\t");
				if (end_game_time == 1)
					printf("再按一次 0 键以结束游戏");
				printf("\n");
			}
		}
	for (int i = 0; i < WIDTH; i++)
	{
		printf("--");
		if (i == WIDTH - 1)printf("\n");
	}
	print_version();
}

void play_sound(int a)
{
	if (sound == 0)
		return;
	;
}
//欢迎页开始输入1（未设置），游戏开始输入2，方块触底输入3，消行输入4，新高分输入5（未设置），游戏结束输入6

void new_shape(int i)
{
	int a = my_random(WIDTH - 3);
	if (shape == 1)
	{
		pixel[a][0] = i;
		pixel[a][1] = i;
		pixel[a + 1][0] = i;
		pixel[a + 1][1] = i;
	}
	if (shape == 2)
	{
		pixel[a + 1][0] = i;
		pixel[a][1] = i;
		pixel[a + 1][1] = i;
		pixel[a + 2][1] = i;
	}
	if (shape == 3)
	{
		pixel[a][0] = i;
		pixel[a][1] = i;
		pixel[a + 1][1] = i;
		pixel[a][2] = i;
	}
	if (shape == 4)
	{
		pixel[a][0] = i;
		pixel[a + 1][0] = i;
		pixel[a + 2][0] = i;
		pixel[a + 1][1] = i;
	}
	if (shape == 5)
	{
		pixel[a + 1][0] = i;
		pixel[a][1] = i;
		pixel[a + 1][1] = i;
		pixel[a + 1][2] = i;
	}
	if (shape == 6)
	{
		pixel[a][0] = i;
		pixel[a + 1][0] = i;
		pixel[a + 1][1] = i;
		pixel[a + 2][1] = i;
	}
	if (shape == 7)
	{
		pixel[a + 1][0] = i;
		pixel[a][1] = i;
		pixel[a + 1][1] = i;
		pixel[a][2] = i;
	}
	if (shape == 8)
	{
		pixel[a][0] = i;
		pixel[a][1] = i;
		pixel[a][2] = i;
		pixel[a][3] = i;
	}
	if (shape == 9)
	{
		pixel[a][0] = i;
		pixel[a + 1][0] = i;
		pixel[a + 2][0] = i;
		pixel[a + 3][0] = i;
	}
	if (shape == 10)
	{
		pixel[a][0] = i;
		pixel[a][1] = i;
		pixel[a][2] = i;
		pixel[a + 1][2] = i;
	}
	if (shape == 11)
	{
		pixel[a][0] = i;
		pixel[a + 1][0] = i;
		pixel[a + 2][0] = i;
		pixel[a][1] = i;
	}
	if (shape == 12)
	{
		pixel[a][0] = i;
		pixel[a + 1][0] = i;
		pixel[a + 1][1] = i;
		pixel[a + 1][2] = i;
	}
	if (shape == 13)
	{
		pixel[a + 2][0] = i;
		pixel[a][1] = i;
		pixel[a + 1][1] = i;
		pixel[a + 2][1] = i;
	}
}

int change_shape(void)
{
	int x, y, temp[50][50] = { 0 }, x0, y0, mark = 0, r;
	for (y = 0; y < HEIGHT + 1; y++)
		for (x = 0; x < WIDTH + 2; x++)
			if (pixel[x][y] > 0)
			{
				x0 = x;
				y0 = y;
				r = pixel[x][y];
			}
	if (shape == 1)return 0;
	if (shape == 5)
	{
		temp[x0][y0] = 1;
		temp[x0 - 1][y0 + 1] = 1;
		temp[x0][y0 + 1] = 1;
		temp[x0 + 1][y0 + 1] = 1;
	}
	if (shape == 2)
	{
		temp[x0][y0] = 1;
		temp[x0][y0 + 1] = 1;
		temp[x0 + 1][y0 + 1] = 1;
		temp[x0][y0 + 2] = 1;
	}
	if (shape == 3)
	{
		temp[x0 - 1][y0 + 1] = 1;
		temp[x0][y0 + 1] = 1;
		temp[x0 + 1][y0 + 1] = 1;
		temp[x0][y0 + 2] = 1;
	}
	if (shape == 4)
	{
		temp[x0][y0] = 1;
		temp[x0 + 1][y0 + 1] = 1;
		temp[x0 + 1][y0] = 1;
		temp[x0 + 1][y0 - 1] = 1;
	}
	if (shape == 7)
	{
		temp[x0 - 2][y0 + 1] = 1;
		temp[x0 - 1][y0 + 1] = 1;
		temp[x0 - 1][y0 + 2] = 1;
		temp[x0][y0 + 2] = 1;
	}
	if (shape == 6)
	{
		temp[x0 + 1][y0] = 1;
		temp[x0 + 2][y0] = 1;
		temp[x0 + 1][y0 + 1] = 1;
		temp[x0 + 2][y0 - 1] = 1;
	}
	if (shape == 9)
	{
		temp[x0 + 1][y0] = 1;
		temp[x0 + 1][y0 + 1] = 1;
		temp[x0 + 1][y0 + 2] = 1;
		temp[x0 + 1][y0 - 1] = 1;
	}
	if (shape == 8)
	{
		temp[x0 - 1][y0 + 1] = 1;
		temp[x0][y0 + 1] = 1;
		temp[x0 + 1][y0 + 1] = 1;
		temp[x0 + 2][y0 + 1] = 1;
	}
	if (shape == 13)
	{
		temp[x0 - 1][y0] = 1;
		temp[x0 - 1][y0 + 1] = 1;
		temp[x0 - 1][y0 + 2] = 1;
		temp[x0][y0 + 2] = 1;
	}
	if (shape == 10)
	{
		temp[x0][y0 + 1] = 1;
		temp[x0 + 1][y0 + 1] = 1;
		temp[x0 + 2][y0 + 1] = 1;
		temp[x0][y0 + 2] = 1;
	}
	if (shape == 11)
	{
		temp[x0][y0 - 1] = 1;
		temp[x0 + 1][y0 - 1] = 1;
		temp[x0 + 1][y0 + 1] = 1;
		temp[x0 + 1][y0] = 1;
	}
	if (shape == 12)
	{
		temp[x0 + 2][y0] = 1;
		temp[x0][y0 + 1] = 1;
		temp[x0 + 1][y0 + 1] = 1;
		temp[x0 + 2][y0 + 1] = 1;
	}
	for (y = 0; y < HEIGHT + 1; y++)
		for (x = 0; x < WIDTH + 2; x++)
			if (pixel[x][y] >= 0 && temp[x][y] == 1)
				mark++;
	if (mark == 4)
	{
		for (y = 0; y < HEIGHT + 1; y++)
			for (x = 0; x < WIDTH + 2; x++)
			{
				if (pixel[x][y] > 0)
					pixel[x][y] = 0;
				if (temp[x][y] == 1)
					pixel[x][y] = r;
			}
		if (shape != 5 && shape != 7 && shape != 9 && shape != 13)
		{
			shape++;
			return 1;
		}
		if (shape == 7 || shape == 9)
		{
			shape--;
			return 1;
		}
		if (shape == 5 || shape == 13)
		{
			shape -= 3;
			return 1;
		}
	}
	return 0;
}
//变形成功，返回1，否则返回0

int move_left_right(int i)
{
	int x, y, temp[50][50] = { 0 }, mark = 0;
	for (y = 0; y < HEIGHT + 1; y++)
		for (x = 0; x < WIDTH + 2; x++)
			if (pixel[x][y] > 0)
			{
				if (i == 1)
					temp[x - 1][y] = pixel[x][y];
				else temp[x + 1][y] = pixel[x][y];
			}
	for (y = 0; y < HEIGHT + 1; y++)
		for (x = 0; x < WIDTH + 2; x++)
			if (pixel[x][y] >= 0 && temp[x][y] > 0)
				mark++;
	if (mark == 4)
	{
		for (y = 0; y < HEIGHT + 1; y++)
			for (x = 0; x < WIDTH + 2; x++)
			{
				if (pixel[x][y] > 0)
					pixel[x][y] = 0;
				if (temp[x][y] > 0)
					pixel[x][y] = temp[x][y];
			}
		return 1;
	}
	return 0;
}
//左移输入1，右移输入2，移动成功返回1，否则返回0

int move_down_slow(void)
{
	int x, y, temp[50][50] = { 0 }, mark = 0;
	for (y = 0; y < HEIGHT + 1; y++)
		for (x = 0; x < WIDTH + 2; x++)
			if (pixel[x][y] > 0)
				temp[x][y + 1] = pixel[x][y];
	for (y = 0; y < HEIGHT + 1; y++)
		for (x = 0; x < WIDTH + 2; x++)
			if (pixel[x][y] >= 0 && temp[x][y] > 0)
				mark++;
	if (mark == 4)
	{
		for (y = 0; y < HEIGHT + 1; y++)
			for (x = 0; x < WIDTH + 2; x++)
			{
				if (pixel[x][y] > 0)
					pixel[x][y] = 0;
				if (temp[x][y] > 0)
					pixel[x][y] = temp[x][y];
			}
		return 1;
	}
	else
	{
		for (y = 0; y < HEIGHT + 1; y++)
			for (x = 0; x < WIDTH + 2; x++)
				if (pixel[x][y] > 0)
					pixel[x][y] *= -1;
		play_sound(3);
		return 0;
	}
}
//移动成功返回1，触底返回0，音乐接口3

void move_down_direct(void)
{
	for (; move_down_slow() != 0;);
}

void kill_line_check(void)
{
	int x, y, temp[50][50] = { 0 }, mark, y0;
	for (y = 0; y < HEIGHT; y++)
	{
		mark = 0;
		for (x = 1; x < WIDTH + 1; x++)
		{
			if (pixel[x][y] < 0)
			{
				mark++;
				temp[x][y + 1] = pixel[x][y];
			}
			y0 = y;
		}
		if (mark == WIDTH)break;
	}
	if (mark == WIDTH)
	{
		score += 100;
		kill_line++;
		for (x = 1; x < WIDTH + 1; x++)
			pixel[x][y0] = 0;
		for (y = 0; y <= y0; y++)
			for (x = 1; x < WIDTH + 1; x++)
				pixel[x][y] = temp[x][y];
		play_sound(4);
	}
}
//音乐接口4,更新score、kill_line

void game_failure_check(void)
{
	for (int x = 1; x < WIDTH + 1; x++)
		if (pixel[x][0] < 0)
			failure_page();
}

int key_check(void)
{
	if (kbhit())
	{
		char c = getch();
		if (c == 'A' || c == 'a')	return 11;
		if (c == 'D' || c == 'd')	return 12;
		if (c == 'W' || c == 'w')	return 13;
		if (c == 'S' || c == 's')	return 14;
		if (c == 32)	return 15;
		if (c == '1')return 1;
		if (c == '2')return 2;
		if (c == '3')return 3;
		if (c == '4')return 4;
		if (c == '5')return 5;
		if (c == '6')return 6;
		if (c == '7')return 7;
		if (c == '8')return 8;
		if (c == '0')return 10;
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
	printf(" Version 2023.11.30\n");
	printf(" Copyright 2023 WangHX and YuanH. All rights reserved.\n");
}

void hide_cursor(void)
{
	CONSOLE_CURSOR_INFO curInfo; 
	curInfo.dwSize = 1;  
	curInfo.bVisible = FALSE; 
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorInfo(handle, &curInfo); 
}

int main()
{
	hide_cursor();
	welcome_page();
	game();
}