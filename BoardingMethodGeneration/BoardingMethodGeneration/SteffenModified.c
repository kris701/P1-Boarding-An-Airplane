#include "SteffenModified.h"

int GetSeatForSteffenModified(BoardingInfo* _BI, int _SeatIndex) {
	int rowCount = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassRows : _BI->PlaneInfo.PeasentClassRows);
	return  (_BI->PeasentClassBoardingGroupOffset +
		// Calculates the highest boarding group
		2 * rowCount -
		// Subtracts for each row already written
		((_BI->BoardingGroupRowCounter % 2 == 1) ? rowCount : rowCount / 2) -
		// Subtracts 1 for each two full rows already checked
		(_BI->BoardingGroupRowCounter / 4) -
		// Subtracts the total amount of full rows of the specific class from half of the plane
		((_BI->BoardingGroupRowCounter % 4 == ((rowCount % 2 == 0) ? 2 : 0)) ? rowCount : 0) +
		// Adds the total amount of full rows of the specific class to 1/4 of the plane rows
		(((_BI->BoardingGroupRowCounter % 4 == ((rowCount % 2 == 0) ? 1 : 3)) && (_BI->BoardingGroupRowCounter % 2 == 1)) ? rowCount : 0) +
		// Adds 1 to to 1/4 of the plane rows
		((rowCount % 2 == 1) ? (((_BI->BoardingGroupRowCounter % 4 == 1) && (_BI->BoardingGroupRowCounter % 2 == 1)) ? 1 : 0) : 0));
}

int GetPeasentOffsetForSteffenModified(BoardingInfo _BI) {
	return (2 * _BI.PlaneInfo.FirstClassRows);
}