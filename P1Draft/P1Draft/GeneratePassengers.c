#include "GeneratePassengers.h"

void GeneratePassengers(int Count, Person _PersonList[MaxPersons], MethodIndex Index, MethodDefinition _MDef)
{
	srand(time(NULL));

	switch (Index)
	{
	case MI_Random:
		Method_Random(Count, _PersonList, _MDef);
		break;
	}
}

void Method_Random(int Count, Person _PersonList[MaxPersons], MethodDefinition _MDef)
{
	for (int i = 0; i < Count; i++)
	{
		int DoorIndex = GetValueFromFunction(_MDef.MinDoorChoice, _MDef.MaxDoorChoice, _MDef.DoorFunction);
		int LuggageCount = GetValueFromFunction(_MDef.MinLuggage, _MDef.MaxLuggage, _MDef.LuggageFunction);
		Point NewTarget = Random_GetTargetLocation(_PersonList, i);

		Person NewPerson = {
			GetValueFromFunction(_MDef.MinWalkSpeed, _MDef.MaxWalkSpeed, _MDef.WalkSpeedFunction),
			Doors[DoorIndex],
			NewTarget,
			false,
			DoorIndex,
			'P',
			LuggageCount,
			LuggageCount,
			false,
			0,
			0,
			i
		};
		_PersonList[i] = NewPerson;
	}
}

int GetValueFromFunction(int Min, int Max, FunctionDefinition _FDef)
{
	int X = GetRandomNumberRanged(Min, Max);
	switch (_FDef.Type)
	{
	case Line:
		return (int)(_FDef.a * X + _FDef.b);
		break;
	case Exp:
		return (int)(pow(X, _FDef.a) + _FDef.b * X + _FDef.c);
		break;
	}
	return 0;
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
					if (IsPointEqu(_PersonList[i].Target, NewTarget))
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

/* Data from: https://journals.plos.org/plosone/article?id=10.1371/journal.pone.0023299 */
int GetWalkingSpeed(int age) {
    if (age >= 60) {
        return 121;
    }
    else if(age >= 50){
        return 123;
    }
    else if (age >= 40) {
        return 126;
    }
    else if (age >= 30) {
        return 126;
    }
    else {
        return 134;
    }
}
