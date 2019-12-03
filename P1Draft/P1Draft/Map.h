#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "StructsAndEnums.h"

enum BoardingGroup {
	BoardingGroup_Door = -1,
	BoardingGroup_Walkway = 0
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
} Map;

bool ReadMapFromFile(Map* map, FILE* file);
bool AllocateMapPoints(Map* map);
void FreeMapPoints(Map* map);
int GetSeatsPerLine(FILE* file);
int GetNumberOfCharsForLongestLineInFile(FILE* file);
int GetNumberOfLinesInFile(FILE* file);
void MapLocationSetValue(Map* map, int x, int y, int value);
Location* MapLocationGet(Map* map, int x, int y);