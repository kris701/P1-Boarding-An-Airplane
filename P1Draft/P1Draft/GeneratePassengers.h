#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "StructsAndEnums.h"
#include "BasicCalls.h"
#include "MapDefinition.h"

void GeneratePassengers(int Count, Person _PersonList[MaxPersons], MethodIndex Index, MethodDefinition _MDef);
void GeneratePassenger(Person* Passenger);
int GenerateLuggage(const Person* Passenger);
int GetStartingDoorID(const Person* Passenger);
int GenerateWalkSpeed(const Person* Passenger);
bool AssignPassengerToAvailableSeat(Person _PassengerList[MaxPersons], int index);
Point Random_GetTargetLocation(Person _PersonList[MaxPersons], int Index);

