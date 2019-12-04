#include "Main.h"

int main()
{
	char UpdateGraphics = ' ';
	char SaveToFile = ' ';
	int RunsToDo = 0;
	int RunTime = 0;
	int AvrStepsTaken = 0;
	int StepsTaken = 0;

	Person* PassengerList;
	Person*** PassengerLocationMatrix;
	clock_t TotalWatchStart, TotalWatchEnd, WatchStart, WatchEnd;
	Map PlaneMap = { 0 };
	BasicSimulationRules BaseRules = { 0 };

	BaseRules = ReadBasicRulesConfigFile(BaseRules, "config.ini");

	srand(time(NULL));

	FILE* PlaneMapFile;
	fopen_s(&PlaneMapFile, BaseRules.BoardingMethodFile, "r");

	if (PlaneMapFile != NULL)
	{
		printf("Using boarding method file: %s\n", BaseRules.BoardingMethodFile);
		ReadMapFromFile(&PlaneMap, PlaneMapFile);

		PassengerList = calloc(PlaneMap.NumberOfSeats, sizeof(Person));

		PassengerLocationMatrix = calloc(PlaneMap.Height, sizeof(Person**));
		for (int i = 0; i < PlaneMap.Height; i++) {
			PassengerLocationMatrix[i] = calloc(PlaneMap.Width, sizeof(Person*));
		}

		while (UpdateGraphics != 'y' && UpdateGraphics != 'n')
		{
			printf("Update graphics? (y/n)\n");
			scanf_s(" %c", &UpdateGraphics, 1);
		}
		while (SaveToFile != 'y' && SaveToFile != 'n')
		{
			printf("Save data to file? (y/n)\n");
			scanf_s(" %c", &SaveToFile, 1);
		}
		while (RunsToDo <= 0 || RunsToDo > MaxRuns)
		{
			printf("How many runs?: \n");
			scanf_s(" %d", &RunsToDo);

			while (getchar() != '\n');
		}

		FILE* OutputFile;

		if (SaveToFile == 'y')
			fopen_s(&OutputFile, "output.csv", "w+");
		else
			OutputFile = 1;

		if (OutputFile != NULL)
		{
			if (SaveToFile == 'y')
				fprintf(OutputFile, "Iterations;\n");

			TotalWatchStart = clock();

			for (int i = 0; i < RunsToDo; i++)
			{
				RunTime = 0;

				for (int i = 0; i < PlaneMap.Height; i++) {
					for (int j = 0; j < PlaneMap.Width; j++) {
						memset(&PassengerLocationMatrix[i][j], NULL, sizeof(Person*));
					}
				}

				GeneratePassengers(PlaneMap.NumberOfSeats, PassengerList, PlaneMap, BaseRules);

				WatchStart = clock();

				RunSim(PassengerList, PassengerLocationMatrix, UpdateGraphics == 'y', &StepsTaken, PlaneMap, BaseRules);

				WatchEnd = clock();

				RunTime = (int)((double)WatchEnd - (double)WatchStart);

				if (UpdateGraphics == 'y')
				{
					printf("All seated! Took: %d ticks and %d iterations\n", RunTime, StepsTaken);

					Sleep(1000);
				}

				if (SaveToFile == 'y')
					fprintf(OutputFile, "%d;\n", StepsTaken);

				AvrStepsTaken += StepsTaken;
				StepsTaken = 0;
			}

			TotalWatchEnd = clock();

			if (SaveToFile == 'y')
				fclose(OutputFile);

			printf("\nFinished! Took %d ms and an avr of %d iterations pr run\n", (int)((((double)TotalWatchEnd - (double)TotalWatchStart) / CLOCKS_PER_SEC) * 1000), (AvrStepsTaken / RunsToDo));
			if (SaveToFile == 'y')
				printf("Using boarding method file: %s\n", BaseRules.BoardingMethodFile);
		}
		else
			printf("Error making output file");

		free(PassengerList);
		free(PassengerLocationMatrix);
	}
	else
		printf("Error reading map file");

	return 0;
}
