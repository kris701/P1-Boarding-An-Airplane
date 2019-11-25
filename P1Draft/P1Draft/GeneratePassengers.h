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

void GeneratePassengers(int _Count, Person _PersonList[MaxPersons], MethodIndex _Index, MethodDefinition _MDef);
void Method_Random(int _Count, Person _PersonList[MaxPersons], MethodDefinition _MDef);
int GetValueFromFunction(int _Min, int _Max, FunctionDefinition _FDef);
Point Random_GetTargetLocation(Person _PersonList[MaxPersons], int _Index);

#endif