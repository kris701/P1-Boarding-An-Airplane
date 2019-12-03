#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "StructsAndEnums.h"
#include "BasicCalls.h"
#include "MapDefinition.h"
#include "Map.h"

void GeneratePassengers(int Count, Person _PersonList[MaxPersons], Map map);
void GeneratePassenger(Person* Passenger);
int GenerateLuggage(const Person* Passenger);
int GetStartingDoorID(const Person* Passenger);
int GenerateWalkSpeed(const Person* Passenger);
bool AssignPassengersToAvailableSeat(int Count, Person _PassengerList[MaxPersons], Map map);
bool AssignSeatByBoardinggroup(int boardingGroup, Person* passenger, Map map);
Point Random_GetTargetLocation(Person _PersonList[MaxPersons], int Index);

