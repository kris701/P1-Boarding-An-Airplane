#include "Main.h"

int main()
{
	bool UpdateGraphics = false;
	int RunsToDo = 0;
	Map PlaneMap = { 0 };
	BasicSimulationRules BasicRules = { 0 };

	srand(time(NULL));

	if (!ReadBasicRulesConfigFile(&BasicRules, "config.ini"))
		return 0;

	UpdateGraphics = GetYNInput("Update Graphics?");
	RunsToDo = GetIntInput("How many runs?", 0, MaxRuns);

	if (BasicRules.DoAllRuns)
		RunMultipleSimulations(PlaneMap, &BasicRules, UpdateGraphics, RunsToDo);
	else
	{
		if (!ReadMapFromFile(&PlaneMap, BasicRules, BasicRules.BoardingMethodFile))
			return 1;

		RunAllSimulationsAndSaveToOutput(PlaneMap, BasicRules, UpdateGraphics, RunsToDo, BasicRules.BoardingMethodFile);
	}
	return 0;

}

void RunMultipleSimulations(Map _PlaneMap, BasicSimulationRules* _BasicRules, bool _UpdateGraphics, int _RunsToDo)
{
	for (int i = 0; i < _BasicRules->MultipleMapsLength; i++)
	{
		_BasicRules->BoardingMethodFile = _BasicRules->MultipleMaps[i];
		if (!ReadMapFromFile(&_PlaneMap, *_BasicRules, _BasicRules->MultipleMaps[i]))
			return;

		RunAllSimulationsAndSaveToOutput(_PlaneMap, *_BasicRules, _UpdateGraphics, _RunsToDo, _BasicRules->MultipleMaps[i]);
	}
}
// The getchar function eats up everything until it reaches \n
void FlushInput()
{
	while (getchar() != '\n');
}

// Checks if the input is 'y'
bool GetYNInput(const char* _Text)
{
	char Choice = ' ';
	while (Choice != 'y' && Choice != 'n')
	{
		printf("%s (y/n)\n", _Text);
		scanf_s(" %c", &Choice, 1);
	}
	return Choice == 'y';
}

// Asks the user to input amount of runs wanted
int GetIntInput(const char* _Text, int Min, int Max)
{
	int Value = Min - 1;
	while (Value <= Min || Value > Max)
	{
		printf("%s \n", _Text);
		scanf_s(" %d", &Value);

		FlushInput();
	}
	return Value;
}

void AllocatePassengerList(Person** _PassengerList, Map _PlaneMap)
{
	*_PassengerList = calloc(_PlaneMap.NumberOfSeats, sizeof(Person));
}

void AllocatePassengerLocationMatrix(Person**** _PassengerLocationMatrix, Map _PlaneMap)
{
	*_PassengerLocationMatrix = calloc(_PlaneMap.Height, sizeof(Person**));
	for (int i = 0; i < _PlaneMap.Height; i++) {
		(*_PassengerLocationMatrix)[i] = calloc(_PlaneMap.Width, sizeof(Person*));
	}
}

void ResetPassengerLocationMatrix(Person**** _PassengerLocationMatrix, Map _PlaneMap)
{
	for (int j = 0; j < _PlaneMap.Height; j++) {
		for (int l = 0; l < _PlaneMap.Width; l++) {
			memset(&(*_PassengerLocationMatrix)[j][l], 0, sizeof(Person*));
		}
	}
}

// A function that runs the simulation a given amount of times and saves data to a file
void RunAllSimulationsAndSaveToOutput(Map _PlaneMap, BasicSimulationRules _BasicRules, bool _UpdateGraphics, int _RunsToDo, char* InputDir)
{
	FILE* OutputFile;
	int TotalStepsTaken = 0;
	clock_t TotalWatchStart, TotalWatchEnd;
	Person* PassengerList;
	Person*** PassengerLocationMatrix;

	char* AccOutputDir = calloc(128, sizeof(char));

	ConvertInputDirToOutputDir(InputDir, &AccOutputDir, 128, "Output/", ".csv");

	DoOpenFile(&OutputFile, AccOutputDir, "w+");

	if (!FileExists(OutputFile)) {
		fprintf(stderr, "Output file missing\n");
		return;
	}

	WriteProbabilitiesToOutput(OutputFile, _BasicRules);

	fprintf(OutputFile, "Iterations\n");

	AllocatePassengerList(&PassengerList, _PlaneMap);
	AllocatePassengerLocationMatrix(&PassengerLocationMatrix, _PlaneMap);

	TotalWatchStart = clock();

	for (int i = 0; i < _RunsToDo; i++)
	{
		TotalStepsTaken += RunOneSimulationAndGetSteps(PassengerList, PassengerLocationMatrix, _PlaneMap, _BasicRules, _UpdateGraphics, OutputFile);
	}

	TotalWatchEnd = clock();

	fclose(OutputFile);

	CleanupAllocations(PassengerList, PassengerLocationMatrix);

	printf("\nFinished - %s Took %4d ms and an avr of %4d iterations pr run\n", InputDir, (int)((((double)TotalWatchEnd - (double)TotalWatchStart) / CLOCKS_PER_SEC) * 1000), (TotalStepsTaken / _RunsToDo));
}

