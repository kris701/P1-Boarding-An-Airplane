#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>

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
int RunOneSimulationAndGetSteps(Person* _PassengerList, Person*** _PassengerLocationMatrix, Map _PlaneMap, BasicSimulationRules _BasicRules, bool _UpdateGraphics, FILE* _OutputFile, const char* MethodFile);
void RunAllSimulationsAndSaveToOutput(BasicSimulationRules _BasicRules, bool _UpdateGraphics, int _RunsToDo, char* InputDir);
void CleanupAllocations(Person** _PassengerList, Person**** _PassengerLocationMatrix, Map* _PlaneMap);
void AppendToStartOfString(char** _Str, int _StrLength, const char* AddToStr);
void WriteProbabilitiesToOutput(FILE* _OutputFile, BasicSimulationRules _BasicRules);
void WriteBasicRulesToOutput(FILE* _OutputFile, BasicSimulationRules _BasicRules, const char* MethodFile);
void RunMultipleSimulations(BasicSimulationRules* _BasicRules, bool _UpdateGraphics, int _RunsToDo);
void ConvertInputDirToOutputDir(char* _InputDir, char** _OutputDir, int OutputLength, const char* Dir, const char* FileExtension);
