#include "ReadSimulationConfig.h"

bool ReadBasicRulesConfigFile(BasicSimulationRules* _BasicRules, const char* FileName)
{
	FILE* BSRFile;
	fopen_s(&BSRFile, FileName, "r");

	if (BSRFile != NULL)
	{
		int BufferLength = GetNumberOfCharsForLongestLineInFile(BSRFile);
		char* Buffer = calloc(BufferLength, sizeof(char));
		char* SubStringBuffer = calloc(BufferLength, sizeof(char));

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
				SubStringBuffer = strchr(Buffer, '=') + 1;
				SubStringBuffer[strlen(SubStringBuffer) - 1] = 0;
				strcpy_s((*_BasicRules).BoardingMethodFile, 128, SubStringBuffer);
				continue;
			}
			if (strstr(Buffer, "CrossDelay"))
			{
				SubStringBuffer = strchr(Buffer, '=') + 1;
				(*_BasicRules).CrossDelay = atoi(SubStringBuffer);
				continue;
			}
			if (strstr(Buffer, "SeatInterferenceDelay"))
			{
				SubStringBuffer = strchr(Buffer, '=') + 1;
				(*_BasicRules).SeatInterferenceDelay = atoi(SubStringBuffer);
				continue;
			}
			if (strstr(Buffer, "LuggageGen"))
			{
				SubStringBuffer = strchr(Buffer, '=') + 1;

				int ItemCount = GetItemCountInConfig(SubStringBuffer, BufferLength);

				(*_BasicRules).LuggageGenerationValues = calloc(ItemCount, sizeof(ValueStatistic));
				(*_BasicRules).LuggageGenerationValuesLength = ItemCount;

				for (int i = 0; i < ItemCount; i++)
				{
					(*_BasicRules).LuggageGenerationValues[i].Value = FindValueBetweenChars(&SubStringBuffer, '[', ',');
					(*_BasicRules).LuggageGenerationValues[i].Possibility = FindValueBetweenChars(&SubStringBuffer, ',', ']');
				}
				continue;
			}
			if (strstr(Buffer, "WalkspeedGen"))
			{
				SubStringBuffer = strchr(Buffer, '=') + 1;

				int ItemCount = GetItemCountInConfig(SubStringBuffer, BufferLength);

				(*_BasicRules).WalkingspeedGenerationValues = calloc(ItemCount, sizeof(ValueStatistic));
				(*_BasicRules).WalkingspeedGenerationValuesLength = ItemCount;

				for (int i = 0; i < ItemCount; i++)
				{
					(*_BasicRules).WalkingspeedGenerationValues[i].Value = FindValueBetweenChars(&SubStringBuffer, '[', ',');
					(*_BasicRules).WalkingspeedGenerationValues[i].Possibility = FindValueBetweenChars(&SubStringBuffer, ',', ']');
				}

				continue;
			}
			if (strstr(Buffer, "AssignToNearestDoor"))
			{
				SubStringBuffer = strchr(Buffer, '=') + 1;
				if (strstr(SubStringBuffer, "true"))
					(*_BasicRules).AssignToNearestDoor = true;
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

int FindValueBetweenChars(char* SubStringBuffer[], char FromChar, char ToChar)
{
	*SubStringBuffer = strchr(*SubStringBuffer, FromChar) + 1;
	int IndexOffset = 0;
	char NumVal[100] = { 0 };
	for (int j = 0; j < 100; j++)
	{
		if ((*SubStringBuffer)[j] != ']')
		{
			NumVal[IndexOffset] = (*SubStringBuffer)[j];
			IndexOffset++;
		}
		else
			break;
	}
	*SubStringBuffer = strchr(*SubStringBuffer, ToChar);

	return atoi(NumVal);
}

int GetItemCountInConfig(char SubStringBuffer[], int _BufferLength)
{
	int ItemCount = 0;
	for (int i = 0; i < _BufferLength; i++)
	{
		if (SubStringBuffer[i] == '[')
			ItemCount++;
	}
	return ItemCount;
}