#include <stdio.h>
#include "chess.h"

int state = 0;
int nextstate = 0;
int end = 0;
//绘制
void draw()
{
	setfillcolor(RGB(252, 215, 162));
	setlinestyle(PS_SOLID, 2);
	//文字样式
	settextstyle(30, 0, LPCTSTR("楷体"));
	setlinecolor(GREEN);
	if (selected_chess != &null_chess)
		fillcircle(getspotx(selected_chess), getspoty(selected_chess), 35);
	for (int i = 0; i < tot_num; i++)
	{
		if (!chess[i]->isDead) {
			settextcolor(chess[i]->type);
			setlinecolor(chess[i]->type);
			fillcircle(getspotx(chess[i]), getspoty(chess[i]), 30);
			fillcircle(getspotx(chess[i]), getspoty(chess[i]), 25);
			outtextxy(getspotx(chess[i]) - 15, getspoty(chess[i]) - 15, LPCTSTR(chess[i]->chess_name));
		}
	}
}

bool judge_jiang(position targ)
{
	Chess* sc = selected_chess;
	if (sc->tag != jiang) return true;
	if (targ.x < 3 || targ.x > 5 || (targ.y < 7 && targ.y > 2)) return false;
	return true;
}

bool judge_shi(position targ)
{
	Chess* sc = selected_chess;
	if (sc->tag != shi) return true;
	if (targ.x < 3 || targ.x > 5 || (targ.y < 7 && targ.y > 2)) return false;
	return true;
	return true;
}

bool judge_xiang(position targ)
{
	Chess* sc = selected_chess;
	if (sc->tag != xiang) return true;
	if (sc->type == RED && targ.y < 5) //过河检查
		return false;
	if (sc->type == BLACK && targ.y > 4)
		return false;
	return true;
}

bool judge_zu(position targ)
{
	Chess* sc = selected_chess;
	if (sc->tag != zu) return true;
	if (sc->type == RED && sc->cur_p.y > 4
		&& !equal(targ, pos_add(sc->cur_p, position{ 0, -1 }))) //过河检查
		return false;
	if (sc->type == BLACK && sc->cur_p.y < 5
		&& !equal(targ, pos_add(sc->cur_p, position{ 0, 1 })))
		return false;
	return true;
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
	for (int i = 0; i < selected_chess->cap_num; i++)
	{
		position vec = selected_chess->capabilitys[i];
		if (equal(pos_add(vec, cur_pos), target))
		{
			if (block_detect_stra(target) && sc->tag == che) return false; //车的阻塞特判
			if (sc->tag == pao)
			{
				if ((block_detect_stra(target) == 1 && targ_chess != &null_chess) //炮的两种行为特判
					|| (!block_detect_stra(target) && map[target.x][target.y] == &null_chess));
				else return false;
			}
			if (sc->tag == ma) // 马的行为特判
			{
				bool flag = true;
				int j = i / 2;
				position tmp = pos_add(sc->cur_p, direct[j]);
				if (j == i / 2 && map[tmp.x][tmp.y] != &null_chess) return false;
			}
			if (sc->tag == xiang) // 象的行为特判
			{
				bool flag = true;
				int j = i / 2;
				position tmp = pos_add(sc->cur_p, xiangs_block[j]);
				if (map[tmp.x][tmp.y] != &null_chess) return false;
				
			}
			/*printf("\n\n(%d,%d)\n\n", sc->cur_p.x, sc->cur_p.y);
			switch (j)
			{
			case 0:
				if (map[sc->cur_p.x + 1][sc`->cur_p.y - 1] != &null_chess) return false;
				break;
			case 1:
				if (map[sc->cur_p.x + 1][sc->cur_p.y + 1] != &null_chess) return false;
				break;
			case 2:
				if (map[sc->cur_p.x - 1][sc->cur_p.y - 1] != &null_chess) return false;
				break;
			case 3:
				if (map[sc->cur_p.x - 1][sc->cur_p.y + 1] != &null_chess) return false;
				break;*/
			if (!judge_jiang(target)) return false;
			if (!judge_shi(target)) return false;
			if (!judge_xiang(target)) return false;
			if (!judge_zu(target)) return false;
			_move(target);
			if (targ_chess != &null_chess) targ_chess->isDead = true;
			return true;
		}
	}
	return false;
}

void next_state() { nextstate = (nextstate + 1) % 4; }

void select(position pos)
{
	Chess* cur_che = map[pos.x][pos.y];
	printp(pos, "tag_pos:\n");
	printf("stateb:%d\n", state);
	switch (state) {
	case R1:
		if (cur_che == &null_chess) break;
		if (cur_che->type == BLACK) break;
		selected_chess = cur_che;
		next_state();
		break;
	case R2:
		if (cur_che->type == RED)
		{
			selected_chess = cur_che;
			break;
		}
		if (chessMove(pos)) next_state();
		break;
	case B1:
		if (cur_che == &null_chess) break;
		if (cur_che->type == RED) break;
		selected_chess = cur_che;
		next_state();
		break;
	case B2:
		if (cur_che->type == BLACK)
		{
			selected_chess = cur_che;
			break;
		}
		if (chessMove(pos)) next_state();
		break;
	}
	state = nextstate;
}

int block_detect_stra(position targ)
{
	int num = 0;
	for (int i = 0; i < 4; i++)
	{
		num = 0;
		position cur = selected_chess->cur_p;
		bool flag = false;
		for (int j = 1; j < 10; j++)
		{
			cur = pos_add(cur, direct[i]);
			if (equal(cur, targ))
			{
				flag = true;
				break;
			}
			if (map[cur.x][cur.y] != &null_chess) num++;
		}
		if (flag) break;
		else num = 0;
	}
	printf("num: %d", num);
	return num;
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
			short col = (msg.x - INTERVAL / 2) / GRID_SIZE;
			short row = (msg.y - INTERVAL / 2) / GRID_SIZE;
			col = 7;
			row = 7;
			position targ = position{ col, row };
			//printf("targ: %d %d\n", targ.x, targ. y);
			select(targ);
			//chessMove(targ); 这里多写了一个chessMove debug半天才发现真是服了
		}

	}
}