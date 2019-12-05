#pragma once

#include "GlobalVariableTypes.h"

void SetBackToFrontMethod(BoardingInfo* _BI);
int GetSeatForBackToFront(BoardingInfo* _BI, int _SeatIndex);
int GetPeasentOffsetForBackToFront(BoardingInfo _BI);