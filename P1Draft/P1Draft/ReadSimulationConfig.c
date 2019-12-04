#include "ReadSimulationConfig.h"

BasicSimulationRules ReadBasicRulesConfigFile(BasicSimulationRules _BaseRules, const char* FileName)
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
			return _BaseRules;
		}

		while (!feof(BSRFile))
		{
			fgets(Buffer, BufferLength, BSRFile);

			if (strstr(Buffer, "CrossDelay "))
			{
				SubStringBuffer = strchr(Buffer, '=') + 1;
				_BaseRules.CrossDelay = atoi(SubStringBuffer);
				continue;
			}
			if (strstr(Buffer, "ShuffleDelay "))
			{
				SubStringBuffer = strchr(Buffer, '=') + 1;
				_BaseRules.ShuffleDelay = atoi(SubStringBuffer);
				continue;
			}
			if (strstr(Buffer, "LuggageGen "))
			{
				SubStringBuffer = strchr(Buffer, '=') + 1;

				int ItemCount = GetItemCountInConfig(SubStringBuffer, BufferLength);

				_BaseRules.LuggageGenerationValues = calloc(ItemCount, sizeof(ValueStatistic));
				_BaseRules.LuggageGenerationValuesLength = ItemCount;

				for (int i = 0; i < ItemCount; i++)
				{
					_BaseRules.LuggageGenerationValues[i].Value = FindValueBetweenChars(&SubStringBuffer, '[', ',');
					_BaseRules.LuggageGenerationValues[i].Possibility = FindValueBetweenChars(&SubStringBuffer, ',', ']');
				}
				continue;
			}
			if (strstr(Buffer, "WalkspeedGen "))
			{
				SubStringBuffer = strchr(Buffer, '=') + 1;

				int ItemCount = GetItemCountInConfig(SubStringBuffer, BufferLength);

				_BaseRules.WalkingspeedGenerationValues = calloc(ItemCount, sizeof(ValueStatistic));
				_BaseRules.WalkingspeedGenerationValuesLength = ItemCount;

				for (int i = 0; i < ItemCount; i++)
				{
					_BaseRules.WalkingspeedGenerationValues[i].Value = FindValueBetweenChars(&SubStringBuffer, '[', ',');
					_BaseRules.WalkingspeedGenerationValues[i].Possibility = FindValueBetweenChars(&SubStringBuffer, ',', ']');
				}

				continue;
			}
		}

		free(Buffer);
	}
	else
	{
		_BaseRules.CrossDelay = 0;
		_BaseRules.ShuffleDelay = 0;
		_BaseRules.LuggageGenerationValuesLength = 0;
		_BaseRules.WalkingspeedGenerationValuesLength = 0;
	}

	return _BaseRules;
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