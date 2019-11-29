#include "Map.h"

bool ReadMapFromFile(Map* map, FILE* file) {
    FreeMapPoints(map);
    map->Width  = GetSeatsPerLine(file);
    map->Height = GetNumberOfLinesInFile(file); 
    
    if (AllocateMapPoints(map) == false) {
        return false;
    }

    int bufferLength = GetNumberOfCharsForLongestLineInFile(file);
    char* buffer = calloc(bufferLength, sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate %d bytes for filebuffer while reading map\n", bufferLength);
        return false;
    }

    long int initialFileCursorLocation = ftell(file);
    fseek(file, 0, SEEK_SET);

    int x=0, y=0;
    while (fgets(buffer, bufferLength, file) != NULL) { // One iteration per line, until NULL is returned at EOF
        char field[32];
        while (sscanf_s(buffer, "%[^,]", field, 32) == 1) { // One iteration per field of data in a line
            switch (field[0]) {
                case '|':  break;
                case '\r': break;
                case '\0': break;
                case '-': x++; break;

                default:
                    if (sscanf_s(field, "%d", &(map->Seats[x][y].BoardingGroup)) == 1) {
                        x++;
                    }
                    else {
                        fprintf(stderr, "Unknown value '%s'", field);
                    }
                break;
            }
        }
        y++;
    }

    free(buffer);

    fseek(file, initialFileCursorLocation, SEEK_SET);
    return true;
}

bool AllocateMapPoints(Map* map) {
    if (map->Seats == NULL) {
        fprintf(stderr, "Failed to allocate %d bytes for map width\n", (int)(map->Width * sizeof(Seat*)));
        return false;
    }

    for (int i = 0; i < map->Width; i++) {
        map->Seats[i] = calloc(map->Height, sizeof(Point));
        if (map->Seats[i] == NULL) {
            fprintf(stderr, "Failed to allocate %d bytes for map row %d\n", (int)(map->Height * sizeof(Point)), i);
            return false;
        }
    }
    return true;
}

void FreeMapPoints(Map* map) {
    if (map == NULL) return;
    if (map->Seats == NULL) return;

    for (int i = 0; i < map->Width; i++) {
        if (map->Seats != NULL) {
			free(map->Seats[i]);
        }
    }
    free(map->Seats);
}

int GetSeatsPerLine(FILE* file) {
    int tmpWidth = 0, highestWidth = 0;
    long int initialFileCursorLocation = ftell(file);
    fseek(file, 0, SEEK_SET);

    char ch;
    do {
        ch = fgetc(file);
        switch (fgetc(file)) {
        case '\n':
            highestWidth = max(tmpWidth, highestWidth);
            tmpWidth = 0;
            break;
        case ',':
            tmpWidth++;
            break;
        default:
            break;
        }
    } while (!feof(file));

    fseek(file, initialFileCursorLocation, SEEK_SET);
    return highestWidth;
}
int GetNumberOfCharsForLongestLineInFile(FILE* file) {
    // Goes through each line and finds the longest
    int tmpWidth = 0, highestWidth = 0;
    long int initialFileCursorLocation = ftell(file);
    fseek(file, 0, SEEK_SET);

    char ch;
    do {
        ch = fgetc(file);
        tmpWidth++;
        if (ch == '\n') {
            highestWidth = max(tmpWidth, highestWidth);
            tmpWidth = 0;
        }
    } while (ch != EOF);

    fseek(file, initialFileCursorLocation, SEEK_SET);
    return highestWidth + 1; // Null-byte
}

int GetNumberOfLinesInFile(FILE* file) {
    int lines = 1; // Begins at line 1, if �ne \n is occured, then there are 2 lines
    long int initialFileCursorLocation = ftell(file);
    fseek(file, 0, SEEK_SET);

    char ch;
    do {
        ch = fgetc(file);
        if (ch == '\n') lines++;
    } while (ch != EOF);

    fseek(file, initialFileCursorLocation, SEEK_SET);
    return lines;
}