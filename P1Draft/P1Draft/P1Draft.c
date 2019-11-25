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

void SaveRunDataToFile(FILE* _fp, Person _PersonList[MaxPersons], int _RunTime, int RunNumber)
{
	for (int i = 1; i < MaxPersons; i++)
	{
		fprintf(_fp, "%d;%d;%d;%d;%d;%d\n", RunNumber, _RunTime, _PersonList[i].WalkingSpeed, _PersonList[i].OrgLuggageCount, _PersonList[i].StartingDoorID, _PersonList[i].StepsTaken);
	}
}

void GenerateProgressBar(char PBar[ProgressBarSteps], int Run, int MaxRun)
{
	PBar[(int)(((double)Run / (double)MaxRun) * ProgressBarSteps)] = 'X';
}

void RunSim(Person _PersonList[MaxPersons], Person* _IsPersonArray[MaxRows][MaxSeatsPrRow], bool UpdateVisuals)
{
	bool AllSeated = false;
	while (!AllSeated)
	{
		AllSeated = true;
		for (int i = 0; i < MaxPersons; i++)
		{
			if (!_PersonList[i].IsSeated)
			{
				AllSeated = false;

				PassengerMovement(i, _PersonList, _IsPersonArray);

				if (IsPointEqu(_PersonList[i].CurrentPos, _PersonList[i].Target) && !_PersonList[i].IsBackingUp)
				{
					_PersonList[i].IsSeated = true;
				}
			}
		}

		if (UpdateVisuals)
		{
			PrintField(_PersonList, BaseFieldData);
		}
	}
}

void PassengerMovement(int Index, Person _PersonList[MaxPersons], Person *_IsPersonArray[MaxRows][MaxSeatsPrRow])
{
	bool TookAStep = false;

	if (!IsInDelayAction(&_PersonList[Index]))
	{
		for (int j = 0; j < _PersonList[Index].WalkingSpeed; j++)
		{
			if (!IsPointEqu(_PersonList[Index].CurrentPos, _PersonList[Index].Target) || _PersonList[Index].IsBackingUp)
			{
				if (_PersonList[Index].IsBackingUp)
				{
					_IsPersonArray[_PersonList[Index].Target.Y][_PersonList[Index].Target.X] = &_PersonList[Index];
					_IsPersonArray[_PersonList[Index].CurrentPos.Y][_PersonList[Index].CurrentPos.X] = NULL;
					_PersonList[Index].CurrentPos = _PersonList[Index].Target;
					_PersonList[Index].IsBackingUp = false;
					_PersonList[Index].IsSeated = true;

					TookAStep = true;
				}
				else
				{
					if (!IsAnyOnPoint(_IsPersonArray, &_PersonList[Index]))
					{
						Point NewPoint = PredictedPoint(_PersonList[Index].CurrentPos, _PersonList[Index].Target);
						_IsPersonArray[NewPoint.Y][NewPoint.X] = &_PersonList[Index];
						_IsPersonArray[_PersonList[Index].CurrentPos.Y][_PersonList[Index].CurrentPos.X] = NULL;
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
	}

	if (TookAStep)
	{
		_PersonList[Index].StepsTaken++;
	}
}

void PrintField(Person _PersonList[MaxPersons], const char _BaseFieldData[MaxRows][MaxSeatsPrRow])
{
	char FieldData[MaxRows][MaxSeatsPrRow] = { 0 };

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

bool IsAnyOnPoint(Person *_IsPersonArray[MaxRows][MaxSeatsPrRow], Person *_Person)
{
	Person FirstPerson = *_Person;
	Point NewPoint = PredictedPoint(FirstPerson.CurrentPos, FirstPerson.Target);
	if (_IsPersonArray[NewPoint.Y][NewPoint.X] != NULL)
	{
		Person* OtherPerson = _IsPersonArray[NewPoint.Y][NewPoint.X];
		Point SecondNewPoint = PredictedPoint(OtherPerson->CurrentPos, OtherPerson->Target);

		if (FirstPerson.Target.Y == OtherPerson->Target.Y && FirstPerson.CurrentPos.Y == OtherPerson->CurrentPos.Y)
		{
			SendRowBack(_IsPersonArray, _Person);
			return true;
		}

		if (IsPointEqu(SecondNewPoint, FirstPerson.CurrentPos))
		{
			OtherPerson->CurrentPos = SecondNewPoint;
			OtherPerson->CrossDelay = BSR.BSR_CrossDelay;
			_IsPersonArray[SecondNewPoint.Y][SecondNewPoint.X] = OtherPerson;

			_Person->CurrentPos = NewPoint;
			_Person->CrossDelay = BSR.BSR_CrossDelay;
			_IsPersonArray[NewPoint.Y][NewPoint.X] = _Person;

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

void SendRowBack(Person* _IsPersonArray[MaxRows][MaxSeatsPrRow], Person *_Person)
{
	Person SenderPerson = *_Person;

	if (SenderPerson.Target.X > Doors[SenderPerson.StartingDoorID].X)
	{
		int InnerMostSeat = -1;
		for (int i = SenderPerson.Target.X; i > Doors[SenderPerson.StartingDoorID].X; i--)
		{
			if (_IsPersonArray[SenderPerson.CurrentPos.Y][i] != NULL)
			{
				Person MomentPerson = *_IsPersonArray[SenderPerson.CurrentPos.Y][i];
				if (InnerMostSeat == -1)
					InnerMostSeat = MomentPerson.Target.X - Doors[SenderPerson.StartingDoorID].X;
				_IsPersonArray[SenderPerson.CurrentPos.Y][i]->IsBackingUp = true;
				_IsPersonArray[SenderPerson.CurrentPos.Y][i]->ShuffleDelay = BackupWaitSteps(SenderPerson.Target.X - Doors[SenderPerson.StartingDoorID].X, InnerMostSeat, BSR.BSR_ShuffleDelay);
			}
		}
		if (_IsPersonArray[SenderPerson.CurrentPos.Y][SenderPerson.CurrentPos.X] != NULL)
		{
			_Person->IsBackingUp = true;
			_Person->ShuffleDelay = BackupWaitSteps(SenderPerson.Target.X - Doors[SenderPerson.StartingDoorID].X, InnerMostSeat, BSR.BSR_ShuffleDelay);
		}
	}
	else
	{
		int InnerMostSeat = -1;
		for (int i = SenderPerson.Target.X; i < Doors[SenderPerson.StartingDoorID].X; i++)
		{
			if (_IsPersonArray[SenderPerson.CurrentPos.Y][i] != NULL)
			{
				Person MomentPerson = *_IsPersonArray[SenderPerson.CurrentPos.Y][i];
				if (InnerMostSeat == -1)
					InnerMostSeat = MomentPerson.Target.X;
				_IsPersonArray[SenderPerson.CurrentPos.Y][i]->IsBackingUp = true;
				_IsPersonArray[SenderPerson.CurrentPos.Y][i]->ShuffleDelay = BackupWaitSteps(SenderPerson.Target.X, InnerMostSeat, BSR.BSR_ShuffleDelay);
			}
		}
		if (_IsPersonArray[SenderPerson.CurrentPos.Y][SenderPerson.CurrentPos.X] != NULL)
		{
			_Person->IsBackingUp = true;
			_Person->ShuffleDelay = BackupWaitSteps(SenderPerson.Target.X, InnerMostSeat, BSR.BSR_ShuffleDelay);
		}
	}
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