#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <stdio.h>
#include "chess.h"
#include"chess_game.h"



//��ʼ��socket��
void init_socket()
{
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
	{
		printf("WSAStartup failed:%d\n", WSAGetLastError());
	}
}

//����socket��
void clean_socket()
{
	WSACleanup();
}

//�ͻ���
SOCKET connectToHost(const char* ip,UINT16 port)
{
	//����socket
	//IPv4 TCP
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("socket failed:%d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	//������������
	SOCKADDR_IN addr={0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip);

	int size = sizeof(addr);
	if (connect(sock, (struct sockaddr*)&addr, size) != 0)
	{
		printf("connect failed:%d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}
	return sock;
}

//������
SOCKET startServer(UINT16 port)
{
	//����socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("socket failed:%d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	//��IP��ַ���˿�
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = ADDR_ANY;//�󶨱�������������ַ
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		printf("bind failed:%d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	//����
	if (listen(sock, SOMAXCONN) != 0)
	{
		printf("listen failed:%d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}
	return sock;
}

//��ȡ�����Ϣ
const char* getmouseMessage()
{
	static char buf[BUFSIZ] = { 0 };

	ExMessage msg;//������Ϣ�ṹ�����
	if (peekmessage(&msg, EM_MOUSE))
	{
		if (msg.message == WM_LBUTTONDOWN)//����������
		{
			//ͨ���������ó���������±�
			//k * GRID_SIZE + INTERVAL=x
			short col = (msg.x - INTERVAL / 2) / GRID_SIZE;
			short row = (msg.y - INTERVAL / 2) / GRID_SIZE;
			
			sprintf(buf, "%d %d", col, row);
			return buf;
		}
	}
}


int main()
{
	init_socket();

	

	//client
	SOCKET sock = connectToHost("127.0.0.1",6666);//���ӱ���
	//��������
	char buf[1024] = { 0 };
	int len = recv(sock, buf, 1024, 0);
	if (len > 0)
	{
		printf("%s\n",buf);
	}
	else if (len == 0) //�����ر�����
	{
		printf("normal close\n");
		closesocket(sock);
	}
	else //��������
	{
		printf("recv error:%d\n", WSAGetLastError());
		closesocket(sock);
	}


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