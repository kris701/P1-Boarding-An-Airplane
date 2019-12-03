#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "StructsAndEnums.h"

enum BoardingGroup {
	BoardingGroup_Padding = -2,
	BoardingGroup_Walkway = -1,
	BoardingGroup_Door    = 0
};

typedef struct _location {
    Point Point;
    int BoardingGroup;
	bool IsTaken;
} Location;

typedef struct _map {
    Location** Locations;
    int Width;
    int Height;
    Point* Doors;
	int DoorCount;
	int NumberOfSeats;
} Map;

bool ReadMapFromFile(Map* map, FILE* file);
bool _AllocateMap(Map* map);
void _FreeMap(Map* map);
int GetSeatsPerLine(FILE* file);
int GetNumberOfCharsForLongestLineInFile(FILE* file);
int GetNumberOfLinesInFile(FILE* file);
void MapLocationSetValue(Map* map, int x, int y, int value);
Location* MapLocationGet(Map* map, int x, int y);