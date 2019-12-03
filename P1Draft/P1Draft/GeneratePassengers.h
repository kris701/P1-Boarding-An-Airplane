#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "StructsAndEnums.h"
#include "BasicCalls.h"
#include "Map.h"

void GeneratePassengers(int Count, Person _PersonList[], Map _PlaneMap);
void GeneratePassenger(Person* Passenger, Map _PlaneMap);
int GenerateLuggage(const Person* Passenger);
int GetStartingDoorID(const Person* Passenger, Map _PlaneMap);
int GenerateWalkSpeed(const Person* Passenger);
bool AssignPassengersToAvailableSeat(int Count, Person _PassengerList[], Map _PlaneMap);
bool AssignSeatByBoardinggroup(int BoardingGroup, Person* _Passenger, Map _PlaneMap);

