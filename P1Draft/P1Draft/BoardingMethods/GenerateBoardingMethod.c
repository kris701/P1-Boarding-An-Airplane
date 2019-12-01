#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FIRST_CLASS_ROWS 8
#define PEASENT_CLASS_ROWS 3
#define SEATS_PER_ROW 8
#define FIRST_CLASS_SEATS_PER_ROW (SEATS_PER_ROW / 2 + (SEATS_PER_ROW / 2 % 2))
#define BOARDING_GROUP_ROW_AMOUNT 4
#define WITH_WALLS false

typedef enum BoardingMethods{
    BMRandom,
    BMWilma,
    BMFrontToBack,
    BMBackToFront,
    BMSteffenModified,
    BMJanModified,
    BMSteffenPerfect
} BoardingMethods;

typedef enum Classes{
    CFirst,
    CPeasent
} Classes;

typedef struct BoardingInfo {
    BoardingMethods BoardingMethod;
    int CurrentBoardingGroup;
    int PeasentClassBoardingGroupOffset;
    int BoardingGroupRowCounter;
    int SeatCounter;
    Classes CurrentClass;
} BoardingInfo;

BoardingInfo ResetBI(BoardingInfo _BI);
int CreateFirstClassRows(BoardingInfo *_BI, FILE *_FP);
void CreatePeasentClassRows(BoardingInfo *_BI, FILE *_FP);
void GetSeatNumber(BoardingInfo *_BI, int _SeatIndex, char _SeatString[]);
int GetPeasentOffset(BoardingInfo _BI);
int GetSeatForFrontToBack(BoardingInfo *_BI, int _SeatIndex);
int GetSeatForBackToFront(BoardingInfo *_BI, int _SeatIndex);
int GetSeatForSteffenModified(BoardingInfo *_BI, int _SeatIndex);
int GetSeatForJanModified(BoardingInfo *_BI, int _SeatIndex);
int GetSeatForSteffenPerfect(BoardingInfo *_BI, int _SeatIndex);

int main(void) {
    FILE *FP;
    BoardingInfo BI = {BMWilma, 1, 0, 0, 0};
    if (SEATS_PER_ROW % 2 != 0)
        return 1;

    for (int i = 0; i < 7; i++) {
        switch((BoardingMethods) i) {
            case BMRandom:
                FP = fopen("random.txt", "w");
                BI.BoardingMethod = BMRandom;
            break;
            case BMWilma:
                FP = fopen("wilma.txt", "w");
                BI.BoardingMethod = BMWilma;
            break;
            case BMFrontToBack:
                FP = fopen("fronttoback.txt", "w");
                BI.BoardingMethod = BMFrontToBack;
            break;
            case BMBackToFront:
                FP = fopen("backtofront.txt", "w");
                BI.BoardingMethod = BMBackToFront;
            break;
            case BMSteffenModified:
                FP = fopen("steffenmodified.txt", "w");
                BI.BoardingMethod = BMSteffenModified;
            break;
            case BMJanModified:
                FP = fopen("janmodified.txt", "w");
                BI.BoardingMethod = BMJanModified;
            break;
            case BMSteffenPerfect:
                FP = fopen("steffenperfect.txt", "w");
                BI.BoardingMethod = BMSteffenPerfect;
            break;
            default:
                return 1;
            break;
        }
        BI = ResetBI(BI);
        BI.PeasentClassBoardingGroupOffset = CreateFirstClassRows(&BI, FP);
        BI.BoardingGroupRowCounter = 0;
        BI.SeatCounter = 0;
        CreatePeasentClassRows(&BI, FP);
        fclose(FP);
    }

    return 0;
}

BoardingInfo ResetBI(BoardingInfo _BI) {
    _BI.CurrentBoardingGroup = 1;
    _BI.PeasentClassBoardingGroupOffset = 0;
    _BI.BoardingGroupRowCounter = 0;
    _BI.SeatCounter = 0;
    return _BI;
}

