#include <stdio.h>
#include "chess.h"
#include"chess_game.h"


int main()
{
	//创建窗口
	initgraph(628, 698,EW_SHOWCONSOLE);
	//设置背景模式
	setbkmode(TRANSPARENT);
	init();
	//贴棋盘
	IMAGE img_board;
	loadimage(&img_board, LPCTSTR("./res/chessboard.png"));
	//结束界面
	IMAGE img_gameover;
	loadimage(&img_gameover, LPCTSTR("./res/test.png"));
	//双缓冲绘图，防止闪屏
	BeginBatchDraw();
	while (true)
	{
		//chess_game.h
		cleardevice();
		putimage(0, 0, &img_board);
		draw();
		mouseEvent();
		FlushBatchDraw();
		if (jiangb.isDead || jiangr.isDead) break;
	}
	EndBatchDraw();

	putimage(189, 236, &img_gameover);
	getchar();
	return 0;
}