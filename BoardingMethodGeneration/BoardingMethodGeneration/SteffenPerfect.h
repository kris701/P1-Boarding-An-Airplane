#pragma once

#include "GlobalVariableTypes.h"

void SetSteffenperfectMethod(BoardingInfo* _BI);
int GetSeatForSteffenPerfect(BoardingInfo* _BI, int _SeatIndex);
int GetPeasentOffsetForSteffenPerfect(BoardingInfo _BI);