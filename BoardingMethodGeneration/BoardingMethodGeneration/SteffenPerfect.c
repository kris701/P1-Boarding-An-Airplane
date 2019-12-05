#include "SteffenPerfect.h"

void SetSteffenperfectMethod(BoardingInfo* _BI) {
	_BI->BoardingMethod.BoardingMethod = BMSteffenPerfect;
	_BI->BoardingMethod.FileName = "Methods/steffenperfect.txt";
}

int GetSeatForSteffenPerfect(BoardingInfo* _BI, int _SeatIndex) {
	int rowCount = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassRows : _BI->PlaneInfo.PeasentClassRows);
	int SeatsPerRow = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassSeatsPerRow : _BI->PlaneInfo.SeatsPerRow);
	return  (_BI->PeasentClassBoardingGroupOffset +
		// Sets the maximum seat number to all seats
		rowCount * SeatsPerRow -
		// Sets the maximum seat number for each column of seats
		(((SeatsPerRow / 2) - _SeatIndex) * rowCount * 2) -
		// Subtracts for each group of rows, as seen below:
		// n   n2   n3   n3   n2   n
		// n   n2   n3   n3   n2   n
		// n-1 n2-1 n3-1 n3-1 n2-1 n-1
		// n-1 n2-1 n3-1 n3-1 n2-1 n-1
		(_BI->BoardingGroupRowCounter / 4) -
		// Subtracts from half the plane layout
		(((_BI->BoardingGroupRowCounter + 1) % 2 == 1) * rowCount / 2) -
		// Subtracts the prior seats
		(((_BI->BoardingGroupRowCounter - _BI->BoardingGroupRowCounter % 2) % 4 == ((rowCount % 2 == 1) ? 0 : 2)) ? rowCount : 0) +
		// Adds 1 to every second row of half the plane
		((rowCount % 2 == 1) ? ((_BI->BoardingGroupRowCounter % 4 == 1) ? 1 : 0) : 0));
}

int GetPeasentOffsetForSteffenPerfect(BoardingInfo _BI) {
	return (_BI.PlaneInfo.FirstClassRows * _BI.PlaneInfo.FirstClassSeatsPerRow);
}