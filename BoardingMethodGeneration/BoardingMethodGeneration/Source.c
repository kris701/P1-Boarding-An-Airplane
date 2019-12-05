#include "Source.h"

int main(void) {
	FILE* FP = NULL;
	BoardingInfo BI = { {0, 0, 0, 0, 0, 0, 0}, BMWilma, 1, 0, 0, 0 };

	if (!OpenConfigFile(&BI, FP))
		return 0;
	// The program can't handle uneven amount of seats
	if (BI.PlaneInfo.SeatsPerRow % 2 != 0) {
		printf("Uneven amount of seats per row\nShutting down\n");
		return 0;
	}

	for (int i = 0; i < 7; i++) {
		FP = SetBoardingMethod(&BI, FP, i);
		BI = ResetBI(BI);
		if (BI.PlaneInfo.UpperDoor)
			CreateDoorRow(&BI, FP);
		BI.PeasentClassBoardingGroupOffset = CreateFirstClassRows(&BI, FP);
		BI.BoardingGroupRowCounter = 0;
		BI.SeatCounter = 0;
		CreatePeasentClassRows(&BI, FP);
		if (BI.PlaneInfo.LowerDoor)
			CreateDoorRow(&BI, FP);
		fclose(FP);
	}

	return 0;
}

int OpenConfigFile(BoardingInfo* _BI, FILE* _FP) {
	fopen_s(&_FP, "config.ini", "r");

	if (_FP == NULL) {
		printf("Missing config file\nShutting down\n");
		return 0;
	}
	ReadConfigFile(_BI, _FP);
	return 1;
}

void ReadConfigFile(BoardingInfo* _BI, FILE* _FP) {
	char str[258];
	char* substring = "";
	char* delimiter = "=";
	for (int i = 0; i < 13; i++) {
		// If there is text, which should really always be the case
		if (fgets(str, 258, _FP) != NULL) {
			// If the line isn't a comment
			if (str[0] != ';' && str[0]!='\n') {
				if (strstr(str, "first_class_rows")) {
					substring = strchr(str, '=') + 1;
					_BI->PlaneInfo.FirstClassRows = atoi(substring);
				}
				else if (strstr(str, "economy_class_rows")) {
					substring = strchr(str, '=') + 1;
					_BI->PlaneInfo.PeasentClassRows = atoi(substring);
				}
				else if (strstr(str, "seats_per_row")) {
					substring = strchr(str, '=') + 1;
					_BI->PlaneInfo.SeatsPerRow = atoi(substring);
					_BI->PlaneInfo.FirstClassSeatsPerRow = (_BI->PlaneInfo.SeatsPerRow / 2 + (_BI->PlaneInfo.SeatsPerRow / 2 % 2));
				}
				else if (strstr(str, "boarding_group_size")) {
					substring = strchr(str, '=') + 1;
					_BI->PlaneInfo.BoardingGroupRowCount = atoi(substring);
				}
				else if (strstr(str, "upper_door")) {
					substring = strchr(str, '=') + 1;
					_BI->PlaneInfo.UpperDoor = atoi(substring);
				}
				else if (strstr(str, "lower_door")) {
					substring = strchr(str, '=') + 1;
					_BI->PlaneInfo.LowerDoor = atoi(substring);
				}
			}
		}
	}
}

BoardingInfo ResetBI(BoardingInfo _BI) {
	_BI.CurrentBoardingGroup = 1;
	_BI.PeasentClassBoardingGroupOffset = 0;
	_BI.BoardingGroupRowCounter = 0;
	_BI.SeatCounter = 0;
	return _BI;
}

void CreateDoorRow(BoardingInfo* _BI, FILE* _FP) {
	for (int i = 0; i < _BI->PlaneInfo.SeatsPerRow; i++) {
		if (i == _BI->PlaneInfo.SeatsPerRow / 2)
			fputs("D,", _FP);
		fputs("-,", _FP);
	}
	fputs("\n", _FP);
}

int CreateFirstClassRows(BoardingInfo* _BI, FILE* _FP) {
	int seats = _BI->PlaneInfo.SeatsPerRow / 2, offset = seats % 2;
	char SeatString[10] = { '\0' };
	_BI->CurrentClass = CFirst;

	for (int j = 0; j < _BI->PlaneInfo.FirstClassRows; j++) {
		for (int i = 1; i <= seats / 2 + seats % 2; i++) {
			GetSeatNumber(_BI, i, SeatString);
			fputs(SeatString, _FP);
			// Adds a empty seat if aplicable
			if ((offset == 0 && i == seats / 2 + offset) || i != seats / 2 + offset)
				fputs("-,", _FP);
		}
		fputs("|,", _FP);
		_BI->BoardingGroupRowCounter++;
		for (int i = seats / 2 + seats % 2; i >= 1; i--) {
			// Adds a empty seat if aplicable
			if ((offset == 0 && i == seats / 2 + offset) || i != seats / 2 + offset)
				fputs("-,", _FP);
			GetSeatNumber(_BI, i, SeatString);
			fputs(SeatString, _FP);
		}
		_BI->BoardingGroupRowCounter++;
		fputs("\n", _FP);
	}
	return GetPeasentOffset(*_BI);
}

void CreatePeasentClassRows(BoardingInfo* _BI, FILE* _FP) {
	int seats = _BI->PlaneInfo.SeatsPerRow / 2;
	char SeatString[10];
	_BI->CurrentClass = CPeasent;
	if (_BI->PeasentClassBoardingGroupOffset == -1)
		_BI->PeasentClassBoardingGroupOffset = 0;
	for (int j = 0; j < _BI->PlaneInfo.PeasentClassRows; j++) {
		for (int i = 1; i <= seats; i++) {
			GetSeatNumber(_BI, i, SeatString);
			fputs(SeatString, _FP);
		}
		fputs("|,", _FP);
		_BI->BoardingGroupRowCounter++;
		for (int i = seats; i > 0; i--) {
			GetSeatNumber(_BI, i, SeatString);
			fputs(SeatString, _FP);
		}
		_BI->BoardingGroupRowCounter++;
		fputs("\n", _FP);
	}
}




