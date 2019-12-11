#include "Map.h"

// A function to read a map from a file
bool ReadMapFromFile(Map* _PlaneMap, BasicSimulationRules _BasicRules, const char* OpenFile)
{
	FILE* MapFile;

	DoOpenFile(&MapFile, OpenFile, "r");

	if (!FileExists(MapFile))
		return false;

	SetMapStaticValues(MapFile, _PlaneMap);

	if (AllocateMap(_PlaneMap) == false)
	{
		return false;
	}

	SetMapValuesFromFile(MapFile, _PlaneMap);

	return true;
}

// A function to set a boarding group at a position
void MapLocationSetValue(Map* _PlaneMap, int _X, int _Y, int Value)
{
	GetMapLocation(_PlaneMap, _X, _Y)->BoardingGroup = Value;
}

// A bool function to allocate map and return true if it's a succes and false if it's fails
bool AllocateMap(Map* _PlaneMap)
{
	_PlaneMap->Locations = calloc(_PlaneMap->Height, sizeof(Location*));
	if (_PlaneMap->Locations == NULL)
	{
		fprintf(stderr, "Failed to allocate %d bytes for map width\n", (int)(_PlaneMap->Height * sizeof(Location*)));
		return false;
	}

	for (int y = 0; y < _PlaneMap->Height; y++)
	{
		_PlaneMap->Locations[y] = calloc(_PlaneMap->Width, sizeof(Location));

		if (_PlaneMap->Locations[y] == NULL)
		{
			fprintf(stderr, "Failed to allocate %d bytes for map row %d\n", (int)(_PlaneMap->Width * sizeof(Location)), y);
			return false;
		}

		for (int x = 0; x < _PlaneMap->Width; x++)
		{
			_PlaneMap->Locations[y][x].Point.X = x;
			_PlaneMap->Locations[y][x].Point.Y = y;
		}
	}

	_PlaneMap->Doors = calloc(_PlaneMap->DoorCount, sizeof(Point*));

	return true;
}

// A function to free map from memory 
void FreeMap(Map* _PlaneMap)
{
	if (_PlaneMap == NULL) return;

	if (_PlaneMap->Locations != NULL)
	{
		for (int i = 0; i < _PlaneMap->Width; i++)
		{
			if (_PlaneMap->Locations != NULL)
			{
				free(_PlaneMap->Locations[i]);
			}
		}
		free(_PlaneMap->Locations);
		_PlaneMap->Width = 0;
		_PlaneMap->Height = 0;
	}

	if (_PlaneMap->Doors != NULL)
	{
		free(_PlaneMap->Doors);
		_PlaneMap->DoorCount = 0;
	}
}

// A function to gets how many seats there are per line
int GetSeatsPerLine(FILE* _File)
{
	int tmpWidth = 0, highestWidth = 0;
	long int initialFileCursorLocation = ftell(_File);
	fseek(_File, 0, SEEK_SET);

	char ch;
	while (!feof(_File))
	{
		ch = fgetc(_File);
		switch (ch)
		{
		case '\n':
			highestWidth = max(tmpWidth, highestWidth);
			tmpWidth = 0;
			break;
		case ',':
			tmpWidth++;
			break;
		default:
			break;
		}
	}

	fseek(_File, initialFileCursorLocation, SEEK_SET);
	return highestWidth;
}

// A function to get how many number of doors there are in the boarding method
int GetNumberOfDoorsInBoardingMethod(FILE* _File) {
	long int initialFileCursorLocation = ftell(_File);
	fseek(_File, 0, SEEK_SET);

	int numberOfDoors = 0;
	char tempChar;
	while (!feof(_File))
	{
		tempChar = fgetc(_File);
		if (tempChar == 'D')
		{
			numberOfDoors++;
		}
	}

	fseek(_File, initialFileCursorLocation, SEEK_SET);
	return numberOfDoors;
}

