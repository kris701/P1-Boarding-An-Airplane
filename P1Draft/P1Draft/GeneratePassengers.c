#include "GeneratePassengers.h"

void GeneratePassengers(int Count, Person _PersonList[MaxPersons], MethodIndex Index, MethodDefinition _MDef)
{
	srand(time(NULL));

    for (int i = 0; i < Count; i++) {
        GeneratePassenger(&(_PersonList[i]));
        AssignPassengerToAvailableSeat(_PersonList, i);
    }

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
}

int GenerateLuggage(const Person* Passenger) {
    return GetRandomNumberRanged(2, 6);
}

int GetStartingDoorID(const Person* Passenger) {
    return GetRandomNumberRanged(0, 1);
}

int GenerateWalkSpeed(const Person* Passenger) {
    return GetRandomNumberRanged(3, 4);
}

bool AssignPassengerToAvailableSeat(Person _PassengerList[MaxPersons], int index) {
    _PassengerList[index].Target = Random_GetTargetLocation(_PassengerList, index);
    return true;
}

Point Random_GetTargetLocation(Person _PersonList[MaxPersons], int Index)
{
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