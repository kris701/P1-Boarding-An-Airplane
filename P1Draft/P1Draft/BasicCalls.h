#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "StructsAndEnums.h"

bool IsPointEqual(Point _A, Point _B);
Point SetPoint(int _X, int _Y);
int GetRandomNumberRanged(int _Min, int _Max);
Location* MapLocationGet(Map* map, int x, int y);
int GetNumberOfCharsForLongestLineInFile(FILE* _File);
int GetNumberOfLinesInFile(FILE* _File);