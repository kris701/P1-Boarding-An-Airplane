#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

#include "StructsAndEnums.h"
#include "BasicCalls.h"

int RunSim(Person _PassengerList[], Person** _PassengerLocationMatrix[], bool ShouldUpdateVisuals, Map _PlaneMap, SimulationConfig _BaseRules);
void UpdateVisuals(Person** _PassengerLocationMatrix[], Map _PlaneMap, int* ShowRPCCount, int* RPSCount, clock_t* OneSecWatchStart, clock_t* OneSecWatchEnd);
void PrintField(Person** _PassengerLocationMatrix[], Map _PlaneMap);
bool AttemptMove(Person** _PassengerLocationMatrix[], Person* _Person, Map _PlaneMap, SimulationConfig _BaseRules);
Point CalculatePassengersNextPosition(Point CurrentPoint, Point TargetPoint);
void SendRowBack(Person** _PassengerLocationMatrix[], Person* _Person, Map _PlaneMap, SimulationConfig _BaseRules);
void PerformPassengerMove(Person* _Passenger, Person** _PassengerLocationMatrix[], Map _PlaneMap, SimulationConfig _BaseRules);
int BackupWaitSteps(int _TargetSeat, int _InnerBlockingSeat, int ExtraPenalty);
bool ReduceDelayAction(Person* _Person, Map _PlaneMap);
bool CanPassengerSit(Person _Person);
bool MoveAllPassengersAndCheckIfSeated(Person _PassengerList[], Person** _PassengerLocationMatrix[], Map _PlaneMap, SimulationConfig _BaseRules);
void SetPassengerPosition(Person* _Passenger, Person** _PassengerLocationMatrix[], Map _PlaneMap, Point _TargetPoint);
void AttemptToMovePassenger(Person* _Passenger, Person** _PassengerLocationMatrix[], Map _PlaneMap, SimulationConfig _BaseRules);
void PerformPassengersNextMove(Person* _Passenger);
void CheckAndAttemptCrossing(Person* _Person, Person* _OtherPerson, Person** _PassengerLocationMatrix[], Map _PlaneMap, SimulationConfig _BaseRules);
bool CheckAndAttemptSeatInterference(Person* _Person, Person* _OtherPerson, Person** _PassengerLocationMatrix[], Map _PlaneMap, SimulationConfig _BaseRules);
void SendPassengerBack(Person* _Passenger, int _DistanceToTargetSeat, int _InnerMostSeat, int _AdditionalDelay);
bool IsAnyOnPoint(Person** _PassengerLocationMatrix[], int X, int Y);
bool IsInFrontOfSeatRow(Person* _Person, Map _PlaneMap);
