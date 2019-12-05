#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "BoardingMethods.h"
#include "GlobalVariableTypes.h"


int OpenConfigFile(BoardingInfo* _BI, FILE* _FP);
void ReadConfigFile(BoardingInfo* _BI, FILE* _FP);

BoardingInfo ResetBI(BoardingInfo _BI);
void CreateDoorRow(BoardingInfo* _BI, FILE* _FP);
int CreateFirstClassRows(BoardingInfo* _BI, FILE* _FP);
void CreatePeasentClassRows(BoardingInfo* _BI, FILE* _FP);

