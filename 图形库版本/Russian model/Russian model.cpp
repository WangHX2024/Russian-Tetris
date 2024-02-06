//俄罗斯方块游戏 代码版本 2023.1.1 19:00

#include<stdio.h>
#include<time.h>				//用于生成随机数
#include<easyx.h>				//使用EasyX图形库
#pragma comment(lib,"Winmm.lib")//用于播放音乐指令

const int wide = 10, high = 20;	//方块画面区宽高
const int speed[13] = { 0,300,290,280,270,260,250,240,230,220,210,200,190};//不同等级的游戏速度
const int level_score[13] = { 0,500,1000,1500,2000,2500,3000,4000,5000,6000,8000,10000 };//不同等级对应的游戏得分
int level = 1;					//游戏等级，可取1~12
int sound = 1, sound_flag = 0;	//sound记录声音开关，开为1，关为0
int pixel[15][25];				//方块画面区状态，空白为0，动态方块为1，静态方块为-1
int shape, next_shape;			//当前和即将下落的方块形状代码，可取1~13
int score, history_score, kill_line, round;	

void main_page(void);			//绘制主界面函数
void welcome_page(void);		//绘制欢迎界面区函数
void info_page(void);			//绘制信息栏函数
void pixel_page(void);			//绘制方块区函数
void start_animate(void);		//绘制游戏开始动画函数
void fail_animate(void);		//绘制游戏结束动画函数
void pause_animate(void);		//绘制游戏暂停动画函数
void kill_line_animate(int);	//绘制游戏消行动画函数
void draw_pixel(int,int,int);	//绘制单个方块函数
int key_check(void);			//按键检测函数，返回按键代码：无输入0，左1，右2，上3，下4，空格5，pause6，sound7，reset8，enter9
void game(void);				//游戏主函数
void game_reset(void);			//游戏重置函数
void new_shape(int a);			//生成新方块函数，传入新方块的形状代码
int change_shape(void);			//改变方块形状函数，成功返回1，失败返回0
int move(int i);				//移动方块函数，传入值：左移1，右移2，下移3，返回值：成功1，失败0
void game_check(void);			//检测方块满行、执行消行、检测游戏结束函数
int my_random(int max);			//生成随机数函数，传入随机数最大值，返回生成的随机数
void play_sound(const char* musicname);	//播放音乐函数，传入音乐名，播放相应音乐

