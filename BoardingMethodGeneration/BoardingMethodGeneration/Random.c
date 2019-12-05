#include "Random.h"

void SetRandomMethod(BoardingInfo* _BI) {
	_BI->BoardingMethod.BoardingMethod = BMRandom;
	_BI->BoardingMethod.FileName = "Methods/random.txt";
}

int GetSeatForRandom(BoardingInfo *_BI, int _SeatIndex) {
	return (_BI->CurrentClass == CFirst ? 1 : 2);
}

int GetPeasentOffsetForRandom(BoardingInfo _BI) {
	return 1;
}