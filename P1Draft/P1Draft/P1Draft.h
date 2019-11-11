#ifndef P1DRAFT_HEADER
#define P1DRAFT_HEADER

#include "GeneratePassengers.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

#define MaxRuns 1000000
#define ProgressBarSteps 10

void PrintField(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], const char _BaseFieldData[MaxRows][MaxSeatsPrRow]);
bool IsAnyOnPoint(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], struct Point TargetPoint, int ExcludeIndex, int RowIndex);
struct Point PredictedPoint(struct Point CurrentPoint, struct Point TargetPoint);
struct Point PredictedPointInvX(struct Point _CurrentPos, enum XAxis TargX, enum XAxis DoorX);
bool IsSeatedFurtherIn(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], int ExcludeIndex, int RowIndex);
void SendRowBack(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], int ExcludeIndex, int RowIndex);
void PersonMovement(int Index, int RowIndex, struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1]);
void GenerateProgressBar(char PBar[ProgressBarSteps], int Run, int MxRun);
void SaveRunDataToFile(FILE* fp, struct Person PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], int RunTime, int RunNumber);
void RunSim(struct Person PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], bool UpdateVisuals);
void MoveToNewArray(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], int* RowIndex, int* Index, int _NewRowIndex);

#endif