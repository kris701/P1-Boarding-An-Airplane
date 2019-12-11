#include "ReadSimulationConfig.h"

// A function to read basic rules from config file
bool ReadBasicRulesConfigFile(BasicSimulationRules* _BasicRules, const char* FileName)
{
	FILE* BSRFile;
	fopen_s(&BSRFile, FileName, "r");

	if (FileExists(BSRFile))
	{
		int BufferLength = GetNumberOfCharsForLongestLineInFile(BSRFile);
		char* Buffer = calloc(BufferLength, sizeof(char));
		char* SubStringBuffer = calloc(BufferLength, sizeof(char));
		(*_BasicRules).BoardingMethodFile = calloc(BufferLength, sizeof(char));

		if (Buffer == NULL || BufferLength == 0)
		{
			printf("Error reading config file!");
			return false;
		}

		while (!feof(BSRFile))
		{
			fgets(Buffer, BufferLength, BSRFile);

			if (strstr(Buffer, "MapName"))
			{
				GetStringAfterIdentifier(BufferLength, Buffer, &((*_BasicRules).BoardingMethodFile));
				continue;
			}
			if (strstr(Buffer, "MultipleMaps"))
			{
				GetStringsOfItems(BufferLength, Buffer, &((*_BasicRules).MultipleMaps), 128, &((*_BasicRules).MultipleMapsLength));
				continue;
			}
			if (strstr(Buffer, "CrossDelay"))
			{
				(*_BasicRules).CrossDelay = GetIntAfterIdentifier(BufferLength, Buffer);
				continue;
			}
			if (strstr(Buffer, "SeatInterferenceDelay"))
			{
				(*_BasicRules).SeatInterferenceDelay = GetIntAfterIdentifier(BufferLength, Buffer);
				continue;
			}
			if (strstr(Buffer, "LuggageGen"))
			{
				GetValueStastisticsOfItems(BufferLength, Buffer, &((*_BasicRules).LuggageGenerationValues), &((*_BasicRules).LuggageGenerationValuesLength));
				continue;
			}
			if (strstr(Buffer, "WalkspeedGen"))
			{
				GetValueStastisticsOfItems(BufferLength, Buffer, &((*_BasicRules).WalkingspeedGenerationValues), &((*_BasicRules).WalkingspeedGenerationValuesLength));
				continue;
			}
			if (strstr(Buffer, "AssignToNearestDoor"))
			{
				(*_BasicRules).AssignToNearestDoor = GetBoolAfterIdentifier(BufferLength, Buffer);
				continue;
			}
			if (strstr(Buffer, "DoAllRuns"))
			{
				(*_BasicRules).DoAllRuns = GetBoolAfterIdentifier(BufferLength, Buffer);
				continue;
			}
		}

		free(Buffer);
	}
	else
	{
		printf("Error reading config file!");
		return false;
	}

	return true;
}

// A function to find int between chars
int FindIntBetweenChars(char* SubStringBuffer[], char FromChar, char ToChar)
{
	char* NumVal = calloc(32, sizeof(char));

	FindStrBetweenChars(SubStringBuffer, &NumVal, 32, FromChar, ToChar);

	return atoi(NumVal);
}

// A function to get item count from config file
int GetItemCountInConfig(char SubStringBuffer[], int _BufferLength)
{
	int ItemCount = 0;
	for (int i = 0; i < _BufferLength; i++)
	{
		if (SubStringBuffer[i] == '[')
			ItemCount++;
		else if (SubStringBuffer[i] == '\n')
			break;
	}
	return ItemCount;
}

void GetStringAfterIdentifier(int BufferSize, char* _Buffer, char* Target[])
{
	char* SubStringBuffer = calloc(BufferSize, sizeof(char));

	RemoveIdentifier(&SubStringBuffer, BufferSize, _Buffer);
	RemoveLastCharacter(&SubStringBuffer, BufferSize, _Buffer);

	strcpy_s(*Target, BufferSize, SubStringBuffer);
}

int GetIntAfterIdentifier(int BufferSize, char* _Buffer)
{
	char* SubStringBuffer = calloc(BufferSize, sizeof(char));

	RemoveIdentifier(&SubStringBuffer, BufferSize, _Buffer);
	RemoveLastCharacter(&SubStringBuffer, BufferSize, _Buffer);
	
	return atoi(SubStringBuffer);
}

bool GetBoolAfterIdentifier(int BufferSize, char* _Buffer)
{
	char* SubStringBuffer = calloc(BufferSize, sizeof(char));

	RemoveIdentifier(&SubStringBuffer, BufferSize, _Buffer);
	RemoveLastCharacter(&SubStringBuffer, BufferSize, _Buffer);

	if (strstr(SubStringBuffer, "true"))
		return true;

	return false;
}

void RemoveIdentifier(char** _SubStringBuffer, int SubStringBufferLength, char* _Buffer)
{
	*_SubStringBuffer = strchr(_Buffer, '=') + 1;
}

void RemoveLastCharacter(char** _SubStringBuffer, int SubStringBufferLength, char* _Buffer)
{
	for (int i = 0; i < SubStringBufferLength; i++)
	{
		if ((*_SubStringBuffer)[i] == '\n')
		{
			(*_SubStringBuffer)[i] = '\0';
			break;
		}
	}
}

void GetStringsOfItems(int SubStringBufferLength, char* _Buffer, char*** Target, int TargetSize, int* TargetLength)
{
	char* SubStringBuffer = calloc(SubStringBufferLength, sizeof(char));

	RemoveIdentifier(&SubStringBuffer, SubStringBufferLength, _Buffer);

	int ItemCount = GetItemCountInConfig(SubStringBuffer, SubStringBufferLength);

	RemoveLastCharacter(&SubStringBuffer, SubStringBufferLength, _Buffer);

	*Target = calloc(ItemCount, sizeof(char**));
	*TargetLength = ItemCount;

	for (int i = 0; i < ItemCount; i++)
	{
		(*Target)[i] = calloc(TargetSize, sizeof(char));
		FindStrBetweenChars(&SubStringBuffer, &((*Target)[i]), TargetSize, '[', ']');
	}
}

void GetValueStastisticsOfItems(int SubStringBufferLength, char* _Buffer, ValueStatistic** Target, int* TargetLength)
{
	char* SubStringBuffer = calloc(SubStringBufferLength, sizeof(char));

	RemoveIdentifier(&SubStringBuffer, SubStringBufferLength, _Buffer);

	int ItemCount = GetItemCountInConfig(SubStringBuffer, SubStringBufferLength);

	RemoveLastCharacter(&SubStringBuffer, SubStringBufferLength, _Buffer);

	*Target = calloc(ItemCount, sizeof(ValueStatistic));
	*TargetLength = ItemCount;

	for (int i = 0; i < ItemCount; i++)
	{
		(*Target)[i].Value = FindIntBetweenChars(&SubStringBuffer, '[', ',');
		(*Target)[i].Possibility = FindIntBetweenChars(&SubStringBuffer, ',', ']');
	}
}
