#include "P1Draft.h"

int main()
{
	char Selection = ' ';
	int DoRuns = 0;
	char ProgressValue[] = "..........";
	bool AllSeated = false;
	int RunTime = 0;
	MethodDefinition Def = { 0, MaxSeatsPrRow, 0, MaxRows, 0, 3, { Line, 1, 0, 0 }, 0, 1, { Line, 1, 0, 0 }, 1, 1, { Line, 1, 0, 0 } };
	Person PersonList[MaxPersons];
	Person *IsPersonArray[MaxRows][MaxSeatsPrRow];
	clock_t TotalWatchStart, TotalWatchEnd, WatchStart, WatchEnd;

	while (Selection != 'y' && Selection != 'n')
	{
		printf("Update graphics? (y/n)\n");
		scanf_s(" %c", &Selection, 1);
	}
	while (DoRuns <= 0 || DoRuns > MaxRuns)
	{
		printf("How many runs?: \n");
		scanf_s(" %d", &DoRuns);
	}

	FILE* fp;
	fopen_s(&fp, "Run.csv", "w+");
	
	if (fp != NULL)
	{
		fprintf(fp, "RunNumber;RunTime;WalkingSpeed;OrgLuggageCount;StartingDoorID;StepsTaken\n");

		TotalWatchStart = clock();

		for (int i = 0; i < DoRuns; i++)
		{
			RunTime = 0;

			memset(IsPersonArray, NULL, sizeof IsPersonArray);

			GeneratePassengers(MaxPersons, PersonList, MI_Random, Def);

			WatchStart = clock();

			RunSim(PersonList, IsPersonArray, Selection == 'y');

			WatchEnd = clock();

			RunTime = (int)((double)WatchEnd - (double)WatchStart);

			if (Selection == 'y')
			{
				printf("All seated! Took: %d ticks\n", RunTime);

				GenerateProgressBar(ProgressValue, i, DoRuns);
				printf("\n [ %s ]", ProgressValue);

				Sleep(1000);
			}

			SaveRunDataToFile(fp, PersonList, RunTime, i);
		}

		TotalWatchEnd = clock();

		fclose(fp);

		printf("\n Finished! Took %d ms \n", (int)((((double)TotalWatchEnd - (double)TotalWatchStart) / CLOCKS_PER_SEC) * 1000));
	}
	system("pause");

	return 0;
}

void SaveRunDataToFile(FILE* fp, Person PersonList[MaxPersons], int RunTime, int RunNumber)
{
	for (int i = 1; i < MaxPersons; i++)
	{
		fprintf(fp, "%d;%d;%d;%d;%d;%d\n", RunNumber, RunTime, PersonList[i].WalkingSpeed, PersonList[i].OrgLuggageCount, PersonList[i].StartingDoorID, PersonList[i].StepsTaken);
	}
}

void GenerateProgressBar(char PBar[ProgressBarSteps], int Run, int MxRun)
{
	PBar[(int)(((double)Run / (double)MxRun) * ProgressBarSteps)] = 'X';
}

void RunSim(Person PersonList[MaxPersons], Person* IsPersonArray[MaxRows][MaxSeatsPrRow], bool UpdateVisuals)
{
	bool AllSeated = false;
	while (!AllSeated)
	{
		AllSeated = true;
		for (int i = 0; i < MaxPersons; i++)
		{
			if (!PersonList[i].IsSeated)
			{
				AllSeated = false;

				PersonMovement(i, PersonList, IsPersonArray);

				if (IsPointEqu(PersonList[i].CurrentPos, PersonList[i].Target) && !PersonList[i].IsBackingUp)
				{
					PersonList[i].IsSeated = true;
				}
			}
		}

		if (UpdateVisuals)
		{
			PrintField(PersonList, BaseFieldData);
		}
	}
}

