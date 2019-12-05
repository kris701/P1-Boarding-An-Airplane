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
	int (*GetSeat)(BoardingInfo*, int);
	switch (_BI->BoardingMethod.BoardingMethod) {
	case BMRandom:			GetSeat = GetSeatForRandom;				break;
	case BMWilma:			GetSeat = GetSeatForWilma;				break;
	case BMFrontToBack:		GetSeat = GetSeatForFrontToBack;		break;
	case BMBackToFront:		GetSeat = GetSeatForBackToFront;		break;
	case BMSteffenModified:	GetSeat = GetSeatForSteffenModified;	break;
	case BMJanModified:		GetSeat = GetSeatForJanModified;		break;
	case BMSteffenPerfect:	GetSeat = GetSeatForSteffenPerfect;		break;
	default:
		printf("Setting seat for non existing boarding method: %d %s\n", _BI->BoardingMethod.BoardingMethod, _BI->BoardingMethod.FileName);
		return;
	}
	sprintf_s(_SeatString, 10, "%d,", GetSeat(_BI, _SeatIndex));
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