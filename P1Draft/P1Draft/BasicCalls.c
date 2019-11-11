#include "BasicCalls.h"

bool IsPointEqu(struct Point A, struct Point B)
{
	if (A.X == B.X)
	{
		if (A.Y == B.Y)
		{
			return true;
		}
	}
	return false;
}

struct Point SetPoint(int X, int Y)
{
	struct Point NewPoint = { X, Y };
	return NewPoint;
}

int GetRandomNumberRanged(int _Min, int _Max)
{
	return rand() % (_Max + 1 - _Min) + _Min;
}

bool IsNumberOdd(int _Value)
{
	return _Value % 2;
}