void PersonMovement(int Index, Person _PersonList[MaxPersons], Person *IsPersonArray[MaxRows][MaxSeatsPrRow])
{
	bool TookAStep = false;
	for (int j = 0; j < _PersonList[Index].WalkingSpeed; j++)
	{
		if (!IsPointEqu(_PersonList[Index].CurrentPos, _PersonList[Index].Target) || _PersonList[Index].IsBackingUp)
		{
			if (_PersonList[Index].LuggageCount != 0)
			{
				if (_PersonList[Index].CurrentPos.Y == _PersonList[Index].Target.Y && _PersonList[Index].CurrentPos.X == Doors[_PersonList[Index].StartingDoorID].X)
				{
					_PersonList[Index].LuggageCount--;

					TookAStep = true;
					break;
				}
			}

			if (_PersonList[Index].IsBackingUp)
			{
				if (_PersonList[Index].ShuffleDelay > 0)
				{
					_PersonList[Index].ShuffleDelay--;
					if (_PersonList[Index].ShuffleDelay == 0)
					{
						IsPersonArray[_PersonList[Index].CurrentPos.Y][_PersonList[Index].CurrentPos.X] = NULL;
						IsPersonArray[_PersonList[Index].Target.Y][_PersonList[Index].Target.X] = &_PersonList[Index];
						_PersonList[Index].CurrentPos = _PersonList[Index].Target;
						_PersonList[Index].IsBackingUp = false;
						_PersonList[Index].IsSeated = true;
					}
				}
			}
			else
			{
				Point NewPoint = PredictedPoint(_PersonList[Index].CurrentPos, _PersonList[Index].Target);
				if (!IsAnyOnPoint(IsPersonArray, &_PersonList[Index]))
				{
					IsPersonArray[_PersonList[Index].CurrentPos.Y][_PersonList[Index].CurrentPos.X] = NULL;
					IsPersonArray[NewPoint.Y][NewPoint.X] = &_PersonList[Index];
					_PersonList[Index].CurrentPos = NewPoint;

					TookAStep = true;
				}
				else
					break;
			}
		}
		else
			break;
	}

	if (TookAStep)
	{
		_PersonList[Index].StepsTaken++;
	}
}

void PrintField(Person _PersonList[MaxPersons], const char _BaseFieldData[MaxRows][MaxSeatsPrRow])
{
	char FieldData[MaxRows][MaxSeatsPrRow];
	memset(FieldData, 0, sizeof FieldData);

	for (int i = 0; i < MaxPersons; i++)
	{
		if (FieldData[_PersonList[i].CurrentPos.Y][_PersonList[i].CurrentPos.X] != '\0')
			FieldData[_PersonList[i].CurrentPos.Y][_PersonList[i].CurrentPos.X] = MultiPersonChar;
		else
			FieldData[_PersonList[i].CurrentPos.Y][_PersonList[i].CurrentPos.X] = _PersonList[i].PersonCharacter;
	}

	system("cls");

	for (int i = 0; i < MaxRows; i++)
	{
		printf("| ");
		for (int j = 0; j < MaxSeatsPrRow; j++)
		{
			if (FieldData[i][j] != '\0')
				printf("%c ", FieldData[i][j]);
			else
				printf("%c ", _BaseFieldData[i][j]);
		}
		printf("|\n");
	}
}

bool IsAnyOnPoint(Person *IsPersonArray[MaxRows][MaxSeatsPrRow], Person *_Person)
{
	Person FirstPerson = *_Person;
	Point NewPoint = PredictedPoint(FirstPerson.CurrentPos, FirstPerson.Target);
	if (IsPersonArray[NewPoint.Y][NewPoint.X] != NULL)
	{
		Person OtherPerson = *IsPersonArray[NewPoint.Y][NewPoint.X];
		Point SecondNewPoint = PredictedPoint(OtherPerson.CurrentPos, OtherPerson.Target);

		if (FirstPerson.Target.Y == OtherPerson.Target.Y && FirstPerson.CurrentPos.Y == OtherPerson.CurrentPos.Y)
		{
			SendRowBack(IsPersonArray, _Person);
			return true;
		}

		if (IsPointEqu(SecondNewPoint, FirstPerson.CurrentPos))
		{
			_Person->CurrentPos = NewPoint;
			IsPersonArray[OtherPerson.CurrentPos.Y][OtherPerson.CurrentPos.X] = _Person;

			OtherPerson.CurrentPos = SecondNewPoint;
			IsPersonArray[SecondNewPoint.Y][SecondNewPoint.X] = &OtherPerson;

			return true;
		}

		return true;
	}

	return false;
}

