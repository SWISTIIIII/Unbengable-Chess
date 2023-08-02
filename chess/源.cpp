#include <stdio.h>
#include <math.h>
#include <graphics.h>

#define ROW 10
#define COL 9
#define INTERVAL 45//�߾�
#define GRID_SIZE 68//���ӿ��


enum Pieces
{
	NONE=-1,
	܇,�R,��,ʿ,˧, ��,��,
	��, ��, ��, ��, ��, �h, ��,
	BEGIN,END,
};

enum Pieces redChess[] = { ܇,�R,��,ʿ,˧,��,�� };
enum Pieces blackChess[] = { ��, ��, ��, ��, ��, �h, �� };

const char* ChessName[] = { "܇","�R","��","ʿ","˧","��","��","��", "��", "��", "��", "��", "�h", "��" };

//��������
struct Chess
{
	enum Pieces id;
	DWORD type;//red or black
	short x;
	short y;
	bool isRiver;//�Ƿ����
};

//��ͼ
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


//��ӡ����
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

//��ʼ������
void init()
{
	//������ͼ
	for (size_t i = 0; i < ROW; i++)
	{
		size_t temp = 0;
		for (size_t k = 0; k < COL; k++)
		{
			map[i][k].id = NONE;//������Ϊ��
			if (i <= 4)//black chess
			{
				map[i][k].type = BLACK;
				if (i == 0)//��һ������
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
				//��
				if (i == 2 && (k == 1 || k == 7))
				{
					map[i][k].id = blackChess[5];
				}
				//��
				if (i == 3 && k % 2 == 0)
				{
					map[i][k].id = blackChess[6];
				}
			}
			else//red chess
			{
				map[i][k].type = RED;
				if (i == 9)//��һ������
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
				//��
				if (i == 7 && (k == 1 || k == 7))
				{
					map[i][k].id = redChess[5];
				}
				//��
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



//����
void draw()
{
	setfillcolor(RGB(252, 215, 162));
	setlinestyle(PS_SOLID, 2);
	//������ʽ
	settextstyle(30, 0, "����");

	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t k = 0; k < COL; k++)
		{
			if (map[i][k].id == NONE)
				continue;

			settextcolor(map[i][k].type);
			setlinecolor(map[i][k].type);
			//��������
			fillcircle(map[i][k].x, map[i][k].y, 30);
			fillcircle(map[i][k].x, map[i][k].y, 25);
			outtextxy(map[i][k].x-15, map[i][k].y-15, ChessName[map[i][k].id]);
		}
	}
}






//ˢ�������Ϣ
// 
// 
//�ض�����³������Ӳ�����λ��
//��������Ϊendr,endc������Ϣ����һ��begr,begc��Ϣ����ֱ��Ӱ��
//
// 
//�ж���ȷ
void refresh()
{
	state.endc = NONE;
	state.endr = NONE;
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
			
			//�±�У׼
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

//�ж�λ�ü���������
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
	//�ж�ˮƽ�������ֱ����
	//����
	//�ж�֮���Ƿ�������
	return cnt;
}

//�жϻغ�
bool isRound = true;

//�ƶ�����
void chessMove()
{
	printf("beg(%d %d) end(%d %d)\n", state.begr, state.begc, state.endr, state.endc);
	bool canMove = false;
	//ʲô����¿����ƶ�����
	if ((!(state.begr == state.endr && state.begc == state.endc)) &&//����Ĳ���ͬһ������
		state.endr != -1 && state.begr != -1 &&//�±�Ӧ�Ϸ�
		map[state.begr][state.begc].id != NONE )//�����Ӳ����ƶ�
		//map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���ܳ��Լ�
	{
		switch (map[state.begr][state.begc].id)
		{


		case ܇:
			if (isRound)
			{
				if (state.begr == state.endr || state.begc == state.endc)
				{
					if (hasBlock(&state) == 0)//��ʼ��ͽ�����֮���Ƿ����谭
					{
						if ((map[state.endr][state.endc].id == NONE) || //Ŀ��λ��������
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//Ŀ��λ���ез�����
						{
							canMove = true;
							isRound = false;
						}

					}
				}
			}
			break;
		case ��:
			if(!isRound)
			{
				if (state.begr == state.endr || state.begc == state.endc)
				{
					if (hasBlock(&state) == 0)//��ʼ��ͽ�����֮���Ƿ����谭
					{
						if ((map[state.endr][state.endc].id == NONE) || //Ŀ��λ��������
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//Ŀ��λ���ез�����
						{
							canMove = true;
							isRound = true;
						}

					}
				}
			}
			break;



		case �R:
			if (isRound)
			{
				//if ((abs(state.begr - state.endr) == 1 && abs(state.begc - state.endc) == 2) ||
					//(abs(state.begr - state.endr) == 2 && abs(state.begc - state.endc) == 1))//������
				if ((state.begc - state.endc == 2 && abs(state.begr - state.endr) == 1 && map[state.begr][state.begc - 1].id == NONE) ||
					(state.begc - state.endc == -2 && abs(state.begr - state.endr) == 1 && map[state.begr][state.begc + 1].id == NONE) ||
					(state.begr - state.endr == 2 && abs(state.begc - state.endc) == 1 && map[state.begr - 1][state.begc].id == NONE) ||
					(state.begr - state.endr == -2 && abs(state.begc - state.endc) == 1 && map[state.begr + 1][state.begc].id == NONE))//�������ҿ��Ǳ��������
				{
					if (map[state.endr][state.endc].id == NONE ||
						map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
					{
						canMove = true;
						isRound = false;
					}
				}
			}
			break;
		case ��:
			if (!isRound)
			{
				//if ((abs(state.begr - state.endr) == 1 && abs(state.begc - state.endc) == 2) ||
					//(abs(state.begr - state.endr) == 2 && abs(state.begc - state.endc) == 1))//������
				if ((state.begc - state.endc == 2 && abs(state.begr - state.endr) == 1 && map[state.begr][state.begc - 1].id == NONE) ||
					(state.begc - state.endc == -2 && abs(state.begr - state.endr) == 1 && map[state.begr][state.begc + 1].id == NONE) ||
					(state.begr - state.endr == 2 && abs(state.begc - state.endc) == 1 && map[state.begr - 1][state.begc].id == NONE) ||
					(state.begr - state.endr == -2 && abs(state.begc - state.endc) == 1 && map[state.begr + 1][state.begc].id == NONE))//�������ҿ��Ǳ��������
				{
					if (map[state.endr][state.endc].id == NONE ||
						map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
					{
						canMove = true;
						isRound = true;
					}
				}
			}
			break;



		case ��:
			if (isRound)
			{
				if ((state.begr <= 4 && state.endr <= 4) || state.begr > 4 && state.endr > 4)//���ɹ���
				{
					if ((state.begc - state.endc == 2 && state.begr - state.endr == 2 && map[state.begr - 1][state.begc - 1].id == NONE) ||
						(state.begc - state.endc == 2 && state.begr - state.endr == -2 && map[state.begr + 1][state.begc - 1].id == NONE) ||
						(state.begc - state.endc == -2 && state.begr - state.endr == 2 && map[state.begr - 1][state.begc + 1].id == NONE) ||
						(state.begc - state.endc == -2 && state.begr - state.endr == -2 && map[state.begr + 1][state.begc + 1].id == NONE))//������
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
						{
							canMove = true;
							isRound = false;
						}
					}
				}
			}
			break;
		case ��:
			if (!isRound)
			{
				if ((state.begr <= 4 && state.endr <= 4) || state.begr > 4 && state.endr > 4)//���ɹ���
				{
					if ((state.begc - state.endc == 2 && state.begr - state.endr == 2 && map[state.begr - 1][state.begc - 1].id == NONE) ||
						(state.begc - state.endc == 2 && state.begr - state.endr == -2 && map[state.begr + 1][state.begc - 1].id == NONE) ||
						(state.begc - state.endc == -2 && state.begr - state.endr == 2 && map[state.begr - 1][state.begc + 1].id == NONE) ||
						(state.begc - state.endc == -2 && state.begr - state.endr == -2 && map[state.begr + 1][state.begc + 1].id == NONE))//������
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
						{
							canMove = true;
							isRound = true;
						}
					}
				}
			}
			break;



		case ʿ:
			if (isRound)
			{
				if ((state.endr <= 2 || state.endr >= 7) && (state.endc >= 3 && state.endc <= 5))//�Ź���
				{
					if (abs(state.begr - state.endr) == 1 && abs(state.begc - state.endc) == 1)//б��һ��
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
						{
							canMove = true;
							isRound = false;
						}
					}
				}
			}
			break;
		case ��:
			if (!isRound)
			{
				if ((state.endr <= 2 || state.endr >= 7) && (state.endc >= 3 && state.endc <= 5))//�Ź���
				{
					if (abs(state.begr - state.endr) == 1 && abs(state.begc - state.endc) == 1)//б��һ��
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
						{
							canMove = true;
							isRound = true;
						}
					}
				}
			}
			break;



		case ˧:
			if (isRound)
			{
				if ((state.endr <= 2 || state.endr >= 7) && (state.endc >= 3 && state.endc <= 5))//�Ź���
				{
					if ((abs(state.begr - state.endr) == 1 || abs(state.begc - state.endc) == 1) &&
						(abs(state.begr - state.endr) != abs(state.begc - state.endc)))//ֱ��һ��
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
						{
							canMove = true;
							isRound = false;
						}
					}
				}
			}
			break;
		case ��:
			if (!isRound)
			{
				if ((state.endr <= 2 || state.endr >= 7) && (state.endc >= 3 && state.endc <= 5))//�Ź���
				{
					if ((abs(state.begr - state.endr) == 1 || abs(state.begc - state.endc) == 1) &&
						(abs(state.begr - state.endr) != abs(state.begc - state.endc)))//ֱ��һ��
					{
						if (map[state.endr][state.endc].id == NONE ||
							map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
						{
							canMove = true;
							isRound = true;
						}
					}
				}
			}
			break;



		case ��:
			if (isRound)
			{
				if (state.begr == state.endr || state.begc == state.endc)
				{
					if ((hasBlock(&state) == 0 && map[state.endr][state.endc].id == NONE) || //ƽ��
						(hasBlock(&state) == 1 && map[state.endr][state.endc].id != NONE &&
							map[state.begr][state.begc].type != map[state.endr][state.endc].type))//����
					{
						canMove = true;
						isRound = false;
					}
				}
			}
			break;
		case �h:
			if (!isRound)
			{
				if (state.begr == state.endr || state.begc == state.endc)
				{
					if ((hasBlock(&state) == 0 && map[state.endr][state.endc].id == NONE) || //ƽ��
						(hasBlock(&state) == 1 && map[state.endr][state.endc].id != NONE &&
							map[state.begr][state.begc].type != map[state.endr][state.endc].type))//����
					{
						canMove = true;
						isRound = true;
					}
				}
			}
			break;



		case ��:
			if (isRound)
			{
				if (map[state.begr][state.begc].type == RED)
				{
					if (state.begr <= 4)//����
					{
						if ((state.begr - state.endr == 1 || abs(state.begc - state.endc) == 1) &&
							(abs(state.begr - state.endr) != abs(state.begc - state.endc)))
						{
							if (map[state.endr][state.endc].id == NONE ||
								map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
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
								map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
							{
								canMove = true;
								isRound = false;
							}
						}
					}
				}
			}
			break;
		case ��:
			if (!isRound)
			{
				if (map[state.begr][state.begc].type == BLACK)
				{
					if (state.begr >= 5)//����
					{
						if ((state.begr - state.endr == -1 || abs(state.begc - state.endc) == 1) &&
							(abs(state.begr - state.endr) != abs(state.begc - state.endc)))
						{
							if (map[state.endr][state.endc].id == NONE ||
								map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
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
								map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���������մ���з����Ӵ�
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
	//��������
	initgraph(628, 698,EW_SHOWCONSOLE);
	//���ñ���ģʽ
	setbkmode(TRANSPARENT);
	//������
	IMAGE img_board;
	loadimage(&img_board, "./res/chessboard.png");



	init();
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
	
	show();


	getchar();
	return 0;
}