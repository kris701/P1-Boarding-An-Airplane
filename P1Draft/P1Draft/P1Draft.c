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

	// Momentary, read from file, maybe a config file
	PlaneMap.SSR.CrossDelay = 0;
	PlaneMap.SSR.ShuffleDelay = 5;

	srand(time(NULL));

	FILE* mapfp;
	fopen_s(&mapfp, "steffenperfect.txt", "r");

	if (mapfp != NULL)
	{
		ReadMapFromFile(&PlaneMap, mapfp);

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

				GeneratePassengers(PlaneMap.NumberOfSeats, PassengerList, PlaneMap);

				WatchStart = clock();

				RunSim(PassengerList, PassengerLocationMatrix, UpdateGraphics == 'y', &StepsTaken, PlaneMap);

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

			printf("\n Finished! Took %d ms and an avr of %d iterations pr run\n", (int)((((double)TotalWatchEnd - (double)TotalWatchStart) / CLOCKS_PER_SEC) * 1000), (AvrStepsTaken / RunsToDo));
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

void RunSim(Person _PassengerList[], Person** _PassengerLocationMatrix[], bool UpdateVisuals, int* _StepsTaken, Map _PlaneMap)
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

				PassengerMovement(&_PassengerList[i], _PassengerLocationMatrix, _PlaneMap);

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

void PassengerMovement(Person* _Passenger, Person** _PassengerLocationMatrix[], Map _PlaneMap)
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
					if (!IsAnyOnPoint(_PassengerLocationMatrix, _Passenger, _PlaneMap))
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
				printf("%c ", _PassengerLocationMatrix[y][x]->PersonCharacter);
			else
			{
				Location MomentLoc = *MapLocationGet(&_PlaneMap, x, y);
				switch (MomentLoc.BoardingGroup)
				{
				case BoardingGroup_Door:
					printf("D ");
					break;
				case BoardingGroup_Walkway:
					printf("| ");
					break;
				case BoardingGroup_Padding:
					printf("  ");
					break;
				case BoardingGroup_Undefined:
					printf("U ");
					break;
				default:
					printf(". ");
					break;
				}
			}
		}
		printf("|\n");
	}
}

bool IsAnyOnPoint(Person* *_PassengerLocationMatrix[], Person *_Person, Map _PlaneMap)
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
			SendRowBack(_PassengerLocationMatrix, _Person, _PlaneMap);

			return true;
		}

		// Cross
		if (IsPointEqual(SecondNewPoint, _Person->CurrentPos))
		{
			OtherPerson->CurrentPos = SecondNewPoint;
			OtherPerson->CrossDelay = _PlaneMap.SSR.CrossDelay;
			OtherPerson->MovedLastTurn = true;
			_PassengerLocationMatrix[SecondNewPoint.Y][SecondNewPoint.X] = OtherPerson;

			_Person->CurrentPos = NewPoint;
			_Person->CrossDelay = _PlaneMap.SSR.CrossDelay;
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

void SendRowBack(Person* *_PassengerLocationMatrix[], Person *_Person, Map _PlaneMap)
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
			MomentPerson->ShuffleDelay = BackupWaitSteps(DistanceToTargetSeat, InnerMostSeat, _PlaneMap.SSR.ShuffleDelay);
			MomentPerson->MovedLastTurn = true;
		}

		if (_Person->Target.X > _PlaneMap.Doors[_Person->StartingDoorID].X)
			CurrentXPosition--;
		else
			CurrentXPosition++;
	}

	_Person->IsBackingUp = true;
	_Person->ShuffleDelay = BackupWaitSteps(DistanceToTargetSeat, InnerMostSeat, _PlaneMap.SSR.ShuffleDelay);
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