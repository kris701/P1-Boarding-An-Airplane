#include "P1Draft.h"

int main()
{
	char YNSelection = ' ';
	int RunsToDo = 0;
	bool AllSeated = false;
	int RunTime = 0;

	// Dette bliver erstatet snart:
	MethodDefinition Def = { 0, MaxSeatsPrRow, 0, MaxRows, 0, 3, { Line, 1, 0, 0 }, 0, 1, { Line, 1, 0, 0 }, 1, 1, { Line, 1, 0, 0 } };

	Person PassengerList[MaxPersons];
	Person *PassengerArray[MaxRows][MaxSeatsPrRow];
	clock_t TotalWatchStart, TotalWatchEnd, WatchStart, WatchEnd;

	while (YNSelection != 'y' && YNSelection != 'n')
	{
		printf("Update graphics? (y/n)\n");
		scanf_s(" %c", &YNSelection, 1);
	}
	while (RunsToDo <= 0 || RunsToDo > MaxRuns)
	{
		printf("How many runs?: \n");
		scanf_s(" %d", &RunsToDo);
	}

	FILE* fp;
	fopen_s(&fp, "output.csv", "w+");
	
	if (fp != NULL)
	{
		fprintf(fp, "RunNumber;RunTime;WalkingSpeed;OrgLuggageCount;StartingDoorID;StepsTaken\n");

		TotalWatchStart = clock();

		for (int i = 0; i < RunsToDo; i++)
		{
			RunTime = 0;

			memset(PassengerArray, NULL, sizeof PassengerArray);

			// Replace this later:
			GeneratePassengers(MaxPersons, PassengerList, MI_Random, Def);

			WatchStart = clock();

			RunSim(PassengerList, PassengerArray, YNSelection == 'y');

			WatchEnd = clock();

			RunTime = (int)((double)WatchEnd - (double)WatchStart);

			if (YNSelection == 'y')
			{
				printf("All seated! Took: %d ticks\n", RunTime);

				Sleep(1000);
			}

			SaveRunDataToFile(fp, PassengerList, RunTime, i);
		}

		TotalWatchEnd = clock();

		fclose(fp);

		printf("\n Finished! Took %d ms \n", (int)((((double)TotalWatchEnd - (double)TotalWatchStart) / CLOCKS_PER_SEC) * 1000));
	}
	system("pause");

	return 0;
}

void SaveRunDataToFile(FILE* _fp, Person _PassengerList[MaxPersons], int _RunTime, int RunNumber)
{
	for (int i = 1; i < MaxPersons; i++)
	{
		fprintf(_fp, "%d;%d;%d;%d;%d;%d\n", RunNumber, _RunTime, _PassengerList[i].WalkingSpeed, _PassengerList[i].OrgLuggageCount, _PassengerList[i].StartingDoorID, _PassengerList[i].StepsTaken);
	}
}

void RunSim(Person _PassengerList[MaxPersons], Person* _PassengerArray[MaxRows][MaxSeatsPrRow], bool UpdateVisuals)
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

				PassengerMovement(i, _PassengerList, _PassengerArray);

				if (IsPointEqual(_PassengerList[i].CurrentPos, _PassengerList[i].Target) && !_PassengerList[i].IsBackingUp)
				{
					_PassengerList[i].IsSeated = true;
				}
			}
		}

		if (UpdateVisuals)
		{
			PrintField(_PassengerArray, BaseFieldData);
			printf("RPS: %d", ShowRPCCount);
		}

		RPSCount++;

		OneSecWatchEnd = clock();

		if ((int)((((double)OneSecWatchEnd - (double)OneSecWatchStart) / CLOCKS_PER_SEC) * 1000) >= 1000)
		{
			OneSecWatchStart = clock();
			ShowRPCCount = RPSCount;
			RPSCount = 0;
		}
	}
}

