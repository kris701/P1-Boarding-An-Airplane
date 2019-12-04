#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StructsAndEnums.h"
#include "BasicCalls.h"

BasicSimulationRules ReadBasicRulesConfigFile(BasicSimulationRules _BaseRules, const char* FileName);
int FindValueBetweenChars(char* SubStringBuffer[], char FromChar, char ToChar);
int GetItemCountInConfig(char SubStringBuffer[], int _BufferLength);