void main_page(void)
{
	initgraph(550, 800);		//新建窗口
	SetWindowText(GetHWnd(), "Russian Tetris Game");//设置窗口标题
	setbkcolor(0x19ccef);		//设置背景色：黄色
	setfillcolor(BLACK);		//设置填充色、文本色、画线色：黑色
	settextcolor(BLACK);
	setlinecolor(BLACK);
	setbkmode(TRANSPARENT);		//设置填充模式：透明
	cleardevice();				//刷新窗口以应用背景色
	solidrectangle(60, 40, 68, 500);//绘制显示屏外线条
	solidrectangle(482, 40, 490, 500);
	solidrectangle(60, 500, 490, 508);
	solidrectangle(60, 40, 100, 48);
	solidrectangle(450, 40, 490, 48);
	for (int i = 110, j = 0; j < 4; i += 18, j++)
		solidrectangle(i, 40, i + 8, 48);
	for (int i = 432, j = 0; j < 4; i -= 18, j++)
		solidrectangle(i, 40, i + 8, 48);
	RECT word = { 182,36,360,52 };	//绘制显示屏顶部文字
	drawtext(_T("Russian Tetris Game"), &word, DT_CENTER);
	setfillcolor(0x86ad9e);		//绘制显示屏
	fillrectangle(110, 82, 439, 457);
	setfillcolor(0x0f7f98);
	int edge1[] = { 445,77,105,77,105,463,110,458,110,82,440,82 };
	solidpolygon((POINT*)edge1, 6);
	setfillcolor(0x67d8ee);
	int edge2[] = { 445,77,445,463,105,463,110,458,440,458,440,82 };
	solidpolygon((POINT*)edge2, 6);
	rectangle(120, 92, 298, 447);	//绘制显示屏内线条
	setlinestyle(PS_ENDCAP_FLAT, 2);//绘制按钮和按钮文字
	setfillcolor(0x21c42d);
	fillcircle(80, 570, 20);
	word = { 60,600,100,650 };
	drawtext(_T("Pause\n(P)"), &word, DT_CENTER);
	fillcircle(150, 570, 20);
	word = { 130,600,170,650 };
	drawtext(_T("Sound\n(L)"), &word, DT_CENTER);
	setfillcolor(0x1a1add);
	fillcircle(220, 570, 20);
	word = { 200,600,240,650 };
	drawtext(_T("Reset\n(M)"), &word, DT_CENTER);
	setfillcolor(0xf1655a);
	fillcircle(150, 700, 50);
	word = { 100,760,200,780 };
	drawtext(_T("Drop (SPACE)"), &word, DT_CENTER);
	fillcircle(400, 585, 35);
	word = { 350,575,450,605 };
	drawtext(_T("Rotation"), &word, DT_CENTER);
	fillcircle(400, 721, 35);
	word = { 350,711,450,741 };
	drawtext(_T("DOWN"), &word, DT_CENTER);
	fillcircle(332, 653, 35);
	word = { 282,643,382,673 };
	drawtext(_T("LEFT"), &word, DT_CENTER);
	fillcircle(468, 653, 35);
	word = { 418,643,518,673 };
	drawtext(_T("RIGHT"), &word, DT_CENTER);
	setfillcolor(BLACK);		//绘制箭头
	int arrow1[] = { 380,653,400,673,420,653,400,633 };
	solidpolygon((POINT*)arrow1, 4);
	setfillcolor(0x19ccef);
	int arrow2[] = { 390,643,390,663,410,663,410,643 };
	solidpolygon((POINT*)arrow2, 4);
}

void welcome_page(void)
{
	info_page();
	pixel_page();
	settextcolor(BLACK);
	RECT word = { 120,230,298,400 };
	drawtext(_T("Russian Tetris Game\n\nPress ENTER to Start"), &word, DT_CENTER);	// 打印标题
	word = { 120,400,298,450 };
	drawtext(_T("Final Version 2024.1.1\nBy WangHX and YuanH"), &word, DT_CENTER);		//打印版本和作者
	for (;;)
	{	
		int key = key_check();	//读入键盘输入
		if (key == 9)game();
		if (sound == 0)sound_flag = 1;	//由于采用循环语句，因此必须加一个判断防止后续继续播放的声音与开始重叠
		if (key == 7)sound = (sound) ? 0 : 1;
		if (sound == 0) mciSendString("stop bkmusic ", NULL, 0, NULL);
		else if(sound_flag--==1) mciSendString("play bkmusic repeat", NULL, 0, NULL);
		if (key == 8)			//全部变量初始化
		{
			sound = 1;
			score = history_score = kill_line = round = 0;
			level = 1;
			FILE* fp = fopen("game.txt", "w");
			fprintf(fp, "round:%d\t\tscore:%d\t\thistory_score:%d\n", round, score, history_score);
			fclose(fp);
			info_page();
		}
	}
}

