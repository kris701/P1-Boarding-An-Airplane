#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FIRST_CLASS_ROWS 5
#define PEASENT_CLASS_ROWS 21
#define SEAT_AMOUT 6
#define BOARDING_GROUP_ROW_AMOUNT 4
#define WITH_WALLS false

typedef enum BoardingMethods{
    BMRandom,
    BMWilma,
    BMFrontToBack,
    BMBackToFront,
    BMJanModified,
    BMSteffenModified
} BoardingMethods;


typedef struct BoardingInfo {
    BoardingMethods BoardingMethod;
    int CurrentBoardingGroup;
    int PeasentClassBoardingGroupOffset;
    int BoardingGroupRowCounter;
} BoardingInfo;

BoardingInfo ResetBI(BoardingInfo _BI);
int CreateFirstClassRows(BoardingInfo *_BI, FILE *_FP);
void CreatePeasentClassRows(BoardingInfo *_BI, FILE *_FP);
void GetSeatNumber(BoardingInfo *_BI, int _SeatIndex, char _SeatString[]);
int GetPeasentClassBoardingOffsetForAfterCreatingFirstClass(BoardingInfo _BI);

int main(void) {
    FILE *FP;
    BoardingInfo BI = {BMWilma, 1, 0, 0};
    if (SEAT_AMOUT % 2 != 0)
        return 1;

    for (int i = 0; i < 6; i++) {
        switch((BoardingMethods) i) {
            case BMRandom:
                FP = fopen("random.txt", "w");
                BI.BoardingMethod = BMRandom;
                BI = ResetBI(BI);
            break;
            case BMWilma:
                FP = fopen("wilma.txt", "w");
                BI.BoardingMethod = BMWilma;
                BI = ResetBI(BI);
            break;
            case BMFrontToBack:
                FP = fopen("fronttoback.txt", "w");
                BI.BoardingMethod = BMFrontToBack;
                BI = ResetBI(BI);
            break;
            case BMBackToFront:
                FP = fopen("backtofront.txt", "w");
                BI.BoardingMethod = BMBackToFront;
                BI = ResetBI(BI);
                BI.CurrentBoardingGroup = 1 + (FIRST_CLASS_ROWS + PEASENT_CLASS_ROWS) / BOARDING_GROUP_ROW_AMOUNT + FIRST_CLASS_ROWS % BOARDING_GROUP_ROW_AMOUNT;
            break;
            case BMJanModified:
                FP = fopen("janmodified.txt", "w");
                BI.BoardingMethod = BMJanModified;
                BI = ResetBI(BI);
                BI.CurrentBoardingGroup = 2 * (FIRST_CLASS_ROWS);
            break;
            case BMSteffenModified:
                FP = fopen("steffenmodified.txt", "w");
                BI.BoardingMethod = BMSteffenModified;
                BI = ResetBI(BI);
                BI.CurrentBoardingGroup = 2 * (FIRST_CLASS_ROWS);
            break;
            default:
                return 1;
            break;
        }
        BI.PeasentClassBoardingGroupOffset = CreateFirstClassRows(&BI, FP);
        CreatePeasentClassRows(&BI, FP);
        fclose(FP);
    }

    return 0;
}

BoardingInfo ResetBI(BoardingInfo _BI) {
    _BI.CurrentBoardingGroup = 1;
    _BI.PeasentClassBoardingGroupOffset = 0;
    _BI.BoardingGroupRowCounter = 0;
    return _BI;
}

