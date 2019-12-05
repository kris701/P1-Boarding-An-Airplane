#include "FrontToBack.h"

void SetFrontToBackMethod(BoardingInfo* _BI) {
	_BI->BoardingMethod.BoardingMethod = BMFrontToBack;
	_BI->BoardingMethod.FileName = "Methods/fronttoback.txt";
}

int GetSeatForFrontToBack(BoardingInfo* _BI, int _SeatIndex) {
	int rowCount = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassRows : _BI->PlaneInfo.PeasentClassRows);
	return  (_BI->PeasentClassBoardingGroupOffset +
		// Offset
		1 +
		// Gets the amount of full rows already written
		(_BI->BoardingGroupRowCounter + (_BI->BoardingGroupRowCounter % 2 == 1 ? 0 : 1))
		// Divides by the boarding group size
		/ (2 * _BI->PlaneInfo.BoardingGroupRowCount));
}

int GetPeasentOffsetForFrontToBack(BoardingInfo _BI) {
	return (_BI.PlaneInfo.FirstClassRows / _BI.PlaneInfo.BoardingGroupRowCount) + ((_BI.PlaneInfo.FirstClassRows % _BI.PlaneInfo.BoardingGroupRowCount == 0) ? 0 : 1);
}