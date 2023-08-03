#pragma once
typedef struct {
	int x;
	int y;
} position;

typedef position vector2;

vector2 ches[41];
vector2 mas[8] = {
	position{2, -1}, {2, 1}, {1, 2}, {-1, 2},
	{-2, 1}, {-2, -1}, {1, -2}, {-1, -2}
};

vector2 xiangs[8] = {
	position{2, 2}, {2, -2}, {2, 2}, {-2, 2},
	{-2, 2}, {-2, -2}, {2, -2}, {-2, -2}
};

vector2 shis[4] = {
	position{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

vector2* get_chev()
{
	for (short i = 1; i < 11; i++)
		ches[i] = vector2{ i, 0 };
	for (short i = 11; i < 21; i++)
		ches[i] = vector2{ 0, i - 10};
	for (short i = 21, v = -1; i < 31; i++, v--)
		ches[i] = vector2{ v, 0 };
	for (short i = 31, v = -1; i < 41; i++, v--)
		ches[i] = vector2{ 0, v };
	return ches;
} 

