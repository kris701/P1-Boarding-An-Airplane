#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

typedef struct PlaneInformation {
	int FirstClassRows;
	int PeasentClassRows;
	int SeatsPerRow;
	int FirstClassSeatsPerRow;
	int BoardingGroupRowCount;
} PlaneInformation;

typedef struct BoardingInfo {
	PlaneInformation PlaneInfo;
	BoardingMethods BoardingMethod;
	int CurrentBoardingGroup;
	int PeasentClassBoardingGroupOffset;
	int BoardingGroupRowCounter;
	int SeatCounter;
	Classes CurrentClass;
} BoardingInfo;

int OpenConfigFile(BoardingInfo* _BI, FILE* _FP);
void ReadConfigFile(BoardingInfo* _BI, FILE* _FP);
FILE* SetBoardingMethod(BoardingInfo* _BI, FILE* _FP, int _Index);
BoardingInfo ResetBI(BoardingInfo _BI);
int CreateFirstClassRows(BoardingInfo* _BI, FILE* _FP);
void CreatePeasentClassRows(BoardingInfo* _BI, FILE* _FP);
void GetSeatNumber(BoardingInfo* _BI, int _SeatIndex, char _SeatString[]);
int GetPeasentOffset(BoardingInfo _BI);
int GetSeatForFrontToBack(BoardingInfo* _BI, int _SeatIndex);
int GetSeatForBackToFront(BoardingInfo* _BI, int _SeatIndex);
int GetSeatForSteffenModified(BoardingInfo* _BI, int _SeatIndex);
int GetSeatForJanModified(BoardingInfo* _BI, int _SeatIndex);
int GetSeatForSteffenPerfect(BoardingInfo* _BI, int _SeatIndex);