#include "Wilma.h"

void SetWilmaMethod(BoardingInfo* _BI) {
	_BI->BoardingMethod.BoardingMethod = BMWilma;
	_BI->BoardingMethod.FileName = "Methods/wilma.txt";
}



int GetSeatForWilma(BoardingInfo *_BI, int _SeatIndex) {
	return _SeatIndex + _BI->PeasentClassBoardingGroupOffset;
}

int GetPeasentOffsetForWilma(BoardingInfo _BI) {
	return  (_BI.PlaneInfo.SeatsPerRow / 4 + (_BI.PlaneInfo.SeatsPerRow / 2) % 2);
}