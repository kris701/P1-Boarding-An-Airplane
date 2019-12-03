#ifndef P1DRAFT_HEADER
#define P1DRAFT_HEADER

#include "GeneratePassengers.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

#define MaxRuns 10000000
#define ProgressBarSteps 10

void PrintField(Person* _PassengerLocationMatrix[MaxRows][MaxSeatsPrRow], const char _BaseFieldData[MaxRows][MaxSeatsPrRow]);
bool IsAnyOnPoint(Person* _PassengerLocationMatrix[MaxRows][MaxSeatsPrRow], Person *_Person);
Point PredictedPoint(Point CurrentPoint, Point TargetPoint);
Point PredictedPointInvX(Point CurrentPos, XAxis TargX, XAxis DoorX);
void SendRowBack(Person* _PassengerLocationMatrix[MaxRows][MaxSeatsPrRow], Person *_Person);
void PassengerMovement(Person* _Passenger, Person* _PassengerLocationMatrix[MaxRows][MaxSeatsPrRow]);
void SaveRunDataToFile(FILE* _fp, Person _PassengerList[MaxPersons], int RunTime, int RunNumber);
void RunSim(Person _PassengerList[MaxPersons], Person *_PassengerLocationMatrix[MaxRows][MaxSeatsPrRow], bool UpdateVisuals, int* _AvrStepsTaken);
int BackupWaitSteps(int _TargetSeat, int _InnerBlockingSeat, int ExtraPenalty);
bool IsInDelayAction(Person* _Person);

#endif