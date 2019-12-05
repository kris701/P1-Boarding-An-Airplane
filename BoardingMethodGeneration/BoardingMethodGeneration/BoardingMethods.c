#include "BoardingMethods.h"

FILE* SetBoardingMethod(BoardingInfo* _BI, FILE* _FP, int _Index) {
	switch ((BoardingMethods)_Index) {
	case BMRandom:			SetRandomMethod(_BI);			break;
	case BMWilma:			SetWilmaMethod(_BI);			break;
	case BMFrontToBack:		SetFrontToBackMethod(_BI);		break;
	case BMBackToFront:		SetBackToFrontMethod(_BI);		break;
	case BMSteffenModified:	SetSteffenModifiedMethod(_BI);	break;
	case BMJanModified:		SetJanModifiedMethod(_BI);		break;
	case BMSteffenPerfect:	SetSteffenperfectMethod(_BI);	break;
	default:
		printf("Array out of index in boardingmethods\n");
		break;
	}
	fopen_s(&_FP, _BI->BoardingMethod.FileName, "w");

	return _FP;
}

void GetSeatNumber(BoardingInfo* _BI, int _SeatIndex, char _SeatString[]) {
	switch (_BI->BoardingMethod.BoardingMethod) {
	case BMRandom:
		// Either 1 for first class or 2 for peasent class
		sprintf_s(_SeatString, 10, "%d,", GetSeatForRandom(_BI, _SeatIndex));
		break;
	case BMWilma:
		// Adds one index per seat between the current seat and the window
		sprintf_s(_SeatString, 10, "%d,", GetSeatForWilma(_BI, _SeatIndex));
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
	switch (_BI.BoardingMethod.BoardingMethod) {
	case BMRandom:			return GetPeasentOffsetForRandom(_BI);
	case BMWilma:			return GetPeasentOffsetForWilma(_BI);
	case BMFrontToBack:		return GetPeasentOffsetForFrontToBack(_BI);
	case BMBackToFront:		return GetPeasentOffsetForBackToFront(_BI);
	case BMSteffenModified:	return GetPeasentOffsetForSteffenModified(_BI);
	case BMJanModified:		return GetPeasentOffsetForJanModified(_BI);
	case BMSteffenPerfect:	return GetPeasentOffsetForSteffenPerfect(_BI);
	default:				return 0;
	}
}