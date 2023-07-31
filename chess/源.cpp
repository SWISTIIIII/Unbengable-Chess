#include <stdio.h>
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
			else if (state.state == BEGIN)
			{
				state.endr = row;
				state.endc = col;
				state.state = BEGIN;
			}
			chessMove();
		}

	}
}


//�ƶ�����
void chessMove()
{
	printf("beg(%d %d) end(%d %d)\n", state.begr, state.begc, state.endr, state.endc);
	bool canMove = false;
	//ʲô����¿����ƶ�����
	if ((!(state.begr == state.endr && state.begc == state.endc)) &&//����Ĳ���ͬһ������
		state.begc != -1 && state.begr != -1 &&//�±�Ӧ�Ϸ�
		map[state.begr][state.begc].id != NONE )//&&//�����Ӳ����ƶ�
		//map[state.begr][state.begc].type != map[state.endr][state.endc].type)//���ܳ��Լ�
	{
		switch (map[state.begr][state.begc].id)
		{
		case ܇:		
		case ��:
			if (state.begr==state.endr||state.begc==state.endc)
			{
				//��ʼ��ͽ�����֮���Ƿ����谭
				canMove = true;
			}
			break;
		case �R:		
		case ��:
			break;
		case ��:
		case ��:
			break;
		case ʿ:
		case ��:
			break;
		case ˧:		
		case ��:
			break;
		case ��:		
		case �h:
			break;
		case ��:
		case ��:
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