Point PredictedPoint(Point CurrentPoint, Point TargetPoint)
{
	Point NewPoint = { CurrentPoint.X, CurrentPoint.Y };
	if (NewPoint.Y == TargetPoint.Y)
	{
		if (TargetPoint.X > CurrentPoint.X)
		{
			NewPoint = SetPoint(CurrentPoint.X + 1, CurrentPoint.Y);
		}
		else
		{
			NewPoint = SetPoint(CurrentPoint.X - 1, CurrentPoint.Y);
		}
	}
	else
	{
		if (TargetPoint.Y > NewPoint.Y)
		{
			NewPoint = SetPoint(CurrentPoint.X, CurrentPoint.Y + 1);
		}
		else
		{
			NewPoint = SetPoint(CurrentPoint.X, CurrentPoint.Y - 1);
		}
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

void SendRowBack(Person* IsPersonArray[MaxRows][MaxSeatsPrRow], Person *_Person)
{
	Person SenderPerson = *_Person;

	if (SenderPerson.Target.X > Doors[SenderPerson.StartingDoorID].X)
	{
		int InnerMostSeat = -1;
		for (int i = SenderPerson.Target.X; i > Doors[SenderPerson.StartingDoorID].X; i--)
		{
			if (IsPersonArray[SenderPerson.CurrentPos.Y][i] != NULL)
			{
				Person MomentPerson = *IsPersonArray[SenderPerson.CurrentPos.Y][i];
				if (InnerMostSeat == -1)
					InnerMostSeat = MomentPerson.Target.X - Doors[SenderPerson.StartingDoorID].X;
				IsPersonArray[SenderPerson.CurrentPos.Y][i]->IsBackingUp = true;
				IsPersonArray[SenderPerson.CurrentPos.Y][i]->ShuffleDelay = BackupWaitSteps(SenderPerson.Target.X - Doors[SenderPerson.StartingDoorID].X, InnerMostSeat);
			}
		}
		if (IsPersonArray[SenderPerson.CurrentPos.Y][SenderPerson.CurrentPos.X] != NULL)
		{
			_Person->IsBackingUp = true;
			_Person->ShuffleDelay = BackupWaitSteps(SenderPerson.Target.X - Doors[SenderPerson.StartingDoorID].X, InnerMostSeat);
		}
	}
	else
	{
		int InnerMostSeat = -1;
		for (int i = SenderPerson.Target.X; i < Doors[SenderPerson.StartingDoorID].X; i++)
		{
			if (IsPersonArray[SenderPerson.CurrentPos.Y][i] != NULL)
			{
				Person MomentPerson = *IsPersonArray[SenderPerson.CurrentPos.Y][i];
				if (InnerMostSeat == -1)
					InnerMostSeat = MomentPerson.Target.X;
				IsPersonArray[SenderPerson.CurrentPos.Y][i]->IsBackingUp = true;
				IsPersonArray[SenderPerson.CurrentPos.Y][i]->ShuffleDelay = BackupWaitSteps(SenderPerson.Target.X, InnerMostSeat);
			}
		}
		if (IsPersonArray[SenderPerson.CurrentPos.Y][SenderPerson.CurrentPos.X] != NULL)
		{
			_Person->IsBackingUp = true;
			_Person->ShuffleDelay = BackupWaitSteps(SenderPerson.Target.X, InnerMostSeat);
		}
	}
}

int BackupWaitSteps(int TargetSeat, int InnerBlockingSeat)
{
	return TargetSeat + InnerBlockingSeat + 2;
}