int CreateFirstClassRows(BoardingInfo *_BI, FILE *_FP) { 
    int seats = SEATS_PER_ROW / 2, offset = seats % 2;
    char SeatString[10];
    _BI->CurrentClass = CFirst;
    for (int j = 0; j < FIRST_CLASS_ROWS; j++) {  
        for (int i = 1; i <= seats / 2 + seats % 2; i++) {
            GetSeatNumber(_BI, i, SeatString);
            fputs(SeatString, _FP);
            if ((offset == 0 && i == seats / 2 + offset) || i != seats / 2 + offset)
                fputs("-,", _FP);
        }
        fputs("|,", _FP);
        _BI->BoardingGroupRowCounter++;
        for (int i = seats / 2 + seats % 2; i >= 1; i-- ) {
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

void CreatePeasentClassRows(BoardingInfo *_BI, FILE *_FP) {
    int seats = SEATS_PER_ROW / 2;
    char SeatString[10];
    _BI->CurrentClass = CPeasent;
    if (_BI->PeasentClassBoardingGroupOffset == -1)
        _BI->PeasentClassBoardingGroupOffset = 0;
    for (int j = 0; j < PEASENT_CLASS_ROWS; j++) {
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

void GetSeatNumber(BoardingInfo *_BI, int _SeatIndex, char _SeatString[]) {
    switch(_BI->BoardingMethod) {
        case BMRandom:
            sprintf(_SeatString, "%d,", _BI->CurrentClass == CFirst ? 1 : 2);
        break;
        case BMWilma:
            sprintf(_SeatString, "%d,", _SeatIndex + _BI->PeasentClassBoardingGroupOffset);
        break;
        case BMFrontToBack:
            sprintf(_SeatString, "%d,", 
                GetSeatForFrontToBack(_BI, _SeatIndex)
                );
        break;
        case BMBackToFront:
            sprintf(_SeatString, "%d,", 
                GetSeatForBackToFront(_BI, _SeatIndex)
                );
        break;
        case BMSteffenModified:
            sprintf(_SeatString, "%d,", 
                GetSeatForSteffenModified(_BI, _SeatIndex)
                );
        break;    
        case BMJanModified:
            sprintf(_SeatString, "%d,", 
                GetSeatForJanModified(_BI, _SeatIndex)
                );
        break;
        case BMSteffenPerfect:
            sprintf(_SeatString, "%d,", 
                GetSeatForSteffenPerfect(_BI, _SeatIndex)
                );
        break;        
        default:
            sprintf(_SeatString, "%d,", -1);
        break;
    }
}

int GetPeasentOffset(BoardingInfo _BI) {
    switch(_BI.BoardingMethod) {
        case BMRandom:
            return 1;
        break;
        case BMWilma:
            return (SEATS_PER_ROW / 4 + (SEATS_PER_ROW / 2) % 2);
        break;
        case BMFrontToBack:
            return (FIRST_CLASS_ROWS / BOARDING_GROUP_ROW_AMOUNT) + ((FIRST_CLASS_ROWS % BOARDING_GROUP_ROW_AMOUNT == 0) ? 0 : 1);
        break;
        case BMBackToFront:
            return (FIRST_CLASS_ROWS / BOARDING_GROUP_ROW_AMOUNT) + ((FIRST_CLASS_ROWS % BOARDING_GROUP_ROW_AMOUNT == 0) ? 0 : 1);
        break;
        case BMSteffenModified:
            return (2 * FIRST_CLASS_ROWS);
        break;
        case BMJanModified:
            return (FIRST_CLASS_ROWS * FIRST_CLASS_SEATS_PER_ROW);
        break;
        case BMSteffenPerfect:
            return (FIRST_CLASS_ROWS * FIRST_CLASS_SEATS_PER_ROW);
        break;
        default:
            return 0;
        break;
    }
        
}

int GetSeatForFrontToBack(BoardingInfo *_BI, int _SeatIndex) {
    int rowCount = ((_BI->CurrentClass == CFirst) ? FIRST_CLASS_ROWS : PEASENT_CLASS_ROWS);
    return  (1 + 

                _BI->PeasentClassBoardingGroupOffset +

                (_BI->BoardingGroupRowCounter + 
                (_BI->BoardingGroupRowCounter % 2 == 1 ? 0 : 1)) 
                / (2 * BOARDING_GROUP_ROW_AMOUNT));
}

int GetSeatForBackToFront(BoardingInfo *_BI, int _SeatIndex) {
    int rowCount = ((_BI->CurrentClass == CFirst) ? FIRST_CLASS_ROWS : PEASENT_CLASS_ROWS);
    return (_BI->PeasentClassBoardingGroupOffset +
           (rowCount / BOARDING_GROUP_ROW_AMOUNT) + ((rowCount % BOARDING_GROUP_ROW_AMOUNT == 0) ? 0 : 1) -
           ((_BI->BoardingGroupRowCounter + (_BI->BoardingGroupRowCounter % 2 == 1 ? 0 : 1)) / 
           (2 * BOARDING_GROUP_ROW_AMOUNT))


        );
}

int GetSeatForSteffenModified(BoardingInfo *_BI, int _SeatIndex) {
    int rowCount = ((_BI->CurrentClass == CFirst) ? FIRST_CLASS_ROWS : PEASENT_CLASS_ROWS);
    return  (2 * rowCount +
                  _BI->PeasentClassBoardingGroupOffset -
                ((_BI->BoardingGroupRowCounter % 2 == 1) ? rowCount : rowCount / 2) -
                 (_BI->BoardingGroupRowCounter / 4) -
                ((_BI->BoardingGroupRowCounter % 4 == ((rowCount % 2 == 0) ? 2 : 0)) ? rowCount : 0) +
               (((_BI->BoardingGroupRowCounter % 4 == ((rowCount % 2 == 0) ? 1 : 3)) && (_BI->BoardingGroupRowCounter % 2 == 1)) ? rowCount : 0) +
               ((rowCount % 2 == 1) ? (((_BI->BoardingGroupRowCounter % 4 == 1) && (_BI->BoardingGroupRowCounter % 2 == 1)) ? 1 : 0) : 0)) ;
}

int GetSeatForJanModified(BoardingInfo *_BI, int _SeatIndex) {
    int rowCount = ((_BI->CurrentClass == CFirst) ? FIRST_CLASS_ROWS : PEASENT_CLASS_ROWS);
    int SeatsPerRow = ((_BI->CurrentClass == CFirst) ? FIRST_CLASS_SEATS_PER_ROW : SEATS_PER_ROW);
    return (_BI->PeasentClassBoardingGroupOffset +
            rowCount * SeatsPerRow -
            _BI->BoardingGroupRowCounter * (SeatsPerRow / 2) -
            ((SeatsPerRow / 2) - _SeatIndex)
           );
}

int GetSeatForSteffenPerfect(BoardingInfo *_BI, int _SeatIndex) {
    int rowCount = ((_BI->CurrentClass == CFirst) ? FIRST_CLASS_ROWS : PEASENT_CLASS_ROWS);
    int SeatsPerRow = ((_BI->CurrentClass == CFirst) ? FIRST_CLASS_SEATS_PER_ROW : SEATS_PER_ROW);
    return (_BI->PeasentClassBoardingGroupOffset +
            // Sets the maximum seat number to all seats
            rowCount * SeatsPerRow - 
            // Sets the maximum seat number for each column of seats
            (((SeatsPerRow / 2) - _SeatIndex) * rowCount * 2) -
            // Subtracts for each group of rows, as seen below:
            // n   n2   n3   n3   n2   n
            // n   n2   n3   n3   n2   n
            // n-1 n2-1 n3-1 n3-1 n2-1 n-1
            // n-1 n2-1 n3-1 n3-1 n2-1 n-1
            (_BI->BoardingGroupRowCounter / 4) -
            // Subtracts from half the plane layout
            (((_BI->BoardingGroupRowCounter + 1) % 2 == 1) * rowCount / 2) - 
            // Subtracts the prior seats
            (((_BI->BoardingGroupRowCounter - _BI->BoardingGroupRowCounter % 2) % 4 == ((rowCount % 2 == 1) ? 0 : 2)) ? rowCount : 0) +
            // Adds 1 to every second row of half the plane
            ((rowCount % 2 == 1) ? ((_BI->BoardingGroupRowCounter % 4 == 1) ? 1 : 0) : 0)
           );
}