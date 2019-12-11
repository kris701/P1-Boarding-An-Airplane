#include "BasicCalls.h"

// A  function to check if two points are equal to find out if a passenger is in it's seat
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

// A function to allocate a point to a struct
Point SetPoint(int X, int Y)
{
	Point NewPoint = { X, Y };
	return NewPoint;
}

// A function to get a random number
int GetRandomNumberRanged(int _Min, int _Max)
{
	if (_Min == _Max)
		return _Min;
	return rand() % (_Max + 1 - _Min) + _Min;
}

// A function that finds which passenger is at a given x and y coordinat
Location* GetMapLocation(Map* map, int x, int y) {
	return &(map->Locations[y][x]);
}

// A function that uses function GetMapLocation to which passenger is at a given x and y coordinat from the struct Point
Location* GetMapLocationFromPoint(Map* map, Point point) {
	return GetMapLocation(map, point.X, point.Y);
}

// A function to finds longest line in file and how many chars it contain
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

// A function to finde out how many lines the file contain
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

// A bool function to check if a file exist
bool FileExists(FILE* _File)
{
	if (_File != NULL)
		return true;
	return false;
}

// A function to open a file
void DoOpenFile(FILE** _File, const char* _FileName, const char* _Parameter)
{
	_mkdir("./Output"); // Creates a directory. If it already exists. Will do nothing.
	fopen_s(&(*_File), _FileName, _Parameter);
}

// A function to find a string between chars
void FindStrBetweenChars(char* SubStringBuffer[], char* Target[], int TargetLength, char FromChar, char ToChar)
{
	*SubStringBuffer = strchr(*SubStringBuffer, FromChar) + 1;
	int IndexOffset = 0;

	for (int j = 0; j < TargetLength; j++)
	{
		if ((*SubStringBuffer)[j] != ToChar)
		{
			(*Target)[IndexOffset] = (*SubStringBuffer)[j];
			IndexOffset++;
		}
		else
			break;
	}
	*SubStringBuffer = strchr(*SubStringBuffer, ToChar);
}