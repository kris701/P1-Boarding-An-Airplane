#pragma once
#include "GlobalVariableTypes.h"

typedef enum BoardingMethods {
	BMRandom,
	BMWilma,
	BMFrontToBack,
	BMBackToFront,
	BMSteffenModified,
	BMJanModified,
	BMSteffenPerfect
} BoardingMethods;

typedef enum Classes {
	CFirst,
	CPeasent
} Classes;

typedef struct BoardingMethod {
	BoardingMethods BoardingMethod;
	char* FileName;
} BoardingMethod;

typedef struct PlaneInformation {
	int FirstClassRows;
	int PeasentClassRows;
	int SeatsPerRow;
	int FirstClassSeatsPerRow;
	int BoardingGroupRowCount;
	int UpperDoor, LowerDoor;
} PlaneInformation;

typedef struct BoardingInfo {
	PlaneInformation PlaneInfo;
	BoardingMethod BoardingMethod;
	int CurrentBoardingGroup;
	int PeasentClassBoardingGroupOffset;
	int BoardingGroupRowCounter;
	int SeatCounter;
	Classes CurrentClass;
} BoardingInfo;

