#include <stdio.h>
#include <math.h>
#include <graphics.h>

#define ROW 10
#define COL 9
#define INTERVAL 45//边距
#define GRID_SIZE 68//格子宽度


enum Pieces
{
	NONE=-1,
	車,馬,相,士,帅, 炮,兵,
	车, 马, 象, 仕, 将, 砲, 卒,
	BEGIN,END,
};

enum Pieces redChess[] = { 車,馬,相,士,帅,炮,兵 };
enum Pieces blackChess[] = { 车, 马, 象, 仕, 将, 砲, 卒 };

const char* ChessName[] = { "車","馬","相","士","帅","炮","兵","车", "马", "象", "仕", "将", "砲", "卒" };

//棋子属性
struct Chess
{
	enum Pieces id;
	DWORD type;//red or black
	short x;
	short y;
	bool isRiver;//是否过河
};

//地图
struct Chess map[ROW][COL];

struct State
{
	int begr;
	int begc;
	int endr;
	int endc;
	int state;
}state={-1,-1,-1,-1,BEGIN};

void chessMove();


//打印数组
void show()
{
	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t k = 0; k < COL; k++)
		{
			printf("%2d ", map[i][k].id);
		}
		printf("\n");
	}
}

//初始化数据
void init()
{
	//遍历地图
	for (size_t i = 0; i < ROW; i++)
	{
		size_t temp = 0;
		for (size_t k = 0; k < COL; k++)
		{
			map[i][k].id = NONE;//将棋盘为空
			if (i <= 4)//black chess
			{
				map[i][k].type = BLACK;
				if (i == 0)//第一行棋子
				{
					//0 1 2 3 4
					if (k <= 4)
					{
						temp = k;
					}
					else 
					{
						temp = 8 - k;
					}
					map[i][k].id = blackChess[temp];
				}
				//炮
				if (i == 2 && (k == 1 || k == 7))
				{
					map[i][k].id = blackChess[5];
				}
				//兵
				if (i == 3 && k % 2 == 0)
				{
					map[i][k].id = blackChess[6];
				}
			}
			else//red chess
			{
				map[i][k].type = RED;
				if (i == 9)//第一行棋子
				{
					//0 1 2 3 4
					if (k <= 4)
					{
						temp = k;
					}
					else
					{
						temp = 8 - k;
					}
					map[i][k].id = redChess[temp];
				}
				//炮
				if (i == 7 && (k == 1 || k == 7))
				{
					map[i][k].id = redChess[5];
				}
				//兵
				if (i == 6 && k % 2 == 0)
				{
					map[i][k].id = redChess[6];
				}
			}
			map[i][k].isRiver = false;
			map[i][k].x = k * GRID_SIZE + INTERVAL;
			map[i][k].y = i * GRID_SIZE + INTERVAL;
		}
	}
}



//绘制
void draw()
{
	setfillcolor(RGB(252, 215, 162));
	setlinestyle(PS_SOLID, 2);
	//文字样式
	settextstyle(30, 0, "楷体");

	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t k = 0; k < COL; k++)
		{
			if (map[i][k].id == NONE)
				continue;

			settextcolor(map[i][k].type);
			setlinecolor(map[i][k].type);
			//绘制棋子
			fillcircle(map[i][k].x, map[i][k].y, 30);
			fillcircle(map[i][k].x, map[i][k].y, 25);
			outtextxy(map[i][k].x-15, map[i][k].y-15, ChessName[map[i][k].id]);
		}
	}
}






