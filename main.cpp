#include <stdio.h>
#include "chess.h"
#include"chess_game.h"


int main()
{
	//��������
	initgraph(628, 698,EW_SHOWCONSOLE);
	//���ñ���ģʽ
	setbkmode(TRANSPARENT);
	init();
	//������
	IMAGE img_board;
	loadimage(&img_board, LPCTSTR("./res/chessboard.png"));
	//��������
	IMAGE img_gameover;
	loadimage(&img_gameover, LPCTSTR("./res/test.png"));
	//˫�����ͼ����ֹ����
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