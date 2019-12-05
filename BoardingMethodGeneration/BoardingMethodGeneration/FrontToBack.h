#pragma once

#include "GlobalVariableTypes.h"

void SetFrontToBackMethod(BoardingInfo* _BI);
int GetSeatForFrontToBack(BoardingInfo* _BI, int _SeatIndex);
int GetPeasentOffsetForFrontToBack(BoardingInfo _BI);