//刷新鼠标信息
// 
// 
//特定情况下出现棋子不合理位移
//初步怀疑为endr,endc坐标信息对下一次begr,begc信息产生直接影响
//
// 
//判断正确
void refresh()
{
	state.endc = NONE;
	state.endr = NONE;
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
			
			//下标校准
			if (msg.x > map[row][col].x + 25 && msg.y < map[row][col].y + 25)
			{
				col++;
			}
			if (msg.x < map[row][col].x + 25 && msg.y > map[row][col].y + 25)
			{
				row++;
			}
			if (msg.x > map[row][col].x + 25 && msg.y > map[row][col].y + 25)
			{
				col++;
				row++;
			}
			//printf("(%d %d)\n", row, col);
			
			if (state.state == BEGIN)
			{
				state.begr = row;
				state.begc = col;
				state.state = END;
			}
			else if (state.state == END)
			{
				state.endr = row;
				state.endc = col;
				state.state = BEGIN;
			}
			chessMove();
			refresh();
		}

	}
}

//判断位置间棋子数量
int hasBlock(struct State* state)
{
	int cnt = 0;

	if (state->begr == state->endr)
	{
		for (size_t i = min(state->begc, state->endc)+1;i< max(state->begc, state->endc);i++)
		{
			if (map[state->begr][i].id != NONE)
			{
				cnt++;
			}
		}
	}
	else if (state->begc == state->endc)
	{
		for (size_t i = min(state->begr, state->endr) + 1; i < max(state->begr, state->endr); i++)
		{
			if (map[i][state->begc].id != NONE)
			{
				cnt++;
			}
		}
	}
	//判断水平方向和竖直方向
	//遍历
	//判断之间是否有棋子
	return cnt;
}

//判断回合
bool isRound = true;

