#include "P1Draft.h"

int main()
{
	char UpdateGraphics = ' ';
	char SaveToFile = ' ';
	int RunsToDo = 0;
	int RunTime = 0;
	int AvrStepsTaken = 0;
	int StepsTaken = 0;

	Person PassengerList[MaxPersons];
	Person *PassengerLocationMatrix[MaxRows][MaxSeatsPrRow];
	clock_t TotalWatchStart, TotalWatchEnd, WatchStart, WatchEnd;
	Map Map;

	FILE* MapFile;
	fopen_s(&MapFile, "asd", "r");

	ReadMapFromFile(&Map, MapFile);

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
	fopen_s(&OutputFile, "output.csv", "w+");
	
	if (OutputFile != NULL)
	{
		if (SaveToFile == 'y')
			fprintf(OutputFile, "Iterations;\n");

		TotalWatchStart = clock();

		for (int i = 0; i < RunsToDo; i++)
		{
			RunTime = 0;

			memset(PassengerLocationMatrix, NULL, sizeof PassengerLocationMatrix);

			// Replace this later:
			GeneratePassengers(MaxPersons, PassengerList, Map);

			WatchStart = clock();

			RunSim(PassengerList, PassengerLocationMatrix, UpdateGraphics == 'y', &StepsTaken);

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

		fclose(OutputFile);

		printf("\n Finished! Took %d ms and an avr of %d iterations pr run\n", (int)((((double)TotalWatchEnd - (double)TotalWatchStart) / CLOCKS_PER_SEC) * 1000), (AvrStepsTaken / RunsToDo));
	}
	system("pause");

	return 0;
}

void RunSim(Person _PassengerList[MaxPersons], Person* _PassengerLocationMatrix[MaxRows][MaxSeatsPrRow], bool UpdateVisuals, int* _StepsTaken)
{
	bool AllSeated = false;
	clock_t OneSecWatchStart, OneSecWatchEnd;
	int RPSCount = 0;
	int ShowRPCCount = 0;

	OneSecWatchStart = clock();

	while (!AllSeated)
	{
		AllSeated = true;
		for (int i = 0; i < MaxPersons; i++)
		{
			if (!_PassengerList[i].IsSeated)
			{
				AllSeated = false;

				PassengerMovement(&_PassengerList[i], _PassengerLocationMatrix);

				if (IsPointEqual(_PassengerList[i].CurrentPos, _PassengerList[i].Target) && !_PassengerList[i].IsBackingUp)
				{
					_PassengerList[i].IsSeated = true;
				}
			}
		}

		if (UpdateVisuals)
		{
			PrintField(_PassengerLocationMatrix, BaseFieldData);
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

void PassengerMovement(Person* _Passenger, Person *_PassengerLocationMatrix[MaxRows][MaxSeatsPrRow])
{
	bool TookAStep = false;

	if (!IsInDelayAction(_Passenger))
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
					if (!IsAnyOnPoint(_PassengerLocationMatrix, _Passenger))
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

void PrintField(Person* _PassengerLocationMatrix[MaxRows][MaxSeatsPrRow], const char _BaseFieldData[MaxRows][MaxSeatsPrRow])
{
	system("cls");

	for (int i = 0; i < MaxRows; i++)
	{
		printf("| ");
		for (int j = 0; j < MaxSeatsPrRow; j++)
		{
			if (_PassengerLocationMatrix[i][j] != NULL)
				printf("%c ", _PassengerLocationMatrix[i][j]->PersonCharacter);
			else
				printf("%c ", _BaseFieldData[i][j]);
		}
		printf("|\n");
	}
}

bool IsAnyOnPoint(Person *_PassengerLocationMatrix[MaxRows][MaxSeatsPrRow], Person *_Person)
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
			SendRowBack(_PassengerLocationMatrix, _Person);

			return true;
		}

		// Cross
		if (IsPointEqual(SecondNewPoint, _Person->CurrentPos))
		{
			OtherPerson->CurrentPos = SecondNewPoint;
			OtherPerson->CrossDelay = BSR.CrossDelay;
			OtherPerson->MovedLastTurn = true;
			_PassengerLocationMatrix[SecondNewPoint.Y][SecondNewPoint.X] = OtherPerson;

			_Person->CurrentPos = NewPoint;
			_Person->CrossDelay = BSR.CrossDelay;
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

void SendRowBack(Person* _PassengerLocationMatrix[MaxRows][MaxSeatsPrRow], Person *_Person)
{
	int CurrentXPosition = _Person->Target.X;
	int DistanceToTargetSeat = abs(_Person->Target.X - Doors[_Person->StartingDoorID].X);
	int InnerMostSeat = -1;
	while (CurrentXPosition != Doors[_Person->StartingDoorID].X)
	{
		if (_PassengerLocationMatrix[_Person->CurrentPos.Y][CurrentXPosition] != NULL)
		{
			Person* MomentPerson = _PassengerLocationMatrix[_Person->CurrentPos.Y][CurrentXPosition];
			if (InnerMostSeat == -1)
				InnerMostSeat = abs(MomentPerson->Target.X - Doors[_Person->StartingDoorID].X);

			MomentPerson->IsBackingUp = true;
			MomentPerson->ShuffleDelay = BackupWaitSteps(DistanceToTargetSeat, InnerMostSeat, BSR.ShuffleDelay);
			MomentPerson->MovedLastTurn = true;
		}

		if (_Person->Target.X > Doors[_Person->StartingDoorID].X)
			CurrentXPosition--;
		else
			CurrentXPosition++;
	}

	_Person->IsBackingUp = true;
	_Person->ShuffleDelay = BackupWaitSteps(DistanceToTargetSeat, InnerMostSeat, BSR.ShuffleDelay);
	_Person->MovedLastTurn = true;
}

int BackupWaitSteps(int TargetSeat, int InnerBlockingSeat, int ExtraPenalty)
{
	return TargetSeat + InnerBlockingSeat + 2 + ExtraPenalty;
}

bool IsInDelayAction(Person* _Person)
{
	if (_Person->LuggageCount > 0)
	{
		if (_Person->CurrentPos.Y == _Person->Target.Y && _Person->CurrentPos.X == Doors[_Person->StartingDoorID].X)
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