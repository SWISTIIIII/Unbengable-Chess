#include <stdio.h>
#include "chess.h"

#define ROW 10
#define COL 9
#define INTERVAL 45//边距
#define GRID_SIZE 68//格子宽度
#define NONE -1

//地图
struct Chess map[ROW][COL];

struct State
{
	int begr;
	int begc;
	int endr;
	int endc;
	int state;
}state={-1,-1,-1,-1, 0};

//绘制
void draw()
{
	setfillcolor(RGB(252, 215, 162));
	setlinestyle(PS_SOLID, 2);
	//文字样式
	settextstyle(30, 0, "楷体");

	for (int i = 0; i < 32; i++)
	{
		if (!chess[i]->isDead) {
			settextcolor(chess[i]->type);
			setlinecolor(chess[i]->type);
			fillcircle(chess[i]->cur_p.x, chess[i]->cur_p.y, 30);
			fillcircle(chess[i]->cur_p.x, chess[i]->cur_p.y, 25);
			outtextxy(chess[i]->cur_p.x - 15, chess[i]->cur_p.y - 15, chess[i]->chess_name);
		}
	}
}

void chessMove() 
{

}


//鼠标操作
void mouseEvent()
{
	ExMessage msg;//定义消息结构体变量

	if (peekmessage(&msg, EM_MOUSE))
	{
		if (msg.message == WM_LBUTTONDOWN)//鼠标左键按下
		{
			//通过鼠标坐标得出点击数组下标
			//k * GRID_SIZE + INTERVAL=x
			int col = (msg.x - INTERVAL) / GRID_SIZE;
			int row = (msg.y - INTERVAL) / GRID_SIZE;
			//printf("(%d %d)\n", row, col);
			printf("(%d %d)\n", row, col);
			
			chessMove();
		}

	}
}





int main()
{
	//创建窗口
	initgraph(628, 698,EW_SHOWCONSOLE);
	//设置背景模式
	setbkmode(TRANSPARENT);
	init();
	//贴棋盘
	IMAGE img_board;
	loadimage(&img_board, "./res/chessboard.png");

	//双缓冲绘图，防止闪屏
	//BeginBatchDraw,FlushBatchDraw,EndBatchDraw
	BeginBatchDraw();
	while (true)
	{
		cleardevice();
		putimage(0, 0, &img_board);
		draw();
		mouseEvent();
		FlushBatchDraw();
	}
	EndBatchDraw();
	getchar();
	return 0;
}