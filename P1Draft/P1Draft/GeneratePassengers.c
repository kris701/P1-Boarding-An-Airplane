#include "GeneratePassengers.h"

void GeneratePassengers(int Count, Person _PersonList[], Map _PlaneMap)
{
    for (int i = 0; i < Count; i++) {
        GeneratePassenger(&(_PersonList[i]), _PlaneMap);
    }

	for (int y = 0; y < _PlaneMap.Height; y++)
	{
		for (int x = 0; x < _PlaneMap.Width; x++)
		{
			MapLocationGet(&_PlaneMap, x, y)->IsTaken = false;
		}
	}

	AssignPassengersToAvailableSeat(Count, _PersonList, _PlaneMap);

	//switch (Index)
	//{
	//    case MI_Random:
	//	    Method_Random(Count, _PersonList, _MDef);
	//	    break;
	//}
}

void GeneratePassenger(Person* Passenger, Map _PlaneMap) {
    Passenger->WalkingSpeed = GenerateWalkSpeed(Passenger);

    Passenger->StartingDoorID = GetStartingDoorID(Passenger, _PlaneMap);
    Passenger->CurrentPos = _PlaneMap.Doors[Passenger->StartingDoorID];
    Passenger->IsSeated = false;
    Passenger->PersonCharacter = 'P';

    Passenger->LuggageCount = GenerateLuggage(Passenger);

    Passenger->IsBackingUp = false;
    Passenger->ShuffleDelay = 0;
    Passenger->CrossDelay = 0;
    Passenger->StepsTaken = 0;

	Passenger->MovedLastTurn = true;
	Passenger->NextMove = SetPoint(0,0);
}

// Read these from config file instead? As well as the SSR.
int GenerateLuggage(const Person* Passenger) {
    return GetRandomNumberRanged(2, 6);
}

// Take the door closet? Or also make that configurable?
int GetStartingDoorID(const Person* Passenger, Map _PlaneMap) {
    return GetRandomNumberRanged(0, _PlaneMap.DoorCount - 1);
}

int GenerateWalkSpeed(const Person* Passenger) {
    return GetRandomNumberRanged(1, 2);
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
			Location* MomentLoc = MapLocationGet(&_PlaneMap, x, y);
			if (MomentLoc->BoardingGroup == BoardingGroup && MomentLoc->IsTaken == 0)
			{
				_Passenger->Target = MomentLoc->Point;
				MomentLoc->IsTaken = 1;
				return true;
			}
		}
	}

	return false;
}
