#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "StructsAndEnums.h"
#include "BasicCalls.h"

void GeneratePassengers(int Count, Person _PersonList[], Map _PlaneMap, BasicSimulationRules _BaseRules);
void InitialisePassenger(Person* Passenger, Map _PlaneMap, BasicSimulationRules _BaseRules);
int GenerateLuggage(BasicSimulationRules _BaseRules);
int GetNearestStartingDoorID(Person* Passenger, Map _PlaneMap);
int GenerateWalkSpeed(BasicSimulationRules _BaseRules);
bool AssignPassengersToAvailableSeat(int Count, Person _PassengerList[], Map _PlaneMap);
bool AssignSeatByBoardinggroup(int BoardingGroup, Person* _Passenger, Map _PlaneMap);
void AssignPassengerToNearestDoor(int Count, Person _PassengerList[], Map _PlaneMap);
void AssignPassengerToRandomDoor(int Count, Person _PassengerList[], Map _PlaneMap);
void ScramblePassengers(Person _PassengerList[], int ArrayLength);
void AssignPassengerToDoor(int Count, Person _PassengerList[], Map _PlaneMap, BasicSimulationRules _BaseRules);
void ResetPassengerListAndMap(int Count, Person* _PersonList[], Map* _PlaneMap, BasicSimulationRules _BaseRules);
