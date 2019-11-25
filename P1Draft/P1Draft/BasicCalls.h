#ifndef BASICCALLS_HEADER
#define BASICCALLS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "StructsAndEnums.h"

bool IsPointEqu(Point _A, Point _B);
Point SetPoint(int _X, int _Y);
int GetRandomNumberRanged(int _Min, int _Max);
bool IsNumberOdd(int _Value);

#endif