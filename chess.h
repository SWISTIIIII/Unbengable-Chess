#pragma once
#include <graphics.h>
#include "vector.h"
#include<string>
#include <iostream>

#define tot_num 4
#define ROW 10
#define COL 9
#define INTERVAL 45//边距
#define GRID_SIZE 68//格子宽度
#define NONE -1
#define R1 0
#define R2 1
#define B1 2
#define B2 3

enum tags {
	che, xiang, ma, shi, pao, bing, jiang, null
};

const char* ChessName[] = { "車","馬","相","士","帅","炮","兵","车", "马", "象", "仕", "将", "砲", "卒" };
struct Chess
{
	const char* chess_name;
	tags tag;
	DWORD type;//red or black
	position cur_p;
	position init_p;
	bool isDead;
	bool isRiver;
	vector2* capabilitys;
	short cap_num;
};

Chess
cher1, cher2, cheb1, cheb2,
mar1, mar2, mab1, mab2,
shir1, shir2, shib1, shib2,
paor1, paor2, paob1, paob2,
xiangr1, xiangr2, xiangb1, xiangb2,
jiangr, jiangb,
zur1, zur2, zur3, zur4, zur5, zub1, zub2, zub3, zub4, zub5
;

Chess null_chess;

Chess* selected_chess = &null_chess;

Chess* map[9][10] = {&null_chess};

Chess* chess[32] = {
	&cher1,&cher2,&cheb1,&cheb2,
	&mar1,&mar2,&mab1,&mab2,
	&shir1,&shir2,&shib1,&shib2,
	&paor1,&paor2,&paob1,&paob2,
	&xiangr1,&xiangr2,&xiangb1,&xiangb2,
	&jiangr,&jiangb,
	&zur1,&zur2,&zur3,&zur4,&zur5,&zub1,&zub2,&zub3,&zub4,&zub5
};

void printp(position pos, std::string desc);
void initMap();
int block_detect_stra(position targ);
void init_chess(Chess* chess, const char* name, position init, DWORD type, vector2* capabilitys, int cap_num, tags tag);
bool equal(position x, position y)
{
	if (x.x == y.x && x.y == y.y) return true;
	return false;
}
position pos_add(position x, position y)
{
	return position{ x.x + y.x, x.y + y.y };
}
int getspotx(Chess* chess) {
	return chess->cur_p.x * GRID_SIZE + INTERVAL;
}

int getspoty(Chess* chess) {
	return chess->cur_p.y * GRID_SIZE + INTERVAL;
}

void init() 
{
	init_chess(&cher1, "车", position{ 0, 0 }, BLACK, get_chev(), 40, che);
	init_chess(&cher2, "车", position{ 8, 0 }, BLACK, get_chev(), 40, che);
	init_chess(&cheb2, "車", position{ 8, 9 }, RED, get_chev(), 40, che);
	init_chess(&cheb1, "車", position{ 0, 9 }, RED, get_chev(), 40, che);
	//剩下的棋子填上
	init_chess(&null_chess, "null", position{ 0, 9 }, GREEN, get_chev(), 40, null);
	initMap();
}


void init_chess(Chess* chess, const char* name, position init, DWORD type, vector2* capabilitys, int cap_num, tags tag)
{
	chess->chess_name = name;
	chess->cur_p = init;
	chess->init_p = init;
	chess->type = type;
	chess->isDead = false;
	chess->isRiver = false;
	chess->capabilitys = capabilitys;
	chess->cap_num = cap_num;
	chess->tag = tag;
}

void initMap()
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 10; j++)
			map[i][j] = &null_chess;
	for (int i = 0; i < tot_num; i++)
		map[chess[i]->cur_p.x][chess[i]->cur_p.y] = chess[i];
}

void printp(position pos, std::string desc)
{
	std::cout << desc << ' ';
	printf("(%d, %d)\n", pos.x, pos.y);
}