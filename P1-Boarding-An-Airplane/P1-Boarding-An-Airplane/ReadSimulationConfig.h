#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StructsAndEnums.h"
#include "BasicCalls.h"

bool ReadSimulationConfigFile(SimulationConfig* _Config, const char* FileName);
int FindIntBetweenChars(char* SubStringBuffer[], char FromChar, char ToChar);
int GetItemCountInConfig(char SubStringBuffer[], int _BufferLength);
void GetStringAfterIdentifier(int BufferSize, char* _Buffer, char* Target[]);
void RemoveIdentifier(char** _SubStringBuffer, int SubStringBufferLength, char* _Buffer);
bool GetBoolAfterIdentifier(int BufferSize, char* _Buffer);
int GetIntAfterIdentifier(int BufferSize, char* _Buffer);
void GetStringsOfItems(int SubStringBufferLength, char* _Buffer, char*** Target, int TargetSize, int* TargetLength);
void GetValueStastisticsOfItems(int SubStringBufferLength, char* _Buffer, ValueStatistic** Target, int* TargetLength);
void RemoveLastCharacter(char** _SubStringBuffer, int SubStringBufferLength, char* _Buffer);
