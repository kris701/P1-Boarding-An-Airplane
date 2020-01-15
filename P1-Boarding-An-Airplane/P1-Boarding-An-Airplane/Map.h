#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BasicCalls.h"
#include "StructsAndEnums.h"

bool ReadMapFromFile(Map* _PlaneMap, SimulationConfig _Config, const char* OpenFile);
bool AllocateMap(Map* _PlaneMap);
void FreeMap(Map* _PlaneMap);
int GetSeatsPerLine(FILE* _File);
void MapLocationSetType(Map* _PlaneMap, int _X, int _Y, int Value);
int GetNumberOfDoorsInBoardingMethod(FILE* _File);
int GetLongestNumberInFile(FILE* _File);
void SetMapStaticValues(FILE* _MapFile, Map* _PlaneMap);
void SetMapValuesFromFile(FILE* _MapFile, Map* _PlaneMap);
void MapAddDoor(Map* _PlaneMap, int _x, int _y, int* _doorIndex);
void MapSetSeatBoardingGroup(Map* _PlaneMap, int _x, int _y, char _field[]);