void info_page(void)
{
	BeginBatchDraw();
	setbkcolor(0x86ad9e);
	clearrectangle(320, 100, 430, 450);
	TCHAR s[30];
	RECT word = { 320,100,420,120 };
	drawtext(_T("Round:"), &word, DT_LEFT);	//游戏轮数
	word = { 320,150,420,170 };
	drawtext(_T("Score:"), &word, DT_LEFT);	//当前分数
	word = { 320,200,420,220 };
	drawtext(_T("High:"), &word, DT_LEFT);	//历史最高分
	word = { 370,220,420,245 };
	if (score > history_score && history_score)drawtext(_T("Break!"), &word, DT_LEFT);
	word = { 320,250,420,270 };
	drawtext(_T("Level:"), &word, DT_LEFT);	//游戏等级
	word = { 320,300,420,320 };
	drawtext(_T("Clean:"), &word, DT_LEFT);	//清除行数
	word = { 320,350,420,370 };
	drawtext(_T("Next:"), &word, DT_LEFT);	//下个方块形状
	word = { 370,100,420,120 };
	_stprintf(s, _T("%5d"), round);
	drawtext(s, &word, DT_LEFT);
	word = { 370,150,420,170 };
	_stprintf(s, _T("%5d"), score);
	drawtext(s, &word, DT_LEFT);
	word = { 370,200,420,220 };
	_stprintf(s, _T("%5d"), history_score);
	drawtext(s, &word, DT_LEFT);
	word = { 370,250,420,270 };
	_stprintf(s, _T("%5d"), level);
	drawtext(s, &word, DT_LEFT);
	word = { 370,300,420,320 };
	_stprintf(s, _T("%5d"), kill_line);
	drawtext(s, &word, DT_LEFT);
	int temp[4][4] = { 0 };					//下个方块形状
	if (next_shape == 1)temp[0][0] = temp[0][1] = temp[1][0] = temp[1][1] = 1;
	if (next_shape == 2)temp[1][0] = temp[0][1] = temp[1][1] = temp[2][1] = 1;
	if (next_shape == 3)temp[0][0] = temp[0][1] = temp[1][1] = temp[0][2] = 1;
	if (next_shape == 4)temp[0][0] = temp[1][0] = temp[2][0] = temp[1][1] = 1;
	if (next_shape == 5)temp[1][0] = temp[0][1] = temp[1][1] = temp[1][2] = 1;
	if (next_shape == 6)temp[0][0] = temp[1][0] = temp[1][1] = temp[2][1] = 1;
	if (next_shape == 7)temp[1][0] = temp[0][1] = temp[1][1] = temp[0][2] = 1;
	if (next_shape == 8)temp[0][0] = temp[0][1] = temp[0][2] = temp[0][3] = 1;
	if (next_shape == 9)temp[0][0] = temp[1][0] = temp[2][0] = temp[3][0] = 1;
	if (next_shape == 10)temp[0][0] = temp[0][1] = temp[0][2] = temp[1][2] = 1;
	if (next_shape == 11)temp[0][0] = temp[1][0] = temp[2][0] = temp[0][1] = 1;
	if (next_shape == 12)temp[0][0] = temp[1][0] = temp[1][1] = temp[1][2] = 1;
	if (next_shape == 13)temp[2][0] = temp[0][1] = temp[1][1] = temp[2][1] = 1;
	int px = 330, py = 372, ix = 0, iy = 0;
	for (; iy < 4; py += 17, iy++, px = 330, ix = 0)
		for (; ix < 4; px += 17, ix++)
		{
			if (temp[ix][iy])setfillcolor(BLACK);
			else setfillcolor(0x729387);
			solidrectangle(px, py, px + 14, py + 14);
			setfillcolor(0x86ad9e);
			solidrectangle(px + 2, py + 2, px + 12, py + 12);
			if (temp[ix][iy])setfillcolor(BLACK);
			else setfillcolor(0x729387);
			solidrectangle(px + 3, py + 3, px + 11, py + 11);
		}
	EndBatchDraw();
}

void pixel_page(void)
{
	BeginBatchDraw();
	int px = 125, py = 100, ix = 1, iy = 0;
	for (; iy < 20; py += 17, iy++, px = 125, ix = 1)
		for (; ix < 11; px += 17, ix++)
		{
			if (pixel[ix][iy])draw_pixel(px, py, 1);	//将有方块的格子涂成黑色
			else draw_pixel(px, py, 0);
		}
	EndBatchDraw();
}

void start_animate(void)
{
	Sleep(200);
	setbkcolor(0x86ad9e);
	clearrectangle(121, 93, 297, 446);
	pixel_page();
	int px = 125, py = 100, ix = 1, iy = 0;
	for (; iy < 20; py += 17, iy++, px = 125, ix = 1)
	{
		for (; ix < 11; px += 17, ix++)
			draw_pixel(px, py, 1);
		Sleep(50);
	}
	Sleep(200);
}

