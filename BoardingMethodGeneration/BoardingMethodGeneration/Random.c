#include "Random.h"

int GetSeatForRandom(BoardingInfo *_BI) {
	return (_BI->CurrentClass == CFirst ? 1 : 2);
}

int GetPeasentOffsetForRandom(BoardingInfo _BI) {
	return 1;
}