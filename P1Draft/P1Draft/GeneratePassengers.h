#ifndef GENERATEPASSENGERS_HEADER
#define GENERATEPASSENGERS_HEADER

#include "StructsAndEnums.h"
#include "BasicCalls.h"
#include "MapDefinition.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

void GeneratePassengers(int Count, struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], enum MethodIndex Index, struct MethodDefinition _MDef);
void Method_Random(int Count, struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], struct MethodDefinition _MDef);
int GetValueFromFunction(int Min, int Max, struct FunctionDefinition _FDef);
struct Point Random_GetTargetLocation(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int Index);

#endif