// A function to get the longest digit in file
int GetLongestNumberInFile(FILE* _File)
{
	long int initialFileCursorLocation = ftell(_File);
	fseek(_File, 0, SEEK_SET);

	int LongestNumber = 0;
	int DigitCharCounter = 0;
	char tempChar;
	while (!feof(_File))
	{
		tempChar = fgetc(_File);
		if (tempChar == ',')
		{
			if (DigitCharCounter > LongestNumber)
				LongestNumber = DigitCharCounter;
			DigitCharCounter = 0;
		}
		else
			DigitCharCounter++;
	}

	fseek(_File, initialFileCursorLocation, SEEK_SET);
	return (LongestNumber + 1);
}

// A function to set static values for map
void SetMapStaticValues(FILE* _MapFile, Map* _PlaneMap)
{
	FreeMap(_PlaneMap);
	_PlaneMap->Width = GetSeatsPerLine(_MapFile);
	_PlaneMap->Height = GetNumberOfLinesInFile(_MapFile);
	_PlaneMap->DoorCount = GetNumberOfDoorsInBoardingMethod(_MapFile);
	_PlaneMap->LongestNumber = GetLongestNumberInFile(_MapFile);
}

// A function to set values for map from file
void SetMapValuesFromFile(FILE* _MapFile, Map* _PlaneMap)
{
	_PlaneMap->NumberOfSeats = 0;

	int bufferLength = GetNumberOfCharsForLongestLineInFile(_MapFile);
	char* buffer = calloc(bufferLength, sizeof(char));
	if (buffer == NULL)
	{
		fprintf(stderr, "Failed to allocate %d bytes for filebuffer while reading map\n", bufferLength);
		return;
	}

	long int initialFileCursorLocation = ftell(_MapFile);
	fseek(_MapFile, 0, SEEK_SET);

	int x = -1, y = 0, doorIndex = 0;
	while (fgets(buffer, bufferLength, _MapFile) != NULL)  // One iteration per line, until NULL is returned at EOF
	{
		char field[32];
		int bufferOffset = 0;

		while (sscanf_s(buffer + bufferOffset, "%[^,]", field, 32) == 1) // One iteration per field of data in a line
		{
			field[31] = '\0';
			bufferOffset += (int)strlen(field) + 1; // +Comma
			if (bufferOffset > (int)strlen(buffer))
				break;

			x++;
			switch (field[0])  // Special characters will typically just be the first character of the field.
			{
			case '|':
				MapLocationSetValue(_PlaneMap, x, y, BoardingGroup_Walkway);
				break;
			case 'D':
				MapSetDoorValue(_PlaneMap, x, y, &doorIndex);
				break;
			case '-':
				MapLocationSetValue(_PlaneMap, x, y, BoardingGroup_Padding);
				break;
			case '\r': break;
			case '\0': break;
			case '\n':
				y++;
				x = -1;
				break;

			default:
				MapSetSeatValue(_PlaneMap, x, y, field);
				break;
			}
		}
		y++;
		x = -1;
	}

	free(buffer);

	fseek(_MapFile, initialFileCursorLocation, SEEK_SET);
}

// A function to set value for door
void MapSetDoorValue(Map* _PlaneMap, int _x, int _y, int* _doorIndex)
{
	MapLocationSetValue(_PlaneMap, _x, _y, BoardingGroup_Door);
	_PlaneMap->Doors[*_doorIndex].X = _x;
	_PlaneMap->Doors[*_doorIndex].Y = _y;
	(*_doorIndex)++;
}

// A function to set value for a seat in map
void MapSetSeatValue(Map* _PlaneMap, int _x, int _y, char _field[])
{
	int _tmpInt = 0;
	if (sscanf_s(_field, "%d", &_tmpInt) == 1)
	{
		MapLocationSetValue(_PlaneMap, _x, _y, _tmpInt);
		GetMapLocation(_PlaneMap, _x, _y)->IsTaken = false;
		_PlaneMap->NumberOfSeats++;
	}
	else
	{
		fprintf(stderr, "Unknown value '%s'\n", _field);
		MapLocationSetValue(_PlaneMap, _x, _y, BoardingGroup_Undefined);
	}
}