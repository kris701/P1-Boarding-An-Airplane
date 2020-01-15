#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>

#include "Map.h"
#include "GeneratePassengers.h"
#include "ReadSimulationConfig.h"
#include "Simulation.h"

void FlushInput();
bool GetBoolInput(const char* _Text);
int GetIntInput(const char* _Text, int Min, int Max);
void AllocatePassengerList(Person** _PassengerList, Map _PlaneMap);
void AllocatePassengerLocationMatrix(Person**** _PassengerLocationMatrix, Map _PlaneMap);
void ResetPassengerLocationMatrix(Person**** _PassengerLocationMatrix, Map _PlaneMap);
int RunOneSimulationAndGetSteps(Person* _PassengerList, Person*** _PassengerLocationMatrix, Map _PlaneMap, SimulationConfig _Config, bool _UpdateGraphics, FILE* _OutputFile, const char* MethodFile);
void RunSimulationsAndSaveToOutput(SimulationConfig _Config, bool _UpdateGraphics, int _RunsToDo, char* InputDir);
void CleanupAllocations(Person** _PassengerList, Person**** _PassengerLocationMatrix, Map* _PlaneMap);
void AppendToStartOfString(char** _Str, int _StrLength, const char* AddToStr);
void WriteHeaderToOutput(FILE* _OutputFile, SimulationConfig _Config);
void WriteConfigValuesToOutput(FILE* _OutputFile, SimulationConfig _Config, const char* MethodFile);
void GenerateOutputDirString(char* _InputDir, char** _OutputDir, int OutputLength, const char* Dir, const char* FileExtension);
