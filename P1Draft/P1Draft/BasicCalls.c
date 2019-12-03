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
	if (_Min == _Max)
		return _Min;
	return rand() % (_Max + 1 - _Min) + _Min;
}

Location* MapLocationGet(Map* map, int x, int y) {
	return &(map->Locations[y][x]);
}