//移动棋子
void chessMove()
{
	printf("beg(%d %d) end(%d %d)\n", state.begr, state.begc, state.endr, state.endc);
	bool canMove = false;
	//什么情况下可以移动棋子
	if ((!(state.begr == state.endr && state.begc == state.endc)) &&//点击的不是同一个棋子
		state.endr != -1 && state.begr != -1 &&//下标应合法
		map[state.begr][state.begc].id != NONE )//无棋子不可移动
		//map[state.begr][state.begc].type != map[state.endr][state.endc].type)//不能吃自己
	{
		switch (map[state.begr][state.begc].id)
		{


		case 車:
			if (isRound)
			{
				if (state.begr == state.endr || state.begc == state.endc)
				{
					if (hasBlock(&state) == 0)//起始点和结束点之间是否有阻碍
					{
						if ((map[state.endr][state.endc].id == NONE) || //目标位置无棋子
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//目标位置有敌方棋子
						{
							canMove = true;
							isRound = false;
						}

					}
				}
			}
			break;
		case 车:
			if(!isRound)
			{
				if (state.begr == state.endr || state.begc == state.endc)
				{
					if (hasBlock(&state) == 0)//起始点和结束点之间是否有阻碍
					{
						if ((map[state.endr][state.endc].id == NONE) || //目标位置无棋子
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//目标位置有敌方棋子
						{
							canMove = true;
							isRound = true;
						}

					}
				}
			}
			break;



		case 馬:
			if (isRound)
			{
				//if ((abs(state.begr - state.endr) == 1 && abs(state.begc - state.endc) == 2) ||
					//(abs(state.begr - state.endr) == 2 && abs(state.begc - state.endc) == 1))//马走日
				if ((state.begc - state.endc == 2 && abs(state.begr - state.endr) == 1 && map[state.begr][state.begc - 1].id == NONE) ||
					(state.begc - state.endc == -2 && abs(state.begr - state.endr) == 1 && map[state.begr][state.begc + 1].id == NONE) ||
					(state.begr - state.endr == 2 && abs(state.begc - state.endc) == 1 && map[state.begr - 1][state.begc].id == NONE) ||
					(state.begr - state.endr == -2 && abs(state.begc - state.endc) == 1 && map[state.begr + 1][state.begc].id == NONE))//马走日且考虑别马腿情况
				{
					if (map[state.endr][state.endc].id == NONE ||
						map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
					{
						canMove = true;
						isRound = false;
					}
				}
			}
			break;
		case 马:
			if (!isRound)
			{
				//if ((abs(state.begr - state.endr) == 1 && abs(state.begc - state.endc) == 2) ||
					//(abs(state.begr - state.endr) == 2 && abs(state.begc - state.endc) == 1))//马走日
				if ((state.begc - state.endc == 2 && abs(state.begr - state.endr) == 1 && map[state.begr][state.begc - 1].id == NONE) ||
					(state.begc - state.endc == -2 && abs(state.begr - state.endr) == 1 && map[state.begr][state.begc + 1].id == NONE) ||
					(state.begr - state.endr == 2 && abs(state.begc - state.endc) == 1 && map[state.begr - 1][state.begc].id == NONE) ||
					(state.begr - state.endr == -2 && abs(state.begc - state.endc) == 1 && map[state.begr + 1][state.begc].id == NONE))//马走日且考虑别马腿情况
				{
					if (map[state.endr][state.endc].id == NONE ||
						map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
					{
						canMove = true;
						isRound = true;
					}
				}
			}
			break;



		case 相:
			if (isRound)
			{
				if ((state.begr <= 4 && state.endr <= 4) || state.begr > 4 && state.endr > 4)//不可过河
				{
					if ((state.begc - state.endc == 2 && state.begr - state.endr == 2 && map[state.begr - 1][state.begc - 1].id == NONE) ||
						(state.begc - state.endc == 2 && state.begr - state.endr == -2 && map[state.begr + 1][state.begc - 1].id == NONE) ||
						(state.begc - state.endc == -2 && state.begr - state.endr == 2 && map[state.begr - 1][state.begc + 1].id == NONE) ||
						(state.begc - state.endc == -2 && state.begr - state.endr == -2 && map[state.begr + 1][state.begc + 1].id == NONE))//象走田
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
						{
							canMove = true;
							isRound = false;
						}
					}
				}
			}
			break;
		case 象:
			if (!isRound)
			{
				if ((state.begr <= 4 && state.endr <= 4) || state.begr > 4 && state.endr > 4)//不可过河
				{
					if ((state.begc - state.endc == 2 && state.begr - state.endr == 2 && map[state.begr - 1][state.begc - 1].id == NONE) ||
						(state.begc - state.endc == 2 && state.begr - state.endr == -2 && map[state.begr + 1][state.begc - 1].id == NONE) ||
						(state.begc - state.endc == -2 && state.begr - state.endr == 2 && map[state.begr - 1][state.begc + 1].id == NONE) ||
						(state.begc - state.endc == -2 && state.begr - state.endr == -2 && map[state.begr + 1][state.begc + 1].id == NONE))//象走田
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
						{
							canMove = true;
							isRound = true;
						}
					}
				}
			}
			break;



		case 士:
			if (isRound)
			{
				if ((state.endr <= 2 || state.endr >= 7) && (state.endc >= 3 && state.endc <= 5))//九宫内
				{
					if (abs(state.begr - state.endr) == 1 && abs(state.begc - state.endc) == 1)//斜走一格
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
						{
							canMove = true;
							isRound = false;
						}
					}
				}
			}
			break;
		case 仕:
			if (!isRound)
			{
				if ((state.endr <= 2 || state.endr >= 7) && (state.endc >= 3 && state.endc <= 5))//九宫内
				{
					if (abs(state.begr - state.endr) == 1 && abs(state.begc - state.endc) == 1)//斜走一格
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
						{
							canMove = true;
							isRound = true;
						}
					}
				}
			}
			break;



		case 帅:
			if (isRound)
			{
				if ((state.endr <= 2 || state.endr >= 7) && (state.endc >= 3 && state.endc <= 5))//九宫内
				{
					if ((abs(state.begr - state.endr) == 1 || abs(state.begc - state.endc) == 1) &&
						(abs(state.begr - state.endr) != abs(state.begc - state.endc)))//直走一格
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
						{
							canMove = true;
							isRound = false;
						}
					}
				}
			}
			break;
		case 将:
			if (!isRound)
			{
				if ((state.endr <= 2 || state.endr >= 7) && (state.endc >= 3 && state.endc <= 5))//九宫内
				{
					if ((abs(state.begr - state.endr) == 1 || abs(state.begc - state.endc) == 1) &&
						(abs(state.begr - state.endr) != abs(state.begc - state.endc)))//直走一格
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
						{
							canMove = true;
							isRound = true;
						}
					}
				}
			}
			break;



		case 炮:
			if (isRound)
			{
				if (state.begr == state.endr || state.begc == state.endc)
				{
					if ((hasBlock(&state) == 0 && map[state.endr][state.endc].id == NONE) || //平移
						(hasBlock(&state) == 1 && map[state.endr][state.endc].id != NONE &&
							map[state.begr][state.begc].type != map[state.endr][state.endc].type))//吃子
					{
						canMove = true;
						isRound = false;
					}
				}
			}
			break;
		case 砲:
			if (!isRound)
			{
				if (state.begr == state.endr || state.begc == state.endc)
				{
					if ((hasBlock(&state) == 0 && map[state.endr][state.endc].id == NONE) || //平移
						(hasBlock(&state) == 1 && map[state.endr][state.endc].id != NONE &&
							map[state.begr][state.begc].type != map[state.endr][state.endc].type))//吃子
					{
						canMove = true;
						isRound = true;
					}
				}
			}
			break;



		case 兵:
			if (isRound)
			{
				if (map[state.begr][state.begc].type == RED)
				{
					if (state.begr <= 4)//过河
					{
						if ((state.begr - state.endr == 1 || abs(state.begc - state.endc) == 1) &&
							(abs(state.begr - state.endr) != abs(state.begc - state.endc)))
						{
							if (map[state.endr][state.endc].id == NONE ||
								map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
							{
								canMove = true;
								isRound = false;
							}
						}
					}
					else
					{
						if (state.begr - state.endr == 1 &&
							abs(state.begr - state.endr) != abs(state.begc - state.endc))
						{
							if (map[state.endr][state.endc].id == NONE ||
								map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
							{
								canMove = true;
								isRound = false;
							}
						}
					}
				}
			}
			break;
		case 卒:
			if (!isRound)
			{
				if (map[state.begr][state.begc].type == BLACK)
				{
					if (state.begr >= 5)//过河
					{
						if ((state.begr - state.endr == -1 || abs(state.begc - state.endc) == 1) &&
							(abs(state.begr - state.endr) != abs(state.begc - state.endc)))
						{
							if (map[state.endr][state.endc].id == NONE ||
								map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
							{
								canMove = true;
								isRound = true;
							}
						}
					}
					else
					{
						if (state.begr - state.endr == -1 &&
							abs(state.begr - state.endr) != abs(state.begc - state.endc))
						{
							if (map[state.endr][state.endc].id == NONE ||
								map[state.begr][state.begc].type != map[state.endr][state.endc].type)//仅可走至空处或敌方棋子处
							{
								canMove = true;
								isRound = true;
							}
						}
					}
				}
			}
			
			break;

		default:
			break;
		}
		if (canMove)
		{
			printf("canMove\n");
			map[state.endr][state.endc].id = map[state.begr][state.begc].id;
			map[state.begr][state.begc].id = NONE;

			map[state.endr][state.endc].isRiver = map[state.begr][state.begc].isRiver;
			map[state.endr][state.endc].type = map[state.begr][state.begc].type;
		}
	}
}






int main()
{
	//创建窗口
	initgraph(628, 698,EW_SHOWCONSOLE);
	//设置背景模式
	setbkmode(TRANSPARENT);
	//贴棋盘
	IMAGE img_board;
	loadimage(&img_board, "./res/chessboard.png");



	init();
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
	
	show();


	getchar();
	return 0;
}