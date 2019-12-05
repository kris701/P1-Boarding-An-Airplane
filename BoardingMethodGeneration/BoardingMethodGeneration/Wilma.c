#include "Wilma.h"

int GetSeatForWilma(int _SeatIndex, BoardingInfo *_BI) {
	return _SeatIndex + _BI->PeasentClassBoardingGroupOffset;
}

int GetPeasentOffsetForWilma(BoardingInfo _BI) {
	return  (_BI.PlaneInfo.SeatsPerRow / 4 + (_BI.PlaneInfo.SeatsPerRow / 2) % 2);
}