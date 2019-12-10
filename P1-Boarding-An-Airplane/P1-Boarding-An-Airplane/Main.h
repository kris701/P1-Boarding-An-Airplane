#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "Map.h"
#include "GeneratePassengers.h"
#include "ReadSimulationConfig.h"
#include "Simulation.h"

#define MaxRuns 10000000

void FlushInput();
bool GetYNInput(const char* _Text);
int GetIntInput(const char* _Text, int Min, int Max);
void AllocatePassengerList(Person** _PassengerList, Map _PlaneMap);
void AllocatePassengerLocationMatrix(Person**** _PassengerLocationMatrix, Map _PlaneMap);
void ResetPassengerLocationMatrix(Person**** _PassengerLocationMatrix, Map _PlaneMap);
int RunOneSimulationAndGetSteps(Person* _PassengerList, Person*** _PassengerLocationMatrix, Map _PlaneMap, BasicSimulationRules _BasicRules, bool _UpdateGraphics, FILE* _OutputFile);
void RunAllSimulationsAndSaveToOutput(Person* _PassengerList, Person*** _PassengerLocationMatrix, Map _PlaneMap, BasicSimulationRules _BasicRules, bool _UpdateGraphics, int _RunsToDo);
void CleanupAllocations(Person* _PassengerList, Person*** _PassengerLocationMatrix);
