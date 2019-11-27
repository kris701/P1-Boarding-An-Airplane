#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FIRST_CLASS_ROWS 5
#define PEASENT_CLASS_ROWS 21
#define SEAT_AMOUT 6
#define BOARDING_GROUP_CHUNK_SIZE 1
#define WITH_WALLS false

typedef enum BoardingMethods{
    BMRandom,
    BMWilma,
    BMFrontToBack,
    BMBackToFront
} BoardingMethods;


typedef struct BoardingInfo {
    BoardingMethods BoardingMethod;
    int CurrentBoardingGroup;
    int PeasentClassBoardingGroupOffset;
} BoardingInfo;


int CreateFirstClassRows(BoardingInfo _BI, FILE *_FP);
void CreatePeasentClassRows(BoardingInfo _BI, FILE *_FP);
void GetSeatNumber(BoardingInfo _BI, int _SeatIndex, char _SeatString[]);
int GetPeasentClassBoardingOffsetForAfterCreatingFirstClass(BoardingInfo _BI);

int main(void) {
    FILE *FP;
    BoardingInfo BI = {BMWilma, 1, 0};
    if (SEAT_AMOUT % 2 != 0)
        return 1;

    for (int i = 0; i < 2; i++) {
        switch((BoardingMethods) i) {
            case BMRandom:
                FP = fopen("random.txt", "w");
            break;
            case BMWilma:
                FP = fopen("wilma.txt", "w");
            break;
        }
        BI.PeasentClassBoardingGroupOffset = CreateFirstClassRows(BI, FP);
        CreatePeasentClassRows(BI, FP);
        fclose(FP);
    }

    return 0;
}

int CreateFirstClassRows(BoardingInfo _BI, FILE *_FP) { 
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
        for (int i = seats / 2 + seats % 2; i >= 1; i-- ) {
            if ((offset == 0 && i == seats / 2 + offset) || i != seats / 2 + offset)
                fputs("-,", _FP);
            GetSeatNumber(_BI, i, SeatString);
            fputs(SeatString, _FP);
            
        }


        if (WITH_WALLS) 
            fputs("|", _FP);
        fputs("\n", _FP);
    }
    return GetPeasentClassBoardingOffsetForAfterCreatingFirstClass(_BI);
}

void CreatePeasentClassRows(BoardingInfo _BI, FILE *_FP) {
    int seats = SEAT_AMOUT / 2;
    char SeatString[10];
    
    for (int j = 0; j < PEASENT_CLASS_ROWS; j++) {
        if (WITH_WALLS) 
            fputs("|,", _FP);
        
        for (int i = 1; i <= seats; i++) {
            GetSeatNumber(_BI, i, SeatString);
            fputs(SeatString, _FP);
        }
        fputs("-,", _FP);
        for (int i = seats; i > 0; i--) {
            GetSeatNumber(_BI, i, SeatString);
            fputs(SeatString, _FP);
        }

        if (WITH_WALLS) 
            fputs("|", _FP);
        fputs("\n", _FP);
    }
}

void GetSeatNumber(BoardingInfo _BI, int _SeatIndex, char _SeatString[]) {
    switch(_BI.BoardingMethod) {
        case BMRandom:
        sprintf(_SeatString, "%d,", 1 + _BI.PeasentClassBoardingGroupOffset);
        break;
        case BMWilma:
        sprintf(_SeatString, "%d,", _SeatIndex + _BI.PeasentClassBoardingGroupOffset);
        break;
        case BMFrontToBack:
        sprintf(_SeatString, "%d,", _BI.CurrentBoardingGroup);
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
        default:
            return 0;
        break;
    }
        
}