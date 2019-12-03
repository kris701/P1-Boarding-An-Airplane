#ifndef P1DRAFT_HEADER
#define P1DRAFT_HEADER

#include "GeneratePassengers.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

#define MaxRuns 10000000

void PrintField(Person** _PassengerLocationMatrix[], Map _PlaneMap);
bool IsAnyOnPoint(Person** _PassengerLocationMatrix[], Person *_Person);
Point PredictedPoint(Point CurrentPoint, Point TargetPoint);
void SendRowBack(Person** _PassengerLocationMatrix[], Person *_Person);
void PassengerMovement(Person* _Passenger, Person** _PassengerLocationMatrix[]);
void RunSim(Person _PassengerList[], Person** _PassengerLocationMatrix[], bool UpdateVisuals, int* _StepsTaken, Map _PlaneMap);
int BackupWaitSteps(int _TargetSeat, int _InnerBlockingSeat, int ExtraPenalty);
bool IsInDelayAction(Person* _Person);

#endif