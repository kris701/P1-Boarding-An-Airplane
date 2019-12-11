#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StructsAndEnums.h"
#include "BasicCalls.h"

bool ReadBasicRulesConfigFile(BasicSimulationRules* _BasicRules, const char* FileName);
int FindIntBetweenChars(char* SubStringBuffer[], char FromChar, char ToChar);
int GetItemCountInConfig(char SubStringBuffer[], int _BufferLength);
void FindStrBetweenChars(char* SubStringBuffer[], char* Target[], int TargetLength, char FromChar, char ToChar);
void GetStringAfterIdentifier(int BufferSize, char* _Buffer, char* Target[]);
void RemoveIdentifier(char** _SubStringBuffer, int SubStringBufferLength, char* _Buffer);
bool GetBoolAfterIdentifier(int BufferSize, char* _Buffer);
int GetIntAfterIdentifier(int BufferSize, char* _Buffer);