int RunOneSimulationAndGetSteps(Person* _PassengerList, Person*** _PassengerLocationMatrix, Map _PlaneMap, BasicSimulationRules _BasicRules, bool _UpdateGraphics, FILE* _OutputFile)
{
	int StepsTaken = 0;

	ResetPassengerLocationMatrix(&_PassengerLocationMatrix, _PlaneMap);

	GeneratePassengers(_PlaneMap.NumberOfSeats, _PassengerList, _PlaneMap, _BasicRules);

	StepsTaken = RunSim(_PassengerList, _PassengerLocationMatrix, _UpdateGraphics, _PlaneMap, _BasicRules);

	if (_UpdateGraphics)
	{
		printf("All seated! Took: %d iterations\n", StepsTaken);

		Sleep(1000);
	}

	WriteBasicRulesToOutput(_OutputFile, _BasicRules);

	fprintf(_OutputFile, "%d\n", StepsTaken);

	return StepsTaken;
}

void CleanupAllocations(Person* _PassengerList, Person*** _PassengerLocationMatrix)
{
	free(_PassengerList);
	free(_PassengerLocationMatrix);
}

void ConvertInputDirToOutputDir(char* _InputDir, char** _OutputDir, int OutputLength, const char* Dir, const char* FileExtension)
{
	FindStrBetweenChars(&_InputDir, &(*_OutputDir), OutputLength, '/', '.');

	AppendToStartOfString(_OutputDir, OutputLength, Dir);

	strcat_s((*_OutputDir), OutputLength, FileExtension);
}

void AppendToStartOfString(char** _Str, int _StrLength, const char* AddToStr)
{
	char* tmp = _strdup((*_Str));

	strcpy_s((*_Str), _StrLength, AddToStr);
	strcat_s((*_Str), _StrLength, tmp);

	free(tmp);
}

void WriteProbabilitiesToOutput(FILE* _OutputFile, BasicSimulationRules _BasicRules)
{
	fprintf(_OutputFile, "%s,%s,%s,%s,", "boarding-method", "cross-delay", "seat-interference-delay", "assign-to-nearest-door");
	for (int i = 0; i < _BasicRules.LuggageGenerationValuesLength; i++) {
		fprintf(_OutputFile, "%s-%d,%s-%d,", "luggage-count", i, "luggage-count-possibility", i);
	}
	for (int i = 0; i < _BasicRules.WalkingspeedGenerationValuesLength; i++) {
		fprintf(_OutputFile, "%s-%d,%s-%d,", "walk-speed", i, "walk-speed-possibility", i);
	}
}

void WriteBasicRulesToOutput(FILE* _OutputFile, BasicSimulationRules _BasicRules)
{
	fprintf(_OutputFile, "%s,%d,%d,%d,", _BasicRules.BoardingMethodFile, _BasicRules.CrossDelay, _BasicRules.SeatInterferenceDelay, _BasicRules.AssignToNearestDoor);
	for (int i = 0; i < _BasicRules.LuggageGenerationValuesLength; i++) {
		fprintf(_OutputFile, "%d,%d,", _BasicRules.LuggageGenerationValues[i].Value, _BasicRules.LuggageGenerationValues[i].Possibility);
	}
	for (int i = 0; i < _BasicRules.WalkingspeedGenerationValuesLength; i++) {
		fprintf(_OutputFile, "%d,%d,", _BasicRules.WalkingspeedGenerationValues[i].Value, _BasicRules.WalkingspeedGenerationValues[i].Possibility);
	}
}

