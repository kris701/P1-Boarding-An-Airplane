#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "GlobalVariableTypes.h"

#include "BackToFront.h"
#include "FrontToBack.h"
#include "JanModified.h"
#include "SteffenModified.h"
#include "SteffenPerfect.h"
#include "Random.h"
#include "Wilma.h"

FILE* SetBoardingMethod(BoardingInfo* _BI, FILE* _FP, int _Index);
void GetSeatNumber(BoardingInfo* _BI, int _SeatIndex, char _SeatString[]);
int GetPeasentOffset(BoardingInfo _BI);