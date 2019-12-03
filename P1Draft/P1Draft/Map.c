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
		int tmpInt;
        while (sscanf_s(buffer, "%[^,]", field, 32) == 1) { // One iteration per field of data in a line
            switch (field[0]) { // Special characters will typically just be the first character of the field.
                case '|': 
					MapLocationSetValue(map, x, y, BoardingGroup_Walkway);
					x++;
				break;
                case '\D':
					MapLocationSetValue(map, x, y, BoardingGroup_Door);
				break;
                case '-':
					MapLocationSetValue(map, x, y, BoardingGroup_Walkway);
					x++;
				break;
				case '\r': break;
				case '\0': break;

                default:
                    if (sscanf_s(field, "%d", &tmpInt) == 1) {
						MapLocationSetValue(map, x, y, tmpInt);
                        x++;
                    }
                    else {
                        fprintf(stderr, "Unknown value '%s'\n", field);
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

void MapLocationSetValue(Map* map, int x, int y, int value) {
	map->Locations[x][y].BoardingGroup = value;
}

int MapLocationGetValue(Map* map, int x, int y) {
	return map->Locations[x][y].BoardingGroup;
}

bool AllocateMapPoints(Map* map) {
	map->Locations = calloc(map->Width, sizeof(Location*));
    if (map->Locations == NULL) {
        fprintf(stderr, "Failed to allocate %d bytes for map width\n", (int)(map->Width * sizeof(Location*)));
        return false;
    }

    for (int i = 0; i < map->Width; i++) {
        map->Locations[i] = calloc(map->Height, sizeof(Location));
        if (map->Locations[i] == NULL) {
            fprintf(stderr, "Failed to allocate %d bytes for map row %d\n", (int)(map->Height * sizeof(Location)), i);
            return false;
        }
    }
    return true;
}

void FreeMapPoints(Map* map) {
    if (map == NULL) return;
    if (map->Locations == NULL) return;

    for (int i = 0; i < map->Width; i++) {
        if (map->Locations != NULL) {
			free(map->Locations[i]);
        }
    }
    free(map->Locations);
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