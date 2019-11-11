#include "P1Draft.h"

int main()
{
	char Selection = ' ';
	int DoRuns = 0;
	char ProgressValue[] = "..........";
	bool AllSeated = false;
	int RunTime = 0;
	struct MethodDefinition Def = { 0, MaxSeatsPrRow, 0, MaxRows, 0, 2, { Line, 1, 0, 0 }, 0, 1, { Line, 1, 0, 0 }, 1, 1, { Line, 1, 0, 0 } };
	struct Person PersonList[MaxRows * 2 + 1][MaxPersons];
	int PersonListIndexes[MaxRows * 2 + 1];
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
	
	fprintf(fp, "RunNumber;RunTime;WalkingSpeed;OrgLuggageCount;StartingDoorID;StepsTaken\n");
	
	TotalWatchStart = clock();

	for(int i = 0; i < DoRuns; i++)
	{
		RunTime = 0;
		
		GeneratePassengers(MaxPersons, PersonList, MI_Random, Def);

		memset(PersonListIndexes, 0, sizeof PersonListIndexes);
		PersonListIndexes[0] = MaxPersons;

		WatchStart = clock();

		RunSim(PersonList, PersonListIndexes, Selection == 'y');

		WatchEnd = clock();

		RunTime = (int)((double)WatchEnd - (double)WatchStart);

		if (Selection == 'y')
		{
			printf("All seated! Took: %d ticks\n", RunTime);

			GenerateProgressBar(ProgressValue, i, DoRuns);
			printf("\n [ %s ]", ProgressValue);

			Sleep(1000);
		}

		SaveRunDataToFile(fp, PersonList, PersonListIndexes, RunTime, i);
	}

	TotalWatchEnd = clock();

	fclose(fp);

	printf("\n Finished! Took %d ms \n", (int)((((double)TotalWatchEnd - (double)TotalWatchStart) / CLOCKS_PER_SEC) * 1000));
	system("pause");

	return 0;
}

void SaveRunDataToFile(FILE* fp, struct Person PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], int RunTime, int RunNumber)
{
	//fprintf(fp, "S;%d;%d;\n", RunNumber, RunTime);

	for (int i = 1; i < MaxRows * 2 + 1; i++)
	{
		for (int j = 0; j < _PersonListIndexes[i]; j++)
		{
			fprintf(fp, "%d;%d;%d;%d;%d;%d\n", RunNumber, RunTime, PersonList[i][j].WalkingSpeed, PersonList[i][j].OrgLuggageCount, PersonList[i][j].StartingDoorID, PersonList[i][j].StepsTaken);
			// fprintf(fp, " P;%d;%d;%d;%d;\n", PersonList[i][j].WalkingSpeed, PersonList[i][j].OrgLuggageCount, PersonList[i][j].StartingDoorID, PersonList[i][j].StepsTaken);
		}
	}
}

void GenerateProgressBar(char PBar[ProgressBarSteps], int Run, int MxRun)
{
	PBar[(int)(((double)Run / (double)MxRun) * ProgressBarSteps)] = 'X';
}

void RunSim(struct Person PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], bool UpdateVisuals)
{
	bool AllSeated = false;
	while (!AllSeated)
	{
		AllSeated = true;
		for (int i = 0; i < MaxRows * 2 + 1; i++)
		{
			for (int j = 0; j < _PersonListIndexes[i]; j++)
			{
				if (!PersonList[i][j].IsSeated)
				{
					AllSeated = false;

					PersonMovement(j, i, PersonList, _PersonListIndexes);

					if (IsPointEqu(PersonList[i][j].CurrentPos, PersonList[i][j].Target) && !PersonList[i][j].BackingUp)
					{
						PersonList[i][j].IsSeated = true;
					}
				}
			}
		}

		if (UpdateVisuals)
		{
			PrintField(PersonList, _PersonListIndexes, BaseFieldData);
		}
	}
}

