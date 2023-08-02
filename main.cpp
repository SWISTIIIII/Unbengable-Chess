#include <stdio.h>
#include "chess.h"

#define ROW 10
#define COL 9
#define INTERVAL 45//�߾�
#define GRID_SIZE 68//���ӿ��
#define NONE -1

//��ͼ
struct Chess map[ROW][COL];

struct State
{
	int begr;
	int begc;
	int endr;
	int endc;
	int state;
}state={-1,-1,-1,-1, 0};

//����
void draw()
{
	setfillcolor(RGB(252, 215, 162));
	setlinestyle(PS_SOLID, 2);
	//������ʽ
	settextstyle(30, 0, "����");

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


//������
void mouseEvent()
{
	ExMessage msg;//������Ϣ�ṹ�����

	if (peekmessage(&msg, EM_MOUSE))
	{
		if (msg.message == WM_LBUTTONDOWN)//����������
		{
			//ͨ���������ó���������±�
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
	//��������
	initgraph(628, 698,EW_SHOWCONSOLE);
	//���ñ���ģʽ
	setbkmode(TRANSPARENT);
	init();
	//������
	IMAGE img_board;
	loadimage(&img_board, "./res/chessboard.png");

	//˫�����ͼ����ֹ����
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