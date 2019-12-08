#include "GeneratePassengers.h"

void GeneratePassengers(int Count, Person _PersonList[], Map _PlaneMap, BasicSimulationRules _BaseRules)
{
    for (int i = 0; i < Count; i++) {
        GeneratePassenger(&(_PersonList[i]), _PlaneMap, _BaseRules);
    }

	for (int y = 0; y < _PlaneMap.Height; y++)
	{
		for (int x = 0; x < _PlaneMap.Width; x++)
		{
			GetMapLocation(&_PlaneMap, x, y)->IsTaken = false;
		}
	}

	AssignPassengersToAvailableSeat(Count, _PersonList, _PlaneMap);

	ScramblePassengers(_PersonList, Count);

	AssignPassengerToNearestDoor(Count, _PersonList, _PlaneMap);
}

void GeneratePassenger(Person* Passenger, Map _PlaneMap, BasicSimulationRules _BaseRules) {
	Passenger->WalkingSpeed = GenerateWalkSpeed(_BaseRules);

    Passenger->StartingDoorID = 0;
	Passenger->CurrentPos = SetPoint(0,0);
    Passenger->IsSeated = false;
    Passenger->PersonCharacter = 'P';

    Passenger->LuggageCount = GenerateLuggage(_BaseRules);

    Passenger->IsBackingUp = false;
    Passenger->ShuffleDelay = 0;
    Passenger->CrossDelay = 0;

	Passenger->MovedLastTurn = true;
	Passenger->NextMove = SetPoint(0,0);
}

int GenerateLuggage(BasicSimulationRules _BaseRules)
{
	int ChanceValue = GetRandomNumberRanged(1, 100);
	int ChanceOffset = 0;
	for (int i = 0; i < _BaseRules.LuggageGenerationValuesLength; i++)
	{
		if (ChanceValue <= (_BaseRules.LuggageGenerationValues[i].Possibility + ChanceOffset))
			return _BaseRules.LuggageGenerationValues[i].Value;
		ChanceOffset += _BaseRules.LuggageGenerationValues[i].Possibility;
	}

    return 0;
}

int GetStartingDoorID(Person* Passenger, Map _PlaneMap) 
{
	int MinLength = 9999;
	int TargetIndex = 0;
	for (int i = 0; i < _PlaneMap.DoorCount; i++)
	{
		if (abs(_PlaneMap.Doors[i].Y - Passenger->Target.Y) < MinLength)
		{
			MinLength = abs(_PlaneMap.Doors[i].Y - Passenger->Target.Y);
			TargetIndex = i;
		}
	}
	return TargetIndex;
}

int GenerateWalkSpeed(BasicSimulationRules _BaseRules)
{
	int ChanceValue = GetRandomNumberRanged(1, 100);
	int ChanceOffset = 0;
	for (int i = 0; i < _BaseRules.WalkingspeedGenerationValuesLength; i++)
	{
		if (ChanceValue <= (_BaseRules.WalkingspeedGenerationValues[i].Possibility + ChanceOffset))
			return _BaseRules.WalkingspeedGenerationValues[i].Value;
		ChanceOffset += _BaseRules.WalkingspeedGenerationValues[i].Possibility;
	}

	return 0;
}

void AssignPassengerToNearestDoor(int Count, Person _PassengerList[], Map _PlaneMap)
{
	for (int i = 0; i < Count; i++) {
		_PassengerList[i].StartingDoorID = GetStartingDoorID(&_PassengerList[i], _PlaneMap);
		_PassengerList[i].CurrentPos = _PlaneMap.Doors[_PassengerList[i].StartingDoorID];
	}
}

bool AssignPassengersToAvailableSeat(int Count, Person _PassengerList[], Map _PlaneMap) {
	int boardingGroup = 1;
	for (int i = 0; i < Count; i++)
	{
		while (!AssignSeatByBoardinggroup(boardingGroup, &_PassengerList[i], _PlaneMap))
		{
			boardingGroup++;
		}
	}
    return true;
}

bool AssignSeatByBoardinggroup(int BoardingGroup, Person* _Passenger, Map _PlaneMap)
{
	for (int y = 0; y < _PlaneMap.Height; y++)
	{
		for (int x = 0; x < _PlaneMap.Width; x++)
		{
			Location* TempLoc = GetMapLocation(&_PlaneMap, x, y);
			if (TempLoc->BoardingGroup == BoardingGroup && TempLoc->IsTaken == false)
			{
				_Passenger->Target = TempLoc->Point;
				_Passenger->BoardingGroup = BoardingGroup;
				TempLoc->IsTaken = true;
				return true;
			}
		}
	}

	return false;
}

void ScramblePassengers(Person _PassengerList[], int ArrayLength)
{
	int StartIndex = 0;
	int EndIndex = 1;

	while (StartIndex < ArrayLength)
	{
		if (_PassengerList[EndIndex].BoardingGroup != _PassengerList[StartIndex].BoardingGroup)
		{
			if (EndIndex - StartIndex > 1)
			{
				for (int i = StartIndex; i < EndIndex - 1; i++)
				{
					int RandValue = GetRandomNumberRanged(i + 1, EndIndex - 1);
					Person TmpPerson = _PassengerList[i];
					_PassengerList[i] = _PassengerList[RandValue];
					_PassengerList[RandValue] = TmpPerson;
				}
			}

			StartIndex = EndIndex;
		}

		EndIndex++;
	}
}