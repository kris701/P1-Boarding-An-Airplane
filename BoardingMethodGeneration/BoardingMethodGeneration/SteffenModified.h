#pragma once

#include "GlobalVariableTypes.h"

void SetSteffenModifiedMethod(BoardingInfo* _BI);
int GetSeatForSteffenModified(BoardingInfo* _BI, int _SeatIndex);
int GetPeasentOffsetForSteffenModified(BoardingInfo _BI);