int CreateFirstClassRows(BoardingInfo *_BI, FILE *_FP) { 
    int seats = SEAT_AMOUT / 2, offset = seats % 2;
    char SeatString[10];
    for (int j = 0; j < FIRST_CLASS_ROWS; j++) {
        if (WITH_WALLS) 
            fputs("|,", _FP);
        
        for (int i = 1; i <= seats / 2 + seats % 2; i++) {
            GetSeatNumber(_BI, i, SeatString);
            fputs(SeatString, _FP);
            if ((offset == 0 && i == seats / 2 + offset) || i != seats / 2 + offset)
                fputs("-,", _FP);
        }
        fputs("-,", _FP);
        if (_BI->BoardingMethod == BMJanModified)
            _BI->CurrentBoardingGroup--;
        for (int i = seats / 2 + seats % 2; i >= 1; i-- ) {
            if ((offset == 0 && i == seats / 2 + offset) || i != seats / 2 + offset)
                fputs("-,", _FP);
            GetSeatNumber(_BI, i, SeatString);
            fputs(SeatString, _FP);
        }
        if (_BI->BoardingMethod == BMJanModified)
            _Bå I->CurrentBoardingGroup--;
        else if (_BI->BoardingMethod == BMSteffenModified)
            _BI->CurrentBoardingGroup -=2;


        if (WITH_WALLS) 
            fputs("|", _FP);
        fputs("\n", _FP);
        if (_BI->BoardingMethod == BMFrontToBack || _BI->BoardingMethod == BMBackToFront) {
            _BI->BoardingGroupRowCounter++;
            if (_BI->BoardingGroupRowCounter == BOARDING_GROUP_ROW_AMOUNT || j == FIRST_CLASS_ROWS - 1) {
                _BI->BoardingGroupRowCounter = 0;
                if (_BI->BoardingMethod == BMFrontToBack)
                    _BI->CurrentBoardingGroup++;
                else
                    _BI->CurrentBoardingGroup--;
            }
        }

    }
    return GetPeasentClassBoardingOffsetForAfterCreatingFirstClass(*_BI);
}

void CreatePeasentClassRows(BoardingInfo *_BI, FILE *_FP) {
    int seats = SEAT_AMOUT / 2;
    char SeatString[10];
    
    if (_BI->BoardingMethod == BMJanModified)
        _BI->CurrentBoardingGroup = 2 * PEASENT_CLASS_ROWS;

    for (int j = 0; j < PEASENT_CLASS_ROWS; j++) {
        if (WITH_WALLS) 
            fputs("|,", _FP);
        
        for (int i = 1; i <= seats; i++) {
            GetSeatNumber(_BI, i, SeatString);
            fputs(SeatString, _FP);
        }
        if (_BI->BoardingMethod == BMJanModified)
            _BI->CurrentBoardingGroup--;
        fputs("-,", _FP);
        for (int i = seats; i > 0; i--) {
            GetSeatNumber(_BI, i, SeatString);
            fputs(SeatString, _FP);
        }
        if (_BI->BoardingMethod == BMJanModified)
            _BI->CurrentBoardingGroup--;    

        if (WITH_WALLS) 
            fputs("|", _FP);
        fputs("\n", _FP);

        if (_BI->BoardingMethod == BMFrontToBack || _BI->BoardingMethod == BMBackToFront) {
            _BI->BoardingGroupRowCounter++;
            if (_BI->BoardingGroupRowCounter == BOARDING_GROUP_ROW_AMOUNT) {
                _BI->BoardingGroupRowCounter = 0;
                if (_BI->BoardingMethod == BMFrontToBack)
                    _BI->CurrentBoardingGroup++;
                else
                    _BI->CurrentBoardingGroup--;
            }
        }
    }
}

void GetSeatNumber(BoardingInfo *_BI, int _SeatIndex, char _SeatString[]) {
    switch(_BI->BoardingMethod) {
        case BMRandom:
            sprintf(_SeatString, "%d,", 1 + _BI->PeasentClassBoardingGroupOffset);
        break;
        case BMWilma:
            sprintf(_SeatString, "%d,", _SeatIndex + _BI->PeasentClassBoardingGroupOffset);
        break;
        case BMFrontToBack:
            sprintf(_SeatString, "%d,", _BI->CurrentBoardingGroup);
        break;
        case BMBackToFront:
            sprintf(_SeatString, "%d,", _BI->CurrentBoardingGroup);
        break;
        case BMJanModified:
            sprintf(_SeatString, "%d,", _BI->CurrentBoardingGroup);
        break;
        case BMSteffenModified:
            sprintf(_SeatString, "%d,", _BI->CurrentBoardingGroup);
        break;
        default:
            sprintf(_SeatString, "%d,", -1);
        break;
    }
}

int GetPeasentClassBoardingOffsetForAfterCreatingFirstClass(BoardingInfo _BI) {
    switch(_BI.BoardingMethod) {
        case BMRandom:
            return 1;
        break;
        case BMWilma:
            return (SEAT_AMOUT / 4 + (SEAT_AMOUT / 2) % 2);
        break;
        case BMFrontToBack:
            return 0;
        break;
        case BMBackToFront:
            return 0;
        break;
        default:
            return 0;
        break;
    }
        
}