void PassengerMovement(int Index, Person _PassengerList[MaxPersons], Person *_PassengerArray[MaxRows][MaxSeatsPrRow])
{
	bool TookAStep = false;

	if (!IsInDelayAction(&_PassengerList[Index]))
	{
		for (int j = 0; j < _PassengerList[Index].WalkingSpeed; j++)
		{
			if (!IsPointEqual(_PassengerList[Index].CurrentPos, _PassengerList[Index].Target) || _PassengerList[Index].IsBackingUp)
			{
				if (_PassengerList[Index].IsBackingUp)
				{
					_PassengerArray[_PassengerList[Index].Target.Y][_PassengerList[Index].Target.X] = &_PassengerList[Index];
					_PassengerArray[_PassengerList[Index].CurrentPos.Y][_PassengerList[Index].CurrentPos.X] = NULL;
					_PassengerList[Index].CurrentPos = _PassengerList[Index].Target;
					_PassengerList[Index].IsBackingUp = false;
					_PassengerList[Index].IsSeated = true;

					TookAStep = true;
				}
				else
				{
					if (!IsAnyOnPoint(_PassengerArray, &_PassengerList[Index]))
					{
						Point NewPoint = PredictedPoint(_PassengerList[Index].CurrentPos, _PassengerList[Index].Target);
						_PassengerArray[NewPoint.Y][NewPoint.X] = &_PassengerList[Index];
						_PassengerArray[_PassengerList[Index].CurrentPos.Y][_PassengerList[Index].CurrentPos.X] = NULL;
						_PassengerList[Index].CurrentPos = NewPoint;

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
		_PassengerList[Index].StepsTaken++;
	}
}

void PrintField(Person* _PassengerArray[MaxRows][MaxSeatsPrRow], const char _BaseFieldData[MaxRows][MaxSeatsPrRow])
{
	system("cls");

	for (int i = 0; i < MaxRows; i++)
	{
		printf("| ");
		for (int j = 0; j < MaxSeatsPrRow; j++)
		{
			if (_PassengerArray[i][j] != NULL)
				printf("%c ", _PassengerArray[i][j]->PersonCharacter);
			else
				printf("%c ", _BaseFieldData[i][j]);
		}
		printf("|\n");
	}
}

bool IsAnyOnPoint(Person *_PassengerArray[MaxRows][MaxSeatsPrRow], Person *_Person)
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

	if (_PassengerArray[NewPoint.Y][NewPoint.X] != NULL)
	{
		Person* OtherPerson = _PassengerArray[NewPoint.Y][NewPoint.X];
		Point SecondNewPoint;
		if (OtherPerson->MovedLastTurn)
		{
			SecondNewPoint = PredictedPoint(OtherPerson->CurrentPos, OtherPerson->Target);
			OtherPerson->NextMove = NewPoint;
			OtherPerson->MovedLastTurn = false;
		}
		else
			SecondNewPoint = OtherPerson->NextMove;

		// Shuffle dance
		if (_Person->Target.Y == OtherPerson->Target.Y && _Person->CurrentPos.Y == OtherPerson->CurrentPos.Y)
		{
			SendRowBack(_PassengerArray, _Person);

			return true;
		}

		// Cross
		if (IsPointEqual(SecondNewPoint, _Person->CurrentPos))
		{
			OtherPerson->CurrentPos = SecondNewPoint;
			OtherPerson->CrossDelay = BSR.CrossDelay;
			OtherPerson->MovedLastTurn = true;
			_PassengerArray[SecondNewPoint.Y][SecondNewPoint.X] = OtherPerson;

			_Person->CurrentPos = NewPoint;
			_Person->CrossDelay = BSR.CrossDelay;
			_Person->MovedLastTurn = true;
			_PassengerArray[NewPoint.Y][NewPoint.X] = _Person;

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

Point PredictedPointInvX(Point _CurrentPos, XAxis TargX, XAxis DoorX)
{
	Point NewPoint = { _CurrentPos.X, _CurrentPos.Y };
	if (TargX > DoorX)
		NewPoint.X -= 1;
	else
		NewPoint.X += 1;

	return NewPoint;
}

void SendRowBack(Person* _PassengerArray[MaxRows][MaxSeatsPrRow], Person *_Person)
{
	int CurrentXPosition = _Person->Target.X;
	int DistanceToTargetSeat = abs(_Person->Target.X - Doors[_Person->StartingDoorID].X);
	int InnerMostSeat = -1;
	while (CurrentXPosition != Doors[_Person->StartingDoorID].X)
	{
		if (_PassengerArray[_Person->CurrentPos.Y][CurrentXPosition] != NULL)
		{
			Person* MomentPerson = _PassengerArray[_Person->CurrentPos.Y][CurrentXPosition];
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