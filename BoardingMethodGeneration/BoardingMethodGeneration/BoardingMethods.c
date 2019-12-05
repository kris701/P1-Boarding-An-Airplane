#include "BoardingMethods.h"

FILE* SetBoardingMethod(BoardingInfo* _BI, FILE* _FP, int _Index) {
	switch ((BoardingMethods)_Index) {
	case BMRandom:
		fopen_s(&_FP, "Methods/random.txt", "w");
		_BI->BoardingMethod = BMRandom;
		break;
	case BMWilma:
		fopen_s(&_FP, "Methods/wilma.txt", "w");
		_BI->BoardingMethod = BMWilma;
		break;
	case BMFrontToBack:
		fopen_s(&_FP, "Methods/fronttoback.txt", "w");
		_BI->BoardingMethod = BMFrontToBack;
		break;
	case BMBackToFront:
		fopen_s(&_FP, "Methods/backtofront.txt", "w");
		_BI->BoardingMethod = BMBackToFront;
		break;
	case BMSteffenModified:
		fopen_s(&_FP, "Methods/steffenmodified.txt", "w");
		_BI->BoardingMethod = BMSteffenModified;
		break;
	case BMJanModified:
		fopen_s(&_FP, "Methods/janmodified.txt", "w");
		_BI->BoardingMethod = BMJanModified;
		break;
	case BMSteffenPerfect:
		fopen_s(&_FP, "Methods/steffenperfect.txt", "w");
		_BI->BoardingMethod = BMSteffenPerfect;
		break;
	default:
		printf("Array out of index in boardingmethods\n");
		break;
	}

	return _FP;
}

void GetSeatNumber(BoardingInfo* _BI, int _SeatIndex, char _SeatString[]) {
	switch (_BI->BoardingMethod) {
	case BMRandom:
		// Either 1 for first class or 2 for peasent class
		sprintf_s(_SeatString, 10, "%d,", GetSeatForRandom(_BI));
		break;
	case BMWilma:
		// Adds one index per seat between the current seat and the window
		sprintf_s(_SeatString, 10, "%d,", GetSeatForWilma(_SeatIndex, _BI));
		break;
	case BMFrontToBack:
		sprintf_s(_SeatString, 10, "%d,", GetSeatForFrontToBack(_BI, _SeatIndex));
		break;
	case BMBackToFront:
		sprintf_s(_SeatString, 10, "%d,", GetSeatForBackToFront(_BI, _SeatIndex));
		break;
	case BMSteffenModified:
		sprintf_s(_SeatString, 10, "%d,", GetSeatForSteffenModified(_BI, _SeatIndex));
		break;
	case BMJanModified:
		sprintf_s(_SeatString, 10, "%d,", GetSeatForJanModified(_BI, _SeatIndex));
		break;
	case BMSteffenPerfect:
		sprintf_s(_SeatString, 10, "%d,", GetSeatForSteffenPerfect(_BI, _SeatIndex));
		break;
	default:
		sprintf_s(_SeatString, 10, "%d,", -1);
		break;
	}
}

int GetPeasentOffset(BoardingInfo _BI) {
	// This is the amount of seats in the first class
	// Thus offsetting peasent class by that amount
	switch (_BI.BoardingMethod) {
	case BMRandom:
		return 1;
		break;
	case BMWilma:
		return (_BI.PlaneInfo.SeatsPerRow / 4 + (_BI.PlaneInfo.SeatsPerRow / 2) % 2);
		break;
	case BMFrontToBack:
		return (_BI.PlaneInfo.FirstClassRows / _BI.PlaneInfo.BoardingGroupRowCount) + ((_BI.PlaneInfo.FirstClassRows % _BI.PlaneInfo.BoardingGroupRowCount == 0) ? 0 : 1);
		break;
	case BMBackToFront:
		return (_BI.PlaneInfo.FirstClassRows / _BI.PlaneInfo.BoardingGroupRowCount) + ((_BI.PlaneInfo.FirstClassRows % _BI.PlaneInfo.BoardingGroupRowCount == 0) ? 0 : 1);
		break;
	case BMSteffenModified:
		return (2 * _BI.PlaneInfo.FirstClassRows);
		break;
	case BMJanModified:
		return (_BI.PlaneInfo.FirstClassRows * _BI.PlaneInfo.FirstClassSeatsPerRow);
		break;
	case BMSteffenPerfect:
		return (_BI.PlaneInfo.FirstClassRows * _BI.PlaneInfo.FirstClassSeatsPerRow);
		break;
	default:
		return 0;
		break;
	}
}