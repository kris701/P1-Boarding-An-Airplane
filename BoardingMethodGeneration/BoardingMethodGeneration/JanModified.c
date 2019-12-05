#include "JanModified.h"

// This was made by accident
int GetSeatForJanModified(BoardingInfo* _BI, int _SeatIndex) {
	int rowCount = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassRows : _BI->PlaneInfo.PeasentClassRows);
	int SeatsPerRow = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassSeatsPerRow : _BI->PlaneInfo.SeatsPerRow);
	return  (_BI->PeasentClassBoardingGroupOffset +
		// Sets the highest boarding group
		rowCount * SeatsPerRow -
		// Subtracts all seats already written
		_BI->BoardingGroupRowCounter * (SeatsPerRow / 2) -
		// Subtracts 1 for each seat between this and the window for the specific row
		((SeatsPerRow / 2) - _SeatIndex));
}

int GetPeasentOffsetForJanModified(BoardingInfo _BI) {
	return (_BI.PlaneInfo.FirstClassRows * _BI.PlaneInfo.FirstClassSeatsPerRow);
}