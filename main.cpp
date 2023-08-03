#include <stdio.h>
#include "chess.h"

int state = 0;
int nextstate = 0;
int end = 0;

//����
void draw()
{
	setfillcolor(RGB(252, 215, 162));
	setlinestyle(PS_SOLID, 2);
	//������ʽ
	settextstyle(30, 0, "����");

	for (int i = 0; i < tot_num; i++)
	{
		if (!chess[i]->isDead) {
			settextcolor(chess[i]->type);
			setlinecolor(chess[i]->type);
			fillcircle(getspotx(chess[i]), getspoty(chess[i]), 30);
			fillcircle(getspotx(chess[i]), getspoty(chess[i]), 25);
			outtextxy(getspotx(chess[i]) - 15, getspoty(chess[i]) - 15, chess[i]->chess_name);
		}
	}
}

void _move(position targ)
{
	position cur_p = selected_chess->cur_p;
	selected_chess->cur_p = targ;
	map[cur_p.x][cur_p.y] = &null_chess;
	map[targ.x][targ.y] = selected_chess;
	selected_chess = &null_chess;
}

bool chessMove(position target)
{
	position cur_pos = selected_chess->cur_p;
	Chess* targ_chess = map[target.x][target.y];
	Chess* sc = selected_chess;
	//if(sc == &paob1 || sc == &paob2 || sc == &paor1 || sc == &paob2)
	for (int i = 0; i < selected_chess->cap_num; i++)
	{
		position vec = selected_chess->capabilitys[i];
		if (equal(pos_add(vec, cur_pos), target))
		{
			_move(target);
			if (targ_chess != &null_chess) {
				targ_chess->isDead = true;
			}
			return true;
		}
	}
	return false;
}

void next_state() { nextstate = (nextstate + 1) % 4; }

void select(position pos)
{
	Chess* cur_che = map[pos.x][pos.y];
	printp(cur_che->cur_p, "cur_chess:");
	printf("stateb:%d\n", state);
	switch (state) {
	case R1:
		if (cur_che == &null_chess) break;
		if (cur_che->type == BLACK) break;
		selected_chess = cur_che;
		next_state();
		break;
	case R2:
		if (cur_che->type == RED) break;
		if (chessMove(pos)) next_state();
		break;
	case B1:
		if (cur_che == &null_chess) break;
		if (cur_che->type == RED) break;
		selected_chess = cur_che;
		next_state();
		break;
	case B2:
		if (cur_che->type == BLACK) break;
		if (chessMove(pos)) next_state();
		break;
	}
	state = nextstate;
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
			short col = (msg.x - INTERVAL) / GRID_SIZE;
			short row = (msg.y - INTERVAL) / GRID_SIZE;
			position targ = position{ col, row };
			select(targ);
			//chessMove(targ); �����д��һ��chessMove debug����ŷ������Ƿ���
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
		if (jiangb.isDead || jiangr.isDead) break;
	}
	EndBatchDraw();
	getchar();
	return 0;
}