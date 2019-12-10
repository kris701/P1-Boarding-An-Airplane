#include "Main.h"

int main()
{
	bool UpdateGraphics = false;
	int RunsToDo = 0;
	Person* PassengerList;
	Person*** PassengerLocationMatrix;
	Map PlaneMap = { 0 };
	BasicSimulationRules BasicRules = { 0 };

	bool RunAll = true;
	int BoardingMethodCount = 1;

	srand(time(NULL));

	if (!ReadBasicRulesConfigFile(&BasicRules, "config.ini"))
		return 0;

	

	if (BasicRules.DoAllRuns)
		BoardingMethodCount = BasicRules.MultipleMapsLength;

	// 2 function used to get user input
	UpdateGraphics = GetYNInput("Update Graphics?");
	RunsToDo = GetIntInput("How many runs?", 0, MaxRuns);

	for (int i = 0; i < BoardingMethodCount; i++) {
		if (BasicRules.DoAllRuns)
			strncpy_s(BasicRules.BoardingMethodFile, 128, BasicRules.MultipleMaps[i], strlen(BasicRules.MultipleMaps[i]));

		char* BoardingMethodString = calloc(64, sizeof(char));
		strncpy_s(BoardingMethodString, 64, BasicRules.BoardingMethodFile + 16, strlen(BasicRules.BoardingMethodFile) - 20);
		strcpy_s(BasicRules.BoardingMethodName, 128, BoardingMethodString);

		PlaneMap.DoorCount = 0;
		PlaneMap.Doors = 0;
		PlaneMap.Height = 0;
		PlaneMap.Locations = 0;
		PlaneMap.LongestDigit = 0;
		PlaneMap.NumberOfSeats = 0;
		PlaneMap.Width = 0;
		if (!ReadMapFromFile(&PlaneMap, BasicRules))
			return 0;

		AllocatePassengerList(&PassengerList, PlaneMap);
		AllocatePassengerLocationMatrix(&PassengerLocationMatrix, PlaneMap);

		RunAllSimulationsAndSaveToOutput(PassengerList, PassengerLocationMatrix, PlaneMap, BasicRules, UpdateGraphics, RunsToDo);

		CleanupAllocations(PassengerList, PassengerLocationMatrix);
		
	}

	return 0;
}

// A function that flushes input
void FlushInput()
{
	while (getchar() != '\n');
}

// A bool function that checks for y/n input
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

// A function that gets an interger input
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
void RunAllSimulationsAndSaveToOutput(Person* _PassengerList, Person*** _PassengerLocationMatrix, Map _PlaneMap, BasicSimulationRules _BasicRules, bool _UpdateGraphics, int _RunsToDo)
{
	FILE* OutputFile;
	int TotalStepsTaken = 0;
	clock_t TotalWatchStart, TotalWatchEnd;
	
	char BoardingMethodFileString[128] = "Outputs/";
	
	strcat_s(BoardingMethodFileString, 128, _BasicRules.BoardingMethodName);
	strncat_s(BoardingMethodFileString, 64, ".csv", 4);
	DoOpenFile(&OutputFile, BoardingMethodFileString, "w+");

	if (!FileExists(OutputFile))
		return;

	fprintf(OutputFile,"%s,%s,%s,%s", "boarding-method", "cross-delay", "seat-interference-delay", "assign-to-nearest-door");
	for (int i = 0; i < _BasicRules.LuggageGenerationValuesLength; i++) {
		fprintf(OutputFile, "%s-%d,%s-%d,", "luggage-count", i, "luggage-count-possibility", i);
	}
	for (int i = 0; i < _BasicRules.WalkingspeedGenerationValuesLength; i++) {
		fprintf(OutputFile, "%s-%d,%s-%d,", "walk-speed", i, "walk-speed-possibility", i);
	}
	
	fprintf(OutputFile, "Iterations;\n");

	TotalWatchStart = clock();

	for (int i = 0; i < _RunsToDo; i++)
	{
		TotalStepsTaken += RunOneSimulationAndGetSteps(_PassengerList, _PassengerLocationMatrix, _PlaneMap, _BasicRules, _UpdateGraphics, OutputFile);
	}

	TotalWatchEnd = clock();

	fclose(OutputFile);

	printf("\nFinished - %-20s Took %4d ms and an avr of %4d iterations pr run\n", _BasicRules.BoardingMethodName, (int)((((double)TotalWatchEnd - (double)TotalWatchStart) / CLOCKS_PER_SEC) * 1000), (TotalStepsTaken / _RunsToDo));
}

// A function that runs the simulation once and gets amount of stepts taken
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

	//Fix
	fprintf(_OutputFile, "%s,%d,%d,%d", _BasicRules.BoardingMethodFile,_BasicRules.CrossDelay, _BasicRules.SeatInterferenceDelay, _BasicRules.AssignToNearestDoor);
	for (int i = 0; i < _BasicRules.LuggageGenerationValuesLength; i++) {
		fprintf(_OutputFile, "%d,%d,", _BasicRules.LuggageGenerationValues[i].Value, _BasicRules.LuggageGenerationValues[i].Possibility);
	}
	for (int i = 0; i < _BasicRules.WalkingspeedGenerationValuesLength; i++) {
		fprintf(_OutputFile, "%d,%d,", _BasicRules.WalkingspeedGenerationValues[i].Value, _BasicRules.WalkingspeedGenerationValues[i].Possibility);
	}
	
	fprintf(_OutputFile, "%d;\n", StepsTaken);

	return StepsTaken;
}

void CleanupAllocations(Person* _PassengerList, Person*** _PassengerLocationMatrix)
{
	free(_PassengerList);
	free(_PassengerLocationMatrix);
}