void PersonMovement(int Index, int RowIndex, struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1])
{
	bool TookAStep = false;
	for (int j = 0; j < _PersonList[RowIndex][Index].WalkingSpeed; j++)
	{
		if (!IsPointEqu(_PersonList[RowIndex][Index].CurrentPos, _PersonList[RowIndex][Index].Target) || _PersonList[RowIndex][Index].BackingUp)
		{
			if (_PersonList[RowIndex][Index].LuggageCount != 0)
			{
				if (_PersonList[RowIndex][Index].CurrentPos.Y == _PersonList[RowIndex][Index].Target.Y && _PersonList[RowIndex][Index].CurrentPos.X == Doors[_PersonList[RowIndex][Index].StartingDoorID].X)
				{
					_PersonList[RowIndex][Index].IsPuttingLuggage = true;
					_PersonList[RowIndex][Index].LuggageCount--;
					if (_PersonList[RowIndex][Index].LuggageCount == 0)
						_PersonList[RowIndex][Index].IsPuttingLuggage = false;

					TookAStep = true;
					break;
				}
			}

			if (!_PersonList[RowIndex][Index].IsPuttingLuggage)
			{
				if (_PersonList[RowIndex][Index].BackingUp)
				{
					if (_PersonList[RowIndex][Index].CurrentPos.X != Doors[_PersonList[RowIndex][Index].StartingDoorID].X)
					{
						struct Point NewPoint = PredictedPointInvX(_PersonList[RowIndex][Index].CurrentPos, _PersonList[RowIndex][Index].Target.X, Doors[_PersonList[RowIndex][Index].StartingDoorID].X);

						if (NewPoint.X == Doors[_PersonList[RowIndex][Index].StartingDoorID].X)
						{
							if (!IsAnyOnPoint(_PersonList, _PersonListIndexes, NewPoint, -1, 0))
							{
								_PersonList[RowIndex][Index].CurrentPos = NewPoint;

								TookAStep = true;
							}
						}
						else
						{
							_PersonList[RowIndex][Index].CurrentPos = NewPoint;

							TookAStep = true;
						}
					}
					else
					{
						if (!IsSeatedFurtherIn(_PersonList, _PersonListIndexes, Index, RowIndex))
						{
							_PersonList[RowIndex][Index].BackingUp = false;
							break;
						}
						else
							SendRowBack(_PersonList, _PersonListIndexes, Index, RowIndex);
					}
				}
				else
				{
					if (RowIndex == 0)
					{
						if (_PersonList[RowIndex][Index].CurrentPos.Y == _PersonList[RowIndex][Index].Target.Y)
						{
							if (_PersonList[RowIndex][Index].Target.X > Doors[_PersonList[RowIndex][Index].StartingDoorID].X)
							{
								MoveToNewArray(_PersonList, _PersonListIndexes, &RowIndex, &Index, _PersonList[RowIndex][Index].Target.Y * 2);
							}
							else
							{
								if (_PersonList[RowIndex][Index].Target.X < Doors[_PersonList[RowIndex][Index].StartingDoorID].X)
								{
									MoveToNewArray(_PersonList, _PersonListIndexes, &RowIndex, &Index, _PersonList[RowIndex][Index].Target.Y * 2 - 1);
								}
							}
						}
					}

					struct Point NewPoint = PredictedPoint(_PersonList[RowIndex][Index].CurrentPos, _PersonList[RowIndex][Index].Target);
					if (!IsAnyOnPoint(_PersonList, _PersonListIndexes, NewPoint, Index, RowIndex))
					{
						_PersonList[RowIndex][Index].CurrentPos = NewPoint;

						TookAStep = true;
					}
					else
						break;
				}
			}
		}
		else
			break;
	}

	if (TookAStep)
	{
		_PersonList[RowIndex][Index].StepsTaken++;
	}
}

