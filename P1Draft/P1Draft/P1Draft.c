#include "P1Draft.h"

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
	char BoardingMethodFile[64] = "steffenperfect.txt";
	fopen_s(&PlaneMapFile, BoardingMethodFile, "r");

	if (PlaneMapFile != NULL)
	{
		printf("Using boarding method file: %s\n", BoardingMethodFile);
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
				printf("Using boarding method file: %s\n", BoardingMethodFile);
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

void RunSim(Person _PassengerList[], Person** _PassengerLocationMatrix[], bool UpdateVisuals, int* _StepsTaken, Map _PlaneMap, BasicSimulationRules _BaseRules)
{
	bool AllSeated = false;
	clock_t OneSecWatchStart, OneSecWatchEnd;
	int RPSCount = 0;
	int ShowRPCCount = 0;

	OneSecWatchStart = clock();

	while (!AllSeated)
	{
		AllSeated = true;
		for (int i = 0; i < _PlaneMap.NumberOfSeats; i++)
		{
			if (!_PassengerList[i].IsSeated)
			{
				AllSeated = false;

				PassengerMovement(&_PassengerList[i], _PassengerLocationMatrix, _PlaneMap, _BaseRules);

				if (IsPointEqual(_PassengerList[i].CurrentPos, _PassengerList[i].Target) && !_PassengerList[i].IsBackingUp)
				{
					_PassengerList[i].IsSeated = true;
				}
			}
		}

		if (UpdateVisuals)
		{
			PrintField(_PassengerLocationMatrix, _PlaneMap);
			printf("RPS: %d", ShowRPCCount);

			RPSCount++;

			OneSecWatchEnd = clock();

			if ((int)((((double)OneSecWatchEnd - (double)OneSecWatchStart) / CLOCKS_PER_SEC) * 1000) >= 1000)
			{
				OneSecWatchStart = clock();
				ShowRPCCount = RPSCount;
				RPSCount = 0;
			}
		}

		(*_StepsTaken)++;
	}
}

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
				
				int ItemCount = 0;
				for (int i = 0; i < BufferLength; i++)
				{
					if (SubStringBuffer[i] == '[')
						ItemCount++;
				}

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

				int ItemCount = 0;
				for (int i = 0; i < BufferLength; i++)
				{
					if (SubStringBuffer[i] == '[')
						ItemCount++;
				}

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

void PassengerMovement(Person* _Passenger, Person** _PassengerLocationMatrix[], Map _PlaneMap, BasicSimulationRules _BaseRules)
{
	bool TookAStep = false;

	if (!IsInDelayAction(_Passenger, _PlaneMap))
	{
		for (int j = 0; j < _Passenger->WalkingSpeed; j++)
		{
			if (!IsPointEqual(_Passenger->CurrentPos, _Passenger->Target) || _Passenger->IsBackingUp)
			{
				if (_Passenger->IsBackingUp)
				{
					_PassengerLocationMatrix[_Passenger->CurrentPos.Y][_Passenger->CurrentPos.X] = NULL;
					_PassengerLocationMatrix[_Passenger->Target.Y][_Passenger->Target.X] = _Passenger;
					_Passenger->CurrentPos = _Passenger->Target;
					_Passenger->IsBackingUp = false;
					_Passenger->IsSeated = true;

					TookAStep = true;
				}
				else
				{
					if (!IsAnyOnPoint(_PassengerLocationMatrix, _Passenger, _PlaneMap, _BaseRules))
					{
						_PassengerLocationMatrix[_Passenger->NextMove.Y][_Passenger->NextMove.X] = _Passenger;
						_PassengerLocationMatrix[_Passenger->CurrentPos.Y][_Passenger->CurrentPos.X] = NULL;
						_Passenger->CurrentPos = _Passenger->NextMove;

						TookAStep = true;
					}
					else
						break;
				}
			}
			else
				break;
		}
	}

	if (TookAStep)
	{
		_Passenger->StepsTaken++;
	}
}

void PrintField(Person* *_PassengerLocationMatrix[], Map _PlaneMap)
{
	system("cls");

	for (int y = 0; y < _PlaneMap.Height - 1; y++)
	{
		printf("| ");
		for (int x = 0; x < _PlaneMap.Width; x++)
		{
			if (_PassengerLocationMatrix[y][x] != NULL)
				printf(" %c  ", _PassengerLocationMatrix[y][x]->PersonCharacter);
			else
			{
				Location tmpLocation = *MapLocationGet(&_PlaneMap, x, y);
				switch (tmpLocation.BoardingGroup) {
					case BoardingGroup_Door:      printf(" D  "); break;
					case BoardingGroup_Walkway:   printf(" |  "); break;
					case BoardingGroup_Padding:   printf("    "); break;
					case BoardingGroup_Undefined: printf(" U  "); break;
					default:
						printf("%-3d ", tmpLocation.BoardingGroup);
					break;
				}
			}
		}
		printf("|\n");
	}
}

bool IsAnyOnPoint(Person* *_PassengerLocationMatrix[], Person *_Person, Map _PlaneMap, BasicSimulationRules _BaseRules)
{
	Point NewPoint;
	if (_Person->MovedLastTurn)
	{
		NewPoint = PredictedPoint(_Person->CurrentPos, _Person->Target);
		_Person->NextMove = NewPoint;
		_Person->MovedLastTurn = false;
	}
	else
		NewPoint = _Person->NextMove;

	if (_PassengerLocationMatrix[NewPoint.Y][NewPoint.X] != NULL)
	{
		Person* OtherPerson = _PassengerLocationMatrix[NewPoint.Y][NewPoint.X];
		Point SecondNewPoint;
		if (OtherPerson->MovedLastTurn)
		{
			SecondNewPoint = PredictedPoint(OtherPerson->CurrentPos, OtherPerson->Target);
			OtherPerson->NextMove = SecondNewPoint;
			OtherPerson->MovedLastTurn = false;
		}
		else
			SecondNewPoint = OtherPerson->NextMove;

		// Shuffle dance
		if (_Person->Target.Y == OtherPerson->Target.Y && _Person->CurrentPos.Y == _Person->Target.Y)
		{
			SendRowBack(_PassengerLocationMatrix, _Person, _PlaneMap, _BaseRules);

			return true;
		}

		// Cross
		if (IsPointEqual(SecondNewPoint, _Person->CurrentPos))
		{
			OtherPerson->CurrentPos = SecondNewPoint;
			OtherPerson->CrossDelay = _BaseRules.CrossDelay;
			OtherPerson->MovedLastTurn = true;
			_PassengerLocationMatrix[SecondNewPoint.Y][SecondNewPoint.X] = OtherPerson;

			_Person->CurrentPos = NewPoint;
			_Person->CrossDelay = _BaseRules.CrossDelay;
			_Person->MovedLastTurn = true;
			_PassengerLocationMatrix[NewPoint.Y][NewPoint.X] = _Person;

			return true;
		}

		return true;
	}

	_Person->MovedLastTurn = true;

	return false;
}

Point PredictedPoint(Point CurrentPoint, Point TargetPoint)
{
	Point NewPoint = { CurrentPoint.X, CurrentPoint.Y };
	if (NewPoint.Y == TargetPoint.Y) {
		if (TargetPoint.X > CurrentPoint.X)
			NewPoint = SetPoint(CurrentPoint.X + 1, CurrentPoint.Y);
		else
			NewPoint = SetPoint(CurrentPoint.X - 1, CurrentPoint.Y);
	}
	else
	{
		if (TargetPoint.Y > NewPoint.Y)
			NewPoint = SetPoint(CurrentPoint.X, CurrentPoint.Y + 1);
		else
			NewPoint = SetPoint(CurrentPoint.X, CurrentPoint.Y - 1);
	}
	return NewPoint;
}

void SendRowBack(Person* *_PassengerLocationMatrix[], Person *_Person, Map _PlaneMap, BasicSimulationRules _BaseRules)
{
	int CurrentXPosition = _Person->Target.X;
	int DistanceToTargetSeat = abs(_Person->Target.X - _PlaneMap.Doors[_Person->StartingDoorID].X);
	int InnerMostSeat = -1;
	while (CurrentXPosition != _PlaneMap.Doors[_Person->StartingDoorID].X)
	{
		if (_PassengerLocationMatrix[_Person->CurrentPos.Y][CurrentXPosition] != NULL)
		{
			Person* MomentPerson = _PassengerLocationMatrix[_Person->CurrentPos.Y][CurrentXPosition];
			if (InnerMostSeat == -1)
				InnerMostSeat = abs(MomentPerson->Target.X - _PlaneMap.Doors[_Person->StartingDoorID].X);

			MomentPerson->IsBackingUp = true;
			MomentPerson->ShuffleDelay = BackupWaitSteps(DistanceToTargetSeat, InnerMostSeat, _BaseRules.ShuffleDelay);
			MomentPerson->MovedLastTurn = true;
		}

		if (_Person->Target.X > _PlaneMap.Doors[_Person->StartingDoorID].X)
			CurrentXPosition--;
		else
			CurrentXPosition++;
	}

	_Person->IsBackingUp = true;
	_Person->ShuffleDelay = BackupWaitSteps(DistanceToTargetSeat, InnerMostSeat, _BaseRules.ShuffleDelay);
	_Person->MovedLastTurn = true;
}

int BackupWaitSteps(int TargetSeat, int InnerBlockingSeat, int ExtraPenalty)
{
	return TargetSeat + InnerBlockingSeat + 2 + ExtraPenalty;
}

bool IsInDelayAction(Person* _Person, Map _PlaneMap)
{
	if (_Person->LuggageCount > 0)
	{
		if (_Person->CurrentPos.Y == _Person->Target.Y && _Person->CurrentPos.X == _PlaneMap.Doors[_Person->StartingDoorID].X)
		{
			_Person->LuggageCount--;

			return true;
		}
	}

	if (_Person->ShuffleDelay > 0)
	{
		_Person->ShuffleDelay--;

		return true;
	}

	if (_Person->CrossDelay > 0)
	{
		_Person->CrossDelay--;

		return true;
	}

	return false;
}