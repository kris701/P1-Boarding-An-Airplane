#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StructsAndEnums.h"
#include "BasicCalls.h"

bool ReadBasicRulesConfigFile(BasicSimulationRules* _BasicRules, const char* FileName);
int FindValueBetweenChars(char* SubStringBuffer[], char FromChar, char ToChar);
int GetItemCountInConfig(char SubStringBuffer[], int _BufferLength);