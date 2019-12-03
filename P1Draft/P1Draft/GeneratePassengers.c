#include "GeneratePassengers.h"

void GeneratePassengers(int Count, Person _PersonList[MaxPersons], Map map)
{
	srand(time(NULL));

    for (int i = 0; i < Count; i++) {
        GeneratePassenger(&(_PersonList[i]));
    }

	AssignPassengersToAvailableSeat(Count, _PersonList, map);

	//switch (Index)
	//{
	//    case MI_Random:
	//	    Method_Random(Count, _PersonList, _MDef);
	//	    break;
	//}
}

void GeneratePassenger(Person* Passenger) {  
    Passenger->WalkingSpeed = GenerateWalkSpeed(Passenger);

    Passenger->StartingDoorID = GetStartingDoorID(Passenger);
    Passenger->CurrentPos = Doors[Passenger->StartingDoorID];
    Passenger->IsSeated = false;
    Passenger->PersonCharacter = 'P';

    Passenger->OrgLuggageCount = GenerateLuggage(Passenger);
    Passenger->LuggageCount = Passenger->OrgLuggageCount;

    Passenger->IsBackingUp = false;
    Passenger->ShuffleDelay = 0;
    Passenger->CrossDelay = 0;
    Passenger->StepsTaken = 0;

	Passenger->MovedLastTurn = true;
	Passenger->NextMove = SetPoint(0,0);
}

int GenerateLuggage(const Person* Passenger) {
    return GetRandomNumberRanged(2, 6);
}

int GetStartingDoorID(const Person* Passenger) {
    return GetRandomNumberRanged(0, 1);
}

int GenerateWalkSpeed(const Person* Passenger) {
    return GetRandomNumberRanged(1, 2);
}

bool AssignPassengersToAvailableSeat(int Count, Person _PassengerList[MaxPersons], Map map) {
	int boardingGroup = 1;
	for (int i = 0; i < Count; i++)
	{
		while (!AssignSeatByBoardinggroup(boardingGroup, &_PassengerList[i], map))
		{
			boardingGroup++;
		}
	}
    return true;
}

bool AssignSeatByBoardinggroup(int boardingGroup, Person* passenger, Map map)
{
	for (int y = 0; y < map.Height; y++)
	{
		for (int x = 0; x < map.Width; x++)
		{
			if (map.Locations[y][x].BoardingGroup == boardingGroup && map.Locations[y][x].IsTaken == 0)
			{
				passenger->Target = map.Locations[y][x].Point;
				map.Locations[y][x].IsTaken = 1;
				return true;
			}
		}
	}

	return false;
}

Point Random_GetTargetLocation(Person _PersonList[MaxPersons], int Index) {
	Point NewTarget = { 0, 0 };
	bool FoundAvailable = true;
	while (FoundAvailable)
	{
		NewTarget = SetPoint(GetRandomNumberRanged(0, MaxSeatsPrRow - 1), GetRandomNumberRanged(1, MaxRows - 2));

		FoundAvailable = false;
		for (int i = 0; i < Index + 1; i++)
		{
			if (BaseFieldData[NewTarget.Y][NewTarget.X] != SeatChar)
			{
				FoundAvailable = true;
				break;
			}
			else
			{
				if (i != Index)
				{
					if (IsPointEqual(_PersonList[i].Target, NewTarget))
					{
						FoundAvailable = true;
						break;
					}
				}
			}
		}
	}

	return NewTarget;
}



void SortPointsAccordingToMethod(Point Points[], FILE* methodFile) {    
    /* Qsort with comp based on filecontents */

    /*
        Look through file, find number of boarding groups
        Allocate 1 int for each boarding group
        Count how many seats for each group
        Go through the seats and assign them to passengers from start to end
        Free array
    */

    /*
    Generate map from file, then pass map to this function
    */



}












