#pragma once
typedef struct {
	int x;
	int y;
} position;

typedef position vector2;

vector2 ches[40];

vector2* get_chev()
{
	for (short i = 1; i < 11; i++)
		ches[i] = vector2{ i, 0 };
	for (short i = 11; i < 21; i++)
		ches[i] = vector2{ 0, i - 10};
	for (short i = 21, v = -1; i < 31; i++, v--)
		ches[i] = vector2{ v, 0 };
	for (short i = 21, v = -1; i < 31; i++, v--)
		ches[i] = vector2{ 0, v };
	return ches;
} 