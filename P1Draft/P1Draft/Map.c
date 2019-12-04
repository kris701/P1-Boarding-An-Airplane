#include "Map.h"

bool ReadMapFromFile(Map* _PlaneMap, FILE* _File) 
{
    FreeMap(_PlaneMap);
    _PlaneMap->Width  = GetSeatsPerLine(_File);
    _PlaneMap->Height = GetNumberOfLinesInFile(_File); 
	_PlaneMap->DoorCount = GetNumberOfDoorsInBoardingMethod(_File);

    if (AllocateMap(_PlaneMap) == false) 
	{
        return false;
    }

    int bufferLength = GetNumberOfCharsForLongestLineInFile(_File);
    char* buffer = calloc(bufferLength, sizeof(char));
    if (buffer == NULL) 
	{
        fprintf(stderr, "Failed to allocate %d bytes for filebuffer while reading map\n", bufferLength);
        return false;
    }

    long int initialFileCursorLocation = ftell(_File);
    fseek(_File, 0, SEEK_SET);

    int x=0, y=0, doorIndex=0;
    while (fgets(buffer, bufferLength, _File) != NULL)  // One iteration per line, until NULL is returned at EOF
	{
        char field[32];
		int bufferOffset = 0;
		int tmpInt;

        while (sscanf_s(buffer+bufferOffset, "%[^,]", field, 32) == 1) // One iteration per field of data in a line
		{
			field[31] = '\0';
			bufferOffset += strlen(field) + 1; // +Comma
			if (bufferOffset > (int)strlen(buffer)) 
				break;

            switch (field[0])  // Special characters will typically just be the first character of the field.
			{
                case '|': 
					MapLocationSetValue(_PlaneMap, x, y, BoardingGroup_Walkway);
					x++;
				break;
                case 'D':
					MapLocationSetValue(_PlaneMap, x, y, BoardingGroup_Door);
					_PlaneMap->Doors[doorIndex].X = x;
					_PlaneMap->Doors[doorIndex].Y = y;
					doorIndex++;
					x++;
				break;
                case '-':
					MapLocationSetValue(_PlaneMap, x, y, BoardingGroup_Padding);
					x++;
				break;
				case '\r': break;
				case '\0': break;
				case '\n':
					y++;
					x = 0;
				break;

                default:
                    if (sscanf_s(field, "%d", &tmpInt) == 1) 
					{
						MapLocationSetValue(_PlaneMap, x, y, tmpInt);
						MapLocationGet(_PlaneMap, x, y)->IsTaken = 0;
						_PlaneMap->NumberOfSeats++;
                        x++;
                    }
                    else 
					{
                        fprintf(stderr, "Unknown value '%s'\n", field);
						MapLocationSetValue(_PlaneMap, x, y, BoardingGroup_Undefined);
                    }
                break;
            }
        }
        y++;
		x = 0;
    }

    free(buffer);

    fseek(_File, initialFileCursorLocation, SEEK_SET);
    return true;
}

void MapLocationSetValue(Map* _PlaneMap, int _X, int _Y, int Value) 
{
	MapLocationGet(_PlaneMap, _X, _Y)->BoardingGroup = Value;
}

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
    int lines = 1; // Begins at line 1, if �ne \n is occured, then there are 2 lines
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

int GetNumberOfDoorsInBoardingMethod(FILE* _File) {
	long int initialFileCursorLocation = ftell(_File);
	fseek(_File, 0, SEEK_SET);

	int numberOfDoors = 0;
	char ch1 = fgetc(_File), ch2;
	while (!feof(_File)) {
		ch2 = fgetc(_File);
		if (ch1 == ',' && ch2 == 'D') {
			numberOfDoors++;
		}
		ch1 = ch2;
	}

	fseek(_File, initialFileCursorLocation, SEEK_SET);
	return numberOfDoors;
}