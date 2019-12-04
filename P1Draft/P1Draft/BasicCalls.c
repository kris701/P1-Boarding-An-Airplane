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

Location* GetMapLocation(Map* map, int x, int y) {
	return &(map->Locations[y][x]);
}
Location* GetMapLocationFromPoint(Map* map, Point point) {
	return GetMapLocation(map, point.X, point.Y);
}

int GetNumberOfCharsForLongestLineInFile(FILE* _File)
{
	// Goes through each line and finds the longest
	int tmpWidth = 0, highestWidth = 0;
	long int initialFileCursorLocation = ftell(_File);
	fseek(_File, 0, SEEK_SET);

	char ch;
	while (!feof(_File))
	{
		ch = fgetc(_File);
		tmpWidth++;
		if (ch == '\n')
		{
			highestWidth = max(tmpWidth, highestWidth);
			tmpWidth = 0;
		}
	}

	fseek(_File, initialFileCursorLocation, SEEK_SET);
	return highestWidth + 1; // Null-byte
}

int GetNumberOfLinesInFile(FILE* _File)
{
	int lines = 1; // Begins at line 1, if one \n is occured, then there are 2 lines
	long int initialFileCursorLocation = ftell(_File);
	fseek(_File, 0, SEEK_SET);

	char ch;
	while (!feof(_File))
	{
		ch = fgetc(_File);
		if (ch == '\n') lines++;
	}

	fseek(_File, initialFileCursorLocation, SEEK_SET);
	return lines;
}