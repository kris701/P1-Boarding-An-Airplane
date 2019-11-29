#include "BasicCalls.h"

bool IsPointEqual(Point A, Point B)
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

Point SetPoint(int X, int Y)
{
	Point NewPoint = { X, Y };
	return NewPoint;
}

int GetRandomNumberRanged(int _Min, int _Max)
{
	return rand() % (_Max + 1 - _Min) + _Min;
}