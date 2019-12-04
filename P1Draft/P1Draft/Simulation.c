#include "Simulation.h"

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

void PrintField(Person** _PassengerLocationMatrix[], Map _PlaneMap)
{
	system("cls");

	for (int y = 0; y < _PlaneMap.Height - 1; y++)
	{
		printf("| ");
		for (int x = 0; x < _PlaneMap.Width; x++)
		{
			if (_PassengerLocationMatrix[y][x] != NULL)
				printf("%-*c", _PlaneMap.LongestDigit, _PassengerLocationMatrix[y][x]->PersonCharacter);
			else
			{
				Location tmpLocation = *GetMapLocation(&_PlaneMap, x, y);
				switch (tmpLocation.BoardingGroup) {
				case BoardingGroup_Door:      printf("%-*c", _PlaneMap.LongestDigit, 'D'); break;
				case BoardingGroup_Walkway:   printf("%-*c", _PlaneMap.LongestDigit, '|'); break;
				case BoardingGroup_Padding:   printf("%-*s", _PlaneMap.LongestDigit, ""); break;
				case BoardingGroup_Undefined: printf("%-*c", _PlaneMap.LongestDigit, 'U'); break;
				default:
					printf("%-*d", _PlaneMap.LongestDigit, tmpLocation.BoardingGroup);
					break;
				}
			}
		}
		printf("|\n");
	}
}

bool IsAnyOnPoint(Person** _PassengerLocationMatrix[], Person* _Person, Map _PlaneMap, BasicSimulationRules _BaseRules)
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

void SendRowBack(Person** _PassengerLocationMatrix[], Person* _Person, Map _PlaneMap, BasicSimulationRules _BaseRules)
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