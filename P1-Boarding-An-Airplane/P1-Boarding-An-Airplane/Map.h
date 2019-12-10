#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BasicCalls.h"
#include "StructsAndEnums.h"

bool ReadMapFromFile(Map* _PlaneMap, BasicSimulationRules _BasicRules);
bool AllocateMap(Map* _PlaneMap);
void FreeMap(Map* _PlaneMap);
int GetSeatsPerLine(FILE* _File);
void MapLocationSetValue(Map* _PlaneMap, int _X, int _Y, int Value);
int GetNumberOfDoorsInBoardingMethod(FILE* _File);
int GetLongestDigitInFile(FILE* _File);
void SetMapStaticValues(FILE* _MapFile, Map* _PlaneMap);
void SetMapValuesFromFile(FILE* _MapFile, Map* _PlaneMap);
void MapSetDoorValue(Map* _PlaneMap, int _x, int _y, int* _doorIndex);
void MapSetSeatValue(Map* _PlaneMap, int _x, int _y, char _field[]);