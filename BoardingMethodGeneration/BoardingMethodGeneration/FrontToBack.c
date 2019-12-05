#include "FrontToBack.h"

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