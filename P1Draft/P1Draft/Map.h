#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "StructsAndEnums.h"

typedef struct _seat {
    Point Point;
    int BoardingGroup;
} Seat;

typedef struct _map {
    Seat** Seats;
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
