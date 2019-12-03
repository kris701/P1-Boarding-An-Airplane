#ifndef MAPDEFINITION_HEADER
#define MAPDEFINITION_HEADER

#include "StructsAndEnums.h"

#define MaxPersons 188
#define MaxSeatsPrRow 7
#define MaxRows 28
// a, b, c, d, e, f, g, h, i...
#define TotalDoors 2
#define SeatChar '.'
#define WalkWayChar '|'
#define DoorChar 'D'
#define EmptyChar ' '
#define MultiPersonChar 'M'
#define FirsClassRow { SeatChar, EmptyChar, SeatChar, WalkWayChar, SeatChar, EmptyChar, SeatChar }
#define EconomyRow { SeatChar, SeatChar, SeatChar, WalkWayChar, SeatChar, SeatChar, SeatChar }
#define DoorRow { EmptyChar, EmptyChar, EmptyChar, DoorChar, EmptyChar, EmptyChar, EmptyChar }

static const char BaseFieldData[MaxRows][MaxSeatsPrRow] = {
		DoorRow,
		FirsClassRow,
		FirsClassRow,
		FirsClassRow,
		FirsClassRow,
		FirsClassRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		EconomyRow,
		DoorRow
};

static const Point Doors[2] = {
	{Col_e,25},
	{Col_e,0}
};

static const BaseSimulationRules BSR = { 0, 5 };

#endif