void fail_animate(void)
{
	Sleep(200);
	setbkcolor(0x86ad9e);
	int px = 125, py = 423, ix = 1, iy = 0;
	for (; iy < 20; py -= 17, iy++, px = 125, ix = 1)
	{
		for (; ix < 11; px += 17, ix++)
			draw_pixel(px, py, 1);
		Sleep(50);
	}
	Sleep(200);
}

void pause_animate(void)
{
	BeginBatchDraw();
	int px = 125, py = 100, ix = 1, iy = 0;
	for (; iy < 20; py += 17, iy++, px = 125, ix = 1)
		for (; ix < 11; px += 17, ix++)
			draw_pixel(px, py, 0);
	EndBatchDraw();
	Sleep(300);
	pixel_page();
	Sleep(300);
}

void kill_line_animate(int y0)
{
	BeginBatchDraw();
	int px = 125, py = 100, ix = 1, iy = 0;
	for (; iy < 20; py += 17, iy++, px = 125, ix = 1)
		for (; ix < 11; px += 17, ix++)
		{
			if (iy == y0)draw_pixel(px, py, 2);
			else if (pixel[ix][iy])draw_pixel(px, py, 1);
			else draw_pixel(px, py, 0);
		}
	EndBatchDraw();
	play_sound(".\\music\\full.wav");	//播放消除音乐
	Sleep(speed[level]);				//延迟和方块下落速度相适应
}

void draw_pixel(int px, int py, int color)
{
	if (color==1)setfillcolor(BLACK);
	else if (color==2)setfillcolor(RED);
	else setfillcolor(0x729387);
	solidrectangle(px, py, px + 14, py + 14);
	setfillcolor(0x86ad9e);
	solidrectangle(px + 2, py + 2, px + 12, py + 12);
	if (color == 1)setfillcolor(BLACK);
	else if (color == 2)setfillcolor(RED);
	else setfillcolor(0x729387);
	solidrectangle(px + 3, py + 3, px + 11, py + 11);
}

int key_check(void)
{
	int r = 0;
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))r= 1;
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))r= 2;
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))r= 3;
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))r= 4;
	if (GetAsyncKeyState(VK_SPACE))r= 5;
	if (GetAsyncKeyState('P'))r= 6;
	if (GetAsyncKeyState('L'))r= 7;
	if (GetAsyncKeyState('M'))r= 8;
	if (GetAsyncKeyState(VK_RETURN))r= 9;
	if(r)play_sound(".\\music\\mouse.wav");
	return r;
}

void game(void)
{
	start_animate();	//绘制游戏开始动画
	game_reset();
	for (;;)
	{
		next_shape = my_random(5);
		if (next_shape == 2)next_shape = next_shape - 1 + my_random(4);
		else if (next_shape == 3)next_shape = next_shape + 2 + my_random(2);
		else if (next_shape == 4)next_shape = next_shape + 3 + my_random(2);
		else if (next_shape == 5)next_shape = next_shape + 4 + my_random(4);	//上述代码是为了增加长条和方块的刷出概率,增加游戏的可玩性
		info_page();
		new_shape(my_random(wide - 3));
		for (;;)
		{
			int r = 0, key = key_check();	//r表示是否通过按键对方块进行了有效的操作，若有，则在本轮循环中不进行方块的自动下移
			if (key == 1)r = move(1);
			else if (key == 2)r = move(2);
			else if (key == 3)r = change_shape();
			else if (key == 4)r = move(3);
			else if (key == 5)while (move(3));
			else if (key == 6)
				while (int t = key_check() != 6)pause_animate();
			if (sound == 0)sound_flag = 1;	//由于采用循环语句，因此必须加一个判断防止后续继续播放的声音与开始重叠
			if (key == 7)sound = (sound) ? 0 : 1;
			if (sound == 0) mciSendString("stop bkmusic ", NULL, 0, NULL);
			else if (sound_flag-- == 1) mciSendString("play bkmusic repeat", NULL, 0, NULL);
			pixel_page();
			if (r == 1 && key != 3)
			{
				if(sound==0)Sleep(50);
			}
			else Sleep(speed[level]);
			game_check();
			if (r == 0 && move(3) == 0)break;
		}
		shape = next_shape;
		score += 10;
		if (level<12&&score >= level_score[level])level++;	//游戏升级，逐级增加难度
	}
}

