#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <stdio.h>
#include "chess.h"
#include"chess_game.h"



//初始化socket库
void init_socket()
{
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
	{
		printf("WSAStartup failed:%d\n", WSAGetLastError());
	}
}

//清理socket库
void clean_socket()
{
	WSACleanup();
}

//客户端
SOCKET connectToHost(const char* ip, UINT16 port)
{
	//创建socket
	//IPv4 TCP
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("socket failed:%d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	//连接至服务器
	SOCKADDR_IN addr = { 0 };
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

//服务器
SOCKET startServer(UINT16 port)
{
	//创建socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("socket failed:%d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	//绑定IP地址及端口
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = ADDR_ANY;//绑定本地所有网卡地址
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		printf("bind failed:%d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	//监听
	if (listen(sock, SOMAXCONN) != 0)
	{
		printf("listen failed:%d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}
	return sock;
}

//获取点击信息
const char* getmouseMessage()
{
	static char buf[BUFSIZ] = { 0 };

	ExMessage msg;//定义消息结构体变量
	if (peekmessage(&msg, EM_MOUSE))
	{
		if (msg.message == WM_LBUTTONDOWN)//鼠标左键按下
		{
			//通过鼠标坐标得出点击数组下标
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

	//server
	SOCKET serSock = startServer(6666);
	while (1)
	{
		//接受链接
		SOCKET clientSock = accept(serSock, NULL, NULL);
		if (clientSock != INVALID_SOCKET)
		{
			const char* clickStr = getmouseMessage();
			//发送信息至客户端
			if (send(clientSock, clickStr, strlen(clickStr), 0) == SOCKET_ERROR)
				printf("send error:\n", WSAGetLastError);
			closesocket(clientSock);
		}
	}

	
	

	//创建窗口
	initgraph(628, 698, EW_SHOWCONSOLE);
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