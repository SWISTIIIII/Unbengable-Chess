#pragma once
#include <graphics.h>
#include "vector.h"
#define tot_num 32

const char* ChessName[] = { "車","馬","相","士","帅","炮","兵","车", "马", "象", "仕", "将", "砲", "卒" };
struct Chess
{
	const char* chess_name;
	DWORD type;//red or black
	position cur_p;
	position init_p;
	bool isDead;
	bool isRiver;
	bool isSelected;
	vector2* capabilitys;
	short cap_num;
};

Chess
* cher1, * cher2, * cheb1, * cheb2,
* mar1, * mar2, * mab1, * mab2,
* shir1, * shir2, * shib1, * shib2,
* paor1, * paor2, * paob1, * paob2,
* xiangr1, * xiangr2, * xiangb1, * xiangb2,
* jiangr, * jiangb,
* zur1, * zur2, * zur3, * zur4, * zur5, * zub1, * zub2, * zub3, * zub4, * zub5
;

Chess* chess[32] = {
	cher1, cher2, cheb1, cheb2,
	mar1, mar2, mab1, mab2,
	shir1, shir2, shib1, shib2,
	paor1, paor2, paob1, paob2,
	xiangr1, xiangr2, xiangb1, xiangb2,
	jiangr, jiangb,
	zur1, zur2, zur3, zur4, zur5, zub1, zub2, zub3, zub4, zub5
 };

Chess* blackChess[16] = {
	cheb1, cheb2,
	mab1, mab2,
	shib1, shib2,
	paob1, paob2,
	xiangb1, xiangb2,
	jiangb,
	zub1, zub2, zub3, zub4, zub5
};

Chess* redChess[16] = {
	cher1, cher2,
	mar1, mar2,
	shir1, shir2,
	paor1, paor2,
	xiangr1, xiangr2,
	jiangr,
	zur1, zur2, zur3, zur4, zur5
};


void init() 
{
	init_chess(cher1, "车", position{0, 0}, 1, get_chev(), 40);
	init_chess(cher2, "车", position{ 8, 0 }, 1, get_chev(), 40);
	init_chess(cheb1, "車", position{ 0, 9}, 1, get_chev(), 40);
	init_chess(cheb2, "車", position{ 8, 9 }, 1, get_chev(), 40);

}

void init_chess(Chess* chess, const char* name, position init, DWORD type, vector2* capabilitys, int cap_num)
{
	chess->chess_name = (char*)malloc(sizeof(char) * 2);
	chess->chess_name = name;
	chess->cur_p = init;
	chess->init_p = init;
	chess->type = type;
	chess->isDead = false;
	chess->isRiver = false;
	chess->capabilitys = capabilitys;
	chess->cap_num = cap_num;
	chess->isSelected = false;
}




















