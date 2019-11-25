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

void PrintField(Person _PersonList[MaxPersons], const char _BaseFieldData[MaxRows][MaxSeatsPrRow]);
bool IsAnyOnPoint(Person* _IsPersonArray[MaxRows][MaxSeatsPrRow], Person *_Person);
Point PredictedPoint(Point _CurrentPoint, Point _TargetPoint);
Point PredictedPointInvX(Point _CurrentPos, XAxis _TargX, XAxis _DoorX);
void SendRowBack(Person* _IsPersonArray[MaxRows][MaxSeatsPrRow], Person *_Person);
void PersonMovement(int _Index, Person _PersonList[MaxPersons], Person* _IsPersonArray[MaxRows][MaxSeatsPrRow]);
void GenerateProgressBar(char _PBar[ProgressBarSteps], int _Run, int _MxRun);
void SaveRunDataToFile(FILE* _fp, Person _PersonList[MaxPersons], int _RunTime, int _RunNumber);
void RunSim(Person _PersonList[MaxPersons], Person *_IsPersonArray[MaxRows][MaxSeatsPrRow], bool _UpdateVisuals);
int BackupWaitSteps(int _TargetSeat, int _InnerBlockingSeat);

#endif