void game_reset(void)
{
	score = 0, kill_line = 0, level = 1, round++;	//游戏部分变量初始化
	for (int x = 0; x < wide + 2; x++)				//对方块画面区左、右、下三围筑墙，防止方块移动出界
		pixel[x][high] = -1;
	for (int y = 0; y < high + 1; y++)
		pixel[0][y] = pixel[wide + 1][y] = -1;
}

void new_shape(int a)
{
	if (shape == 1)pixel[a][0] = pixel[a][1] = pixel[a + 1][0] = pixel[a + 1][1] = 1;
	if (shape == 2)pixel[a + 1][0] = pixel[a][1] = pixel[a + 1][1] = pixel[a + 2][1] = 1;
	if (shape == 3)pixel[a][0] = pixel[a][1] = pixel[a + 1][1] = pixel[a][2] = 1;
	if (shape == 4)pixel[a][0] = pixel[a + 1][0] = pixel[a + 2][0] = pixel[a + 1][1] = 1;
	if (shape == 5)pixel[a + 1][0] = pixel[a][1] = pixel[a + 1][1] = pixel[a + 1][2] = 1;
	if (shape == 6)pixel[a][0] = pixel[a + 1][0] = pixel[a + 1][1] = pixel[a + 2][1] = 1;
	if (shape == 7)pixel[a + 1][0] = pixel[a][1] = pixel[a + 1][1] = pixel[a][2] = 1;
	if (shape == 8)pixel[a][0] = pixel[a][1] = pixel[a][2] = pixel[a][3] = 1;
	if (shape == 9)pixel[a][0] = pixel[a + 1][0] = pixel[a + 2][0] = pixel[a + 3][0] = 1;
	if (shape == 10)pixel[a][0] = pixel[a][1] = pixel[a][2] = pixel[a + 1][2] = 1;
	if (shape == 11)pixel[a][0] = pixel[a + 1][0] = pixel[a + 2][0] = pixel[a][1] = 1;
	if (shape == 12)pixel[a][0] = pixel[a + 1][0] = pixel[a + 1][1] = pixel[a + 1][2] = 1;
	if (shape == 13)pixel[a + 2][0] = pixel[a][1] = pixel[a + 1][1] = pixel[a + 2][1] = 1;
}

