#include "Main.h"

int main()
{
	bool UpdateGraphics = false;
	int RunsToDo = 0;
	SimulationConfig Config = { 0 };

	srand(time(NULL));

	if (!ReadSimulationConfigFile(&Config, "config.ini"))
	{
		fprintf(stderr, "Error, config.ini not found!\n");
		return 0;
	}

	UpdateGraphics = GetBoolInput("Update Graphics?");
	RunsToDo = GetIntInput("How many runs?", 0, INT_MAX);

	for (int i = 0; i < Config.MultipleMapsLength; i++)
	{
		RunSimulationsAndSaveToOutput(Config, UpdateGraphics, RunsToDo, Config.MultipleMaps[i]);
	}

	// Debug
	system("pause");

	return 0;

}

// The getchar function eats up everything until it reaches \n
void FlushInput()
{
	while (getchar() != '\n');
}

// Checks if the input is 'y'
bool GetBoolInput(const char* _Text)
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
void RunSimulationsAndSaveToOutput(SimulationConfig _Config, bool _UpdateGraphics, int _RunsToDo, char* InputDir)
{
	FILE* OutputFile;
	int TotalStepsTaken = 0;
	clock_t TotalWatchStart, TotalWatchEnd;
	Person* PassengerList;
	Person*** PassengerLocationMatrix;
	Map PlaneMap = { 0 };

	if (!ReadMapFromFile(&PlaneMap, _Config, InputDir))
	{
		fprintf(stderr, "Error, input method could not be opened!\n");
		return;
	}

	char* OutputDir = calloc(128, sizeof(char));

	_mkdir("./Output"); // Creates a directory. If it already exists. Will do nothing.
	GenerateOutputDirString(InputDir, &OutputDir, 128, "Output/", ".csv");

	if (!DoOpenFile(&OutputFile, OutputDir, "w+"))
	{
		fprintf(stderr, "Output file missing\n");
		return;
	}

	WriteHeaderToOutput(OutputFile, _Config);

	AllocatePassengerList(&PassengerList, PlaneMap);
	AllocatePassengerLocationMatrix(&PassengerLocationMatrix, PlaneMap);

	TotalWatchStart = clock();

	for (int i = 0; i < _RunsToDo; i++)
	{
		int Value = RunOneSimulationAndGetSteps(PassengerList, PassengerLocationMatrix, PlaneMap, _Config, _UpdateGraphics, OutputFile, InputDir);
		TotalStepsTaken += Value;

		WriteConfigValuesToOutput(OutputFile, _Config, InputDir);

		fprintf(OutputFile, "%d\n", Value);
	}

	TotalWatchEnd = clock();

	fclose(OutputFile);

	free(OutputDir);
	CleanupAllocations(&PassengerList, &PassengerLocationMatrix, &PlaneMap);

	printf("\nFinished - %s Took %4d ms and an avr of %4d iterations pr run\n", InputDir, TimeDifference(TotalWatchStart, TotalWatchEnd), (TotalStepsTaken / _RunsToDo));
}

int RunOneSimulationAndGetSteps(Person* _PassengerList, Person*** _PassengerLocationMatrix, Map _PlaneMap, SimulationConfig _Config, bool _UpdateGraphics, FILE* _OutputFile, char* MethodFile)
{
	int StepsTaken = 0;

	ResetPassengerLocationMatrix(&_PassengerLocationMatrix, _PlaneMap);

	GeneratePassengers(_PlaneMap.NumberOfSeats, _PassengerList, _PlaneMap, _Config);

	StepsTaken = RunSim(_PassengerList, _PassengerLocationMatrix, _UpdateGraphics, _PlaneMap, _Config);

	if (_UpdateGraphics)
	{
		printf("All seated! Took: %d iterations\n", StepsTaken);

		Sleep(1000);
	}

	return StepsTaken;
}

void CleanupAllocations(Person** _PassengerList, Person**** _PassengerLocationMatrix, Map* _PlaneMap)
{
	for (int i = 0; i < (*_PlaneMap).Height; i++)
	{
		free((*_PassengerLocationMatrix)[i]);
	}

	free(*_PassengerLocationMatrix);
	free(*_PassengerList);

	FreeMap(_PlaneMap);
}

void GenerateOutputDirString(char* _InputDir, char** _OutputDir, int OutputLength, const char* Dir, const char* FileExtension)
{
	FindStrBetweenDelimiters(&_InputDir, &(*_OutputDir), OutputLength, '/', '.');

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

void WriteHeaderToOutput(FILE* _OutputFile, SimulationConfig _Config)
{
	fprintf(_OutputFile, "%s,%s,%s,%s,", "boarding-method", "cross-delay", "seat-interference-delay", "assign-to-nearest-door");
	for (int i = 0; i < _Config.LuggageGenerationValuesLength; i++) {
		fprintf(_OutputFile, "%s-%d,%s-%d,", "luggage-count", i, "luggage-count-possibility", i);
	}
	for (int i = 0; i < _Config.WalkingspeedGenerationValuesLength; i++) {
		fprintf(_OutputFile, "%s-%d,%s-%d,", "walk-speed", i, "walk-speed-possibility", i);
	}
	fprintf(_OutputFile, "Iterations\n");
}

void WriteConfigValuesToOutput(FILE* _OutputFile, SimulationConfig _Config, const char* MethodFile)
{
	fprintf(_OutputFile, "%s,%d,%d,%d,", MethodFile, _Config.CrossDelay, _Config.SeatInterferenceDelay, _Config.AssignToNearestDoor);
	for (int i = 0; i < _Config.LuggageGenerationValuesLength; i++) {
		fprintf(_OutputFile, "%d,%d,", _Config.LuggageGenerationValues[i].Value, _Config.LuggageGenerationValues[i].Possibility);
	}
	for (int i = 0; i < _Config.WalkingspeedGenerationValuesLength; i++) {
		fprintf(_OutputFile, "%d,%d,", _Config.WalkingspeedGenerationValues[i].Value, _Config.WalkingspeedGenerationValues[i].Possibility);
	}
}

