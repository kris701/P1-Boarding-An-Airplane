#include "GeneratePassengers.h"

void GeneratePassengers(int Count, struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], enum MethodIndex Index, struct MethodDefinition _MDef)
{
	srand(time(NULL));

	switch (Index)
	{
	case MI_Random:
		Method_Random(Count, _PersonList, _MDef);
		break;
	}
}

void Method_Random(int Count, struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], struct MethodDefinition _MDef)
{
	for (int i = 0; i < Count; i++)
	{
		int DoorIndex = GetValueFromFunction(_MDef.MinDoorChoice, _MDef.MaxDoorChoice, _MDef.DoorFunction);
		int LuggageCount = GetValueFromFunction(_MDef.MinLuggage, _MDef.MaxLuggage, _MDef.LuggageFunction);
		struct Point NewTarget = Random_GetTargetLocation(_PersonList, i);

		struct Person NewPerson = {
			GetValueFromFunction(_MDef.MinWalkSpeed, _MDef.MaxWalkSpeed, _MDef.WalkSpeedFunction),
			Doors[DoorIndex],
			NewTarget,
			false,
			DoorIndex,
			'P',
			LuggageCount,
			LuggageCount,
			false,
			false,
			0,
			i
		};
		_PersonList[0][i] = NewPerson;
	}
}

int GetValueFromFunction(int Min, int Max, struct FunctionDefinition _FDef)
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

struct Point Random_GetTargetLocation(struct Person _PersonList[MaxRows * 2 + 1][MaxPersons], int Index)
{
	struct Point NewTarget = { 0, 0 };
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
					if (IsPointEqu(_PersonList[0][i].Target, NewTarget))
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