int change_shape(void)
{
	int x, y, temp[50][50] = { 0 }, x0, y0, mark = 0, r;
	for (y = 0; y < high + 1; y++)
		for (x = 0; x < wide + 2; x++)
			if (pixel[x][y] > 0)x0 = x, y0 = y, r = pixel[x][y];
	if (shape == 2)temp[x0 - 1][y0 - 2] = temp[x0 - 1][y0 - 1] = temp[x0][y0 - 1] = temp[x0 - 1][y0] = 1;
	if (shape == 3)temp[x0 - 1][y0 - 2] = temp[x0][y0 - 2] = temp[x0 + 1][y0 - 2] = temp[x0][y0 - 1] = 1;
	if (shape == 4)temp[x0 - 1][y0 - 2] = temp[x0][y0 - 1] = temp[x0][y0 - 2] = temp[x0][y0 - 3] = 1;
	if (shape == 5)temp[x0][y0 - 3] = temp[x0 - 1][y0 - 2] = temp[x0][y0 - 2] = temp[x0 + 1][y0 - 2] = 1;
	if (shape == 6)temp[x0 - 1][y0 - 1] = temp[x0][y0 - 1] = temp[x0 - 1][y0] = temp[x0][y0 - 2] = 1;
	if (shape == 7)temp[x0 - 1][y0 - 2] = temp[x0][y0 - 2] = temp[x0][y0 - 1] = temp[x0 + 1][y0 - 1] = 1;
	if (shape == 8)temp[x0 - 1][y0 - 2] = temp[x0][y0 - 2] = temp[x0 + 1][y0 - 2] = temp[x0 + 2][y0 - 2] = 1;
	if (shape == 9)temp[x0 - 2][y0 - 1] = temp[x0 - 2][y0] = temp[x0 - 2][y0 + 1] = temp[x0 - 2][y0 - 2] = 1;
	if (shape == 10)temp[x0 - 2][y0 - 2] = temp[x0 - 1][y0 - 2] = temp[x0][y0 - 2] = temp[x0 - 2][y0 - 1] = 1;
	if (shape == 11)temp[x0][y0 - 3] = temp[x0 + 1][y0 - 3] = temp[x0 + 1][y0 - 1] = temp[x0 + 1][y0 - 2] = 1;
	if (shape == 12)temp[x0 + 1][y0 - 3] = temp[x0 - 1][y0 - 2] = temp[x0][y0 - 2] = temp[x0 + 1][y0 - 2] = 1;
	if (shape == 13)temp[x0 - 1][y0 - 2] = temp[x0 - 1][y0 - 1] = temp[x0 - 1][y0] = temp[x0][y0] = 1;
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

int move(int i)
{
	int x, y, temp[50][50] = { 0 }, mark = 0;
	for (y = 0; y < high + 1; y++)
		for (x = 0; x < wide + 2; x++)
			if (pixel[x][y] > 0)
			{
				if (i == 1) temp[x - 1][y] = pixel[x][y];
				if (i == 2) temp[x + 1][y] = pixel[x][y];
				if (i == 3) temp[x][y + 1] = pixel[x][y];
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
	if (i == 3)
		for (y = 0; y < high + 1; y++)
			for (x = 0; x < wide + 2; x++)
				if (pixel[x][y] > 0)pixel[x][y] *= -1;
	return 0;

}

void game_check(void)
{
	recheck:
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
		if (mark == wide)break;	//有一行被填满了
	}
	if (mark == wide)
	{
		kill_line_animate(y0);
		score += 100;			//分数加100
		kill_line++;			//消除行数加一
		for (y = 0; y <= y0; y++)
			for (x = 1; x < wide + 1; x++)
				pixel[x][y] = temp[x][y];
		goto recheck;
	}
	for (x = 1; x < wide + 1; x++)
		if (pixel[x][0] < 0)	//有方块触及顶端
		{
			play_sound(".\\music\\fail2.wav");					//播放失败音效
			if (score > history_score)history_score = score;	//更新历史高分
			FILE* fp = fopen("game.txt", "w");
			fprintf(fp, "round:%d\t\tscore:%d\t\thistory_score:%d\n", round, score, history_score);	//保存到记录中
			fclose(fp);
			fail_animate();
			for (int y = 0; y < high + 1; y++)					//游戏部分变量初始化
				for (int x = 0; x < wide + 2; x++)
					pixel[x][y] = 0;
			next_shape = 0;
			welcome_page();
		}
}

int my_random(int max)
{
	srand(rand() * (unsigned int)(time(0)));
	return rand() % max + 1;
}

void play_sound(const char* musicname) 
{
	if (sound == 0)return;
	char playMusic[50] = "open ";					//打开音乐指令
	strcat(playMusic, musicname);
	strcat(playMusic, " alias tempMusic");
	mciSendString("close tempMusic", NULL, 0, NULL);//关闭上个音效
	mciSendString(playMusic, NULL, 0, NULL);		//打开音效文件
	mciSendString("play tempMusic", NULL, 0, NULL);	//播放音效
}

int main()
{
	fopen("game.txt", "a");		//读取文件
	FILE* fp = fopen("game.txt", "r");
	fscanf_s(fp, "round:%d\t\tscore:%d\t\thistory_score:%d", &round, &score, &history_score);	//读取游戏轮数、分数、历史高分
	mciSendString("open .\\music\\bk1.mp3 alias bkmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);	//防止在此回到welcomepage时出现卡顿
	main_page();
	welcome_page();
}