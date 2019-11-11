#ifndef BASICCALLS_HEADER
#define BASICCALLS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "StructsAndEnums.h"

bool IsPointEqu(struct Point A, struct Point B);
struct Point SetPoint(int X, int Y);
int GetRandomNumberRanged(int _Min, int _Max);
bool IsNumberOdd(int _Value);

#endif