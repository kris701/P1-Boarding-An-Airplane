#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

#include "GeneratePassengers.h"

#define MaxRuns 10000000

void PrintField(Person** _PassengerLocationMatrix[], Map _PlaneMap);
bool IsAnyOnPoint(Person** _PassengerLocationMatrix[], Person *_Person, Map _PlaneMap, BasicSimulationRules _BaseRules);
Point PredictedPoint(Point CurrentPoint, Point TargetPoint);
void SendRowBack(Person** _PassengerLocationMatrix[], Person *_Person, Map _PlaneMap, BasicSimulationRules _BaseRules);
void PassengerMovement(Person* _Passenger, Person** _PassengerLocationMatrix[], Map _PlaneMap, BasicSimulationRules _BaseRules);
void RunSim(Person _PassengerList[], Person** _PassengerLocationMatrix[], bool UpdateVisuals, int* _StepsTaken, Map _PlaneMap, BasicSimulationRules _BaseRules);
int BackupWaitSteps(int _TargetSeat, int _InnerBlockingSeat, int ExtraPenalty);
bool IsInDelayAction(Person* _Person, Map _PlaneMap);
BasicSimulationRules ReadBasicRulesConfigFile(BasicSimulationRules _BaseRules, const char* FileName);
int FindValueBetweenChars(char* SubStringBuffer[], char FromChar, char ToChar);