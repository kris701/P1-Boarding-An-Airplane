#include "Random.h"

int GetSeatForRandom(BoardingInfo *_BI) {
	return (_BI->CurrentClass == CFirst ? 1 : 2);
}