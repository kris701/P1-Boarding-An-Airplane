#include "BackToFront.h"

void SetBackToFrontMethod(BoardingInfo* _BI) {
	_BI->BoardingMethod.BoardingMethod = BMBackToFront;
	_BI->BoardingMethod.FileName = "Methods/backtofront.txt";
}

int GetSeatForBackToFront(BoardingInfo* _BI, int _SeatIndex) {
	int rowCount = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassRows : _BI->PlaneInfo.PeasentClassRows);
	return  (_BI->PeasentClassBoardingGroupOffset +
		// Calculates the amount of total boarding groups
		(rowCount / _BI->PlaneInfo.BoardingGroupRowCount) + ((rowCount % _BI->PlaneInfo.BoardingGroupRowCount == 0) ? 0 : 1) -
		// Calculates the total amount of full rows alreadt written
		((_BI->BoardingGroupRowCounter + (_BI->BoardingGroupRowCounter % 2 == 1 ? 0 : 1)) /
			// Divides by the boarding group size
		(2 * _BI->PlaneInfo.BoardingGroupRowCount)));
}

int GetPeasentOffsetForBackToFront(BoardingInfo _BI) {
	return (_BI.PlaneInfo.FirstClassRows / _BI.PlaneInfo.BoardingGroupRowCount) + ((_BI.PlaneInfo.FirstClassRows % _BI.PlaneInfo.BoardingGroupRowCount == 0) ? 0 : 1);
}