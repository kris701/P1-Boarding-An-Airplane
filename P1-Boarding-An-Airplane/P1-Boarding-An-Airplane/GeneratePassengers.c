#include "GeneratePassengers.h"

// A function to generate all passengers with use of some of the other functions
void GeneratePassengers(int Count, Person _PersonList[], Map _PlaneMap, BasicSimulationRules _BaseRules)
{
	ResetPassengerListAndMap(Count, &_PersonList, &_PlaneMap, _BaseRules);

	AssignPassengersToAvailableSeat(Count, _PersonList, _PlaneMap);

	ScramblePassengers(_PersonList, Count);

	AssignPassengerToDoor(Count, _PersonList, _PlaneMap, _BaseRules);
}

void ResetPassengerListAndMap(int Count, Person* _PersonList[], Map* _PlaneMap, BasicSimulationRules _BaseRules)
{
	for (int i = 0; i < Count; i++) {
		InitialisePassenger(&(*_PersonList)[i], (*_PlaneMap), _BaseRules);
	}

	for (int y = 0; y < (*_PlaneMap).Height; y++)
	{
		for (int x = 0; x < (*_PlaneMap).Width; x++)
		{
			GetMapLocation(_PlaneMap, x, y)->IsTaken = false;
		}
	}
}

// A function to initialise passenger. It uses some other functions to generate diffrent parameters for each passenger
void InitialisePassenger(Person* Passenger, Map _PlaneMap, BasicSimulationRules _BaseRules) {
	Passenger->WalkingSpeed = GenerateWalkSpeed(_BaseRules);

    Passenger->StartingDoorID = 0;
	Passenger->CurrentPos = SetPoint(0,0);
    Passenger->IsSeated = false;
    Passenger->PersonCharacter = '.';

    Passenger->LuggageCount = GenerateLuggage(_BaseRules);

    Passenger->IsBackingUp = false;
    Passenger->SeatInterferenceDelay = 0;
    Passenger->CrossDelay = 0;

	Passenger->MovedLastTurn = true;
	Passenger->NextMove = SetPoint(0,0);
}

// A function to genereate how much luggage a passenger has with them
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

	printf("Error, Luggage generation did not add up!\n");
	Sleep(500);

    return 0;
}

// A function to get the ID on the nearest starting door
int GetNearestStartingDoorID(Person* Passenger, Map _PlaneMap) 
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

// A function generate walking speed for a passenger
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

	printf("Error, Walking speed generation did not add up!\n");
	Sleep(500);

	return 1;
}

// A function that uses our to other assign door functions to determine which door to start at
void AssignPassengerToDoor(int Count, Person _PassengerList[], Map _PlaneMap, BasicSimulationRules _BaseRules)
{
	if (_BaseRules.AssignToNearestDoor)
		AssignPassengerToNearestDoor(Count, _PassengerList, _PlaneMap);
	else
		AssignPassengerToRandomDoor(Count, _PassengerList, _PlaneMap);
}

// A function to assign a passenger to the nearest door
void AssignPassengerToNearestDoor(int Count, Person _PassengerList[], Map _PlaneMap)
{
	for (int i = 0; i < Count; i++) {
		_PassengerList[i].StartingDoorID = GetNearestStartingDoorID(&_PassengerList[i], _PlaneMap);
		_PassengerList[i].CurrentPos = _PlaneMap.Doors[_PassengerList[i].StartingDoorID];
	}
}

// A function to assign a passenger to a random door to start at
void AssignPassengerToRandomDoor(int Count, Person _PassengerList[], Map _PlaneMap)
{
	for (int i = 0; i < Count; i++) {
		_PassengerList[i].StartingDoorID = GetRandomNumberRanged(0, _PlaneMap.DoorCount - 1);
		_PassengerList[i].CurrentPos = _PlaneMap.Doors[_PassengerList[i].StartingDoorID];
	}
}

// A function to assign a passenger to an available seat
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

// A function to assign a seat to a passenger based on his boarding group
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

// A function to scramble passengers
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