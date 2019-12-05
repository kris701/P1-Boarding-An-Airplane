#include "Wilma.h"

int GetSeatForWilma(int _SeatIndex, BoardingInfo *_BI) {
	return _SeatIndex + _BI->PeasentClassBoardingGroupOffset;
}