void PrintField(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], const char _BaseFieldData[MaxRows][MaxSeatsPrRow])
{
	char FieldData[MaxRows][MaxSeatsPrRow];
	memset(FieldData, 0, sizeof FieldData);

	for (int i = 0; i < MaxRows * 2 + 1; i++)
	{
		for (int j = 0; j < _PersonListIndexes[i]; j++)
		{
			if (FieldData[_PersonList[i][j].CurrentPos.Y][_PersonList[i][j].CurrentPos.X] != '\0')
				FieldData[_PersonList[i][j].CurrentPos.Y][_PersonList[i][j].CurrentPos.X] = MultiPersonChar;
			else
				FieldData[_PersonList[i][j].CurrentPos.Y][_PersonList[i][j].CurrentPos.X] = _PersonList[i][j].PersonCharacter;
		}
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

bool IsAnyOnPoint(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], struct Point TargetPoint, int ExcludeIndex, int RowIndex)
{
	for (int i = 0; i < _PersonListIndexes[RowIndex]; i++)
	{
		if (i != ExcludeIndex && _PersonList[RowIndex][i].StepsTaken != 0)
		{
			if (RowIndex == 0)
			{
				if (!_PersonList[RowIndex][i].IsPuttingLuggage)
				{
					if (IsPointEqu(_PersonList[RowIndex][i].CurrentPos, TargetPoint))
					{
						struct Point NewPoint = PredictedPoint(_PersonList[RowIndex][i].CurrentPos, _PersonList[RowIndex][i].Target);
						if (IsPointEqu(NewPoint, _PersonList[RowIndex][ExcludeIndex].CurrentPos))
						{
							_PersonList[RowIndex][ExcludeIndex].CurrentPos = TargetPoint;
							_PersonList[RowIndex][i].CurrentPos = NewPoint;
							return true;
						}
						if (!_PersonList[RowIndex][i].IsSeated)
							return true;
					}
				}
			}
			else
			{
				if (IsPointEqu(_PersonList[RowIndex][i].CurrentPos, TargetPoint))
				{
					SendRowBack(_PersonList, _PersonListIndexes, ExcludeIndex, RowIndex);
					return true;
				}
			}
		}
	}
	return false;
}

struct Point PredictedPoint(struct Point CurrentPoint, struct Point TargetPoint)
{
	struct Point NewPoint = { CurrentPoint.X, CurrentPoint.Y };
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

struct Point PredictedPointInvX(struct Point _CurrentPos, enum XAxis TargX, enum XAxis DoorX)
{
	struct Point NewPoint = { _CurrentPos.X, _CurrentPos.Y };
	if (TargX > DoorX)
		NewPoint.X -= 1;
	else
		NewPoint.X += 1;

	return NewPoint;
}

bool IsSeatedFurtherIn(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], int ExcludeIndex, int RowIndex)
{
	bool AtLeastOneBackingStill = false;

	for (int i = 0; i < _PersonListIndexes[RowIndex]; i++)
	{
		if (i != ExcludeIndex)
		{
			if (!_PersonList[RowIndex][i].IsSeated)
			{
				if (IsNumberOdd(RowIndex))
				{
					if (_PersonList[RowIndex][i].CurrentPos.X <= _PersonList[RowIndex][ExcludeIndex].Target.X)
					{
						if (_PersonList[RowIndex][i].CurrentPos.X != Doors[_PersonList[RowIndex][ExcludeIndex].StartingDoorID].X)
						{
							AtLeastOneBackingStill = true;
							break;
						}
					}
				}
				else
				{
					if (_PersonList[RowIndex][i].CurrentPos.X >= _PersonList[RowIndex][ExcludeIndex].Target.X)
					{
						if (_PersonList[RowIndex][i].CurrentPos.X != Doors[_PersonList[RowIndex][ExcludeIndex].StartingDoorID].X)
						{
							AtLeastOneBackingStill = true;
							break;
						}
					}
				}
			}
		}
	}

	return AtLeastOneBackingStill;
}

void SendRowBack(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], int ExcludeIndex, int RowIndex)
{
	for (int i = 0; i < _PersonListIndexes[RowIndex]; i++)
	{
		if (i != ExcludeIndex)
		{
			if (!_PersonList[RowIndex][i].BackingUp)
			{
				if (IsNumberOdd(RowIndex))
				{
					if (_PersonList[RowIndex][i].Target.X >= _PersonList[RowIndex][ExcludeIndex].Target.X)
					{
						_PersonList[RowIndex][i].BackingUp = true;
						_PersonList[RowIndex][i].IsSeated = false;
					}
				}
				else
				{
					if (_PersonList[RowIndex][i].Target.X <= _PersonList[RowIndex][ExcludeIndex].Target.X)
					{
						_PersonList[RowIndex][i].BackingUp = true;
						_PersonList[RowIndex][i].IsSeated = false;
					}
				}
			}
		}
	}
}

void MoveToNewArray(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1], int *RowIndex, int *Index, int _NewRowIndex)
{
	int NewIndex = _PersonListIndexes[_NewRowIndex];

	_PersonList[_NewRowIndex][NewIndex] = _PersonList[*RowIndex][*Index];
	_PersonListIndexes[_NewRowIndex]++;

	for (int i = *Index; i < _PersonListIndexes[*RowIndex] - 1; i++)
	{
		_PersonList[*RowIndex][i] = _PersonList[*RowIndex][i + 1];
	}
	_PersonListIndexes[*RowIndex]--;

	*RowIndex = _NewRowIndex;
	*Index = NewIndex;
}

// REMEMBER TO REMOVE LATER!
void ResetToStart(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int _PersonListIndexes[MaxRows * 2 + 1])
{

}