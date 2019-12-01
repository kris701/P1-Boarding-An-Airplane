#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


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

typedef struct PlaneInformation {
    int FirstClassRows;
    int PeasentClassRows;
    int SeatsPerRow;
    int FirstClassSeatsPerRow;
    int BoardingGroupRowCount;
} PlaneInformation;

typedef struct BoardingInfo {
    PlaneInformation PlaneInfo;
    BoardingMethods BoardingMethod;
    int CurrentBoardingGroup;
    int PeasentClassBoardingGroupOffset;
    int BoardingGroupRowCounter;
    int SeatCounter;
    Classes CurrentClass;
} BoardingInfo;

void ReadConfigFile(BoardingInfo *_BI, FILE *_FP);
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
    BoardingInfo BI = {{0, 0, 0, 0, 0}, BMWilma, 1, 0, 0, 0};

    FP = fopen("config.ini", "r");
    if (FP == NULL) {
        printf("Missing config file\nShutting down\n");
        return 0;
    }
    ReadConfigFile(&BI, FP);

    if (BI.PlaneInfo.SeatsPerRow % 2 != 0) {
        printf("Uneven amount of seats per row\nShutting down\n");
        return 0;
    }


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

void ReadConfigFile(BoardingInfo *_BI, FILE *_FP) {
    char str[128];
    char *substring = "";
    char *delimiter = "=";
    for (int i = 0; i < 7; i++) {
        // If there is text, which should really always be the case
        if (fgets(str, 128, _FP) != NULL) {
            // If the line isn't a comment
            if (str[0] != ';') {
                if (strstr(str, "first_class_rows")) {
                    substring = strchr(str, '=') + 1;
                    _BI->PlaneInfo.FirstClassRows = atoi(substring);
                } else if (strstr(str, "economy_class_rows")) {
                    substring = strchr(str, '=') + 1;
                    _BI->PlaneInfo.PeasentClassRows = atoi(substring);
                } else if (strstr(str, "seats_per_row")) {
                    substring = strchr(str, '=') + 1;
                    _BI->PlaneInfo.SeatsPerRow = atoi(substring);
                    _BI->PlaneInfo.FirstClassSeatsPerRow = (_BI->PlaneInfo.SeatsPerRow / 2 + (_BI->PlaneInfo.SeatsPerRow / 2 % 2));
                } else if (strstr(str, "boarding_group_size")) {
                    substring = strchr(str, '=') + 1;
                    _BI->PlaneInfo.BoardingGroupRowCount = atoi(substring);
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

int CreateFirstClassRows(BoardingInfo *_BI, FILE *_FP) { 
    int seats = _BI->PlaneInfo.SeatsPerRow / 2, offset = seats % 2;
    char SeatString[10];
    _BI->CurrentClass = CFirst;
    for (int j = 0; j < _BI->PlaneInfo.FirstClassRows; j++) {  
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
            return (_BI.PlaneInfo.SeatsPerRow / 4 + (_BI.PlaneInfo.SeatsPerRow / 2) % 2);
        break;
        case BMFrontToBack:
            return (_BI.PlaneInfo.FirstClassRows / _BI.PlaneInfo.BoardingGroupRowCount) + ((_BI.PlaneInfo.FirstClassRows % _BI.PlaneInfo.BoardingGroupRowCount == 0) ? 0 : 1);
        break;
        case BMBackToFront:
            return (_BI.PlaneInfo.FirstClassRows / _BI.PlaneInfo.BoardingGroupRowCount) + ((_BI.PlaneInfo.FirstClassRows % _BI.PlaneInfo.BoardingGroupRowCount == 0) ? 0 : 1);
        break;
        case BMSteffenModified:
            return (2 * _BI.PlaneInfo.FirstClassRows);
        break;
        case BMJanModified:
            return (_BI.PlaneInfo.FirstClassRows * _BI.PlaneInfo.FirstClassSeatsPerRow);
        break;
        case BMSteffenPerfect:
            return (_BI.PlaneInfo.FirstClassRows * _BI.PlaneInfo.FirstClassSeatsPerRow);
        break;
        default:
            return 0;
        break;
    }
        
}

int GetSeatForFrontToBack(BoardingInfo *_BI, int _SeatIndex) {
    int rowCount = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassRows : _BI->PlaneInfo.PeasentClassRows);
    return  (1 + 

                _BI->PeasentClassBoardingGroupOffset +

                (_BI->BoardingGroupRowCounter + 
                (_BI->BoardingGroupRowCounter % 2 == 1 ? 0 : 1)) 
                / (2 * _BI->PlaneInfo.BoardingGroupRowCount));
}

int GetSeatForBackToFront(BoardingInfo *_BI, int _SeatIndex) {
    int rowCount = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassRows : _BI->PlaneInfo.PeasentClassRows);
    return (_BI->PeasentClassBoardingGroupOffset +
           (rowCount / _BI->PlaneInfo.BoardingGroupRowCount) + ((rowCount % _BI->PlaneInfo.BoardingGroupRowCount == 0) ? 0 : 1) -
           ((_BI->BoardingGroupRowCounter + (_BI->BoardingGroupRowCounter % 2 == 1 ? 0 : 1)) / 
           (2 * _BI->PlaneInfo.BoardingGroupRowCount))


        );
}

int GetSeatForSteffenModified(BoardingInfo *_BI, int _SeatIndex) {
    int rowCount = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassRows : _BI->PlaneInfo.PeasentClassRows);
    return  (2 * rowCount +
                  _BI->PeasentClassBoardingGroupOffset -
                ((_BI->BoardingGroupRowCounter % 2 == 1) ? rowCount : rowCount / 2) -
                 (_BI->BoardingGroupRowCounter / 4) -
                ((_BI->BoardingGroupRowCounter % 4 == ((rowCount % 2 == 0) ? 2 : 0)) ? rowCount : 0) +
               (((_BI->BoardingGroupRowCounter % 4 == ((rowCount % 2 == 0) ? 1 : 3)) && (_BI->BoardingGroupRowCounter % 2 == 1)) ? rowCount : 0) +
               ((rowCount % 2 == 1) ? (((_BI->BoardingGroupRowCounter % 4 == 1) && (_BI->BoardingGroupRowCounter % 2 == 1)) ? 1 : 0) : 0)) ;
}

int GetSeatForJanModified(BoardingInfo *_BI, int _SeatIndex) {
    int rowCount = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassRows : _BI->PlaneInfo.PeasentClassRows);
    int SeatsPerRow = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassSeatsPerRow : _BI->PlaneInfo.SeatsPerRow);
    return (_BI->PeasentClassBoardingGroupOffset +
            rowCount * SeatsPerRow -
            _BI->BoardingGroupRowCounter * (SeatsPerRow / 2) -
            ((SeatsPerRow / 2) - _SeatIndex)
           );
}

int GetSeatForSteffenPerfect(BoardingInfo *_BI, int _SeatIndex) {
    int rowCount = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassRows : _BI->PlaneInfo.PeasentClassRows);
    int SeatsPerRow = ((_BI->CurrentClass == CFirst) ? _BI->PlaneInfo.FirstClassSeatsPerRow : _BI->PlaneInfo.SeatsPerRow);
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