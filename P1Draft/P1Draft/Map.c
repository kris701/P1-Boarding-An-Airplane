#include "Map.h"

bool ReadMapFromFile(Map* map, FILE* file) {
    _FreeMap(map);
    map->Width  = GetSeatsPerLine(file);
    map->Height = GetNumberOfLinesInFile(file); 
	map->DoorCount = GetNumberOfDoorsInBoardingMethod(file);
    
    if (_AllocateMap(map) == false) {
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

    int x=0, y=0, doorIndex=0;
    while (fgets(buffer, bufferLength, file) != NULL) { // One iteration per line, until NULL is returned at EOF
        char field[32];
		int bufferOffset = 0;
		int tmpInt;
        while (sscanf_s(buffer+bufferOffset, "%[^,]", field, 32) == 1) { // One iteration per field of data in a line
			bufferOffset += strlen(field)+1; // +Comma
			if (bufferOffset > strlen(buffer)) break;
            switch (field[0]) { // Special characters will typically just be the first character of the field.
                case '|': 
					MapLocationSetValue(map, x, y, BoardingGroup_Walkway);
					x++;
				break;
                case '\D':
					MapLocationSetValue(map, x, y, BoardingGroup_Door);
					map->Doors[doorIndex].X = x;
					map->Doors[doorIndex].Y = y;
				break;
                case '-':
					MapLocationSetValue(map, x, y, BoardingGroup_Walkway);
					x++;
				break;
				case '\r': break;
				case '\0': break;
				case '\n':
					y++;
					x = 0;
				break;

                default:
                    if (sscanf_s(field, "%d", &tmpInt) == 1) {
						MapLocationSetValue(map, x, y, tmpInt);
						MapLocationGet(map, x, y)->IsTaken = 0;
						map->NumberOfSeats++;
                        x++;
                    }
                    else {
                        fprintf(stderr, "Unknown value '%s'\n", field);
                    }
                break;
            }
        }
        y++;
		x = 0;
    }

    free(buffer);

    fseek(file, initialFileCursorLocation, SEEK_SET);
    return true;
}

void MapLocationSetValue(Map* map, int x, int y, int value) {
	MapLocationGet(map, x, y)->BoardingGroup = value;
}

Location* MapLocationGet(Map* map, int x, int y) {
	return &(map->Locations[x][y]);
}

bool _AllocateMap(Map* map) {
	map->Locations = calloc(map->Width, sizeof(Location*));
    if (map->Locations == NULL) {
        fprintf(stderr, "Failed to allocate %d bytes for map width\n", (int)(map->Width * sizeof(Location*)));
        return false;
    }

    for (int x = 0; x < map->Width; x++) {
        map->Locations[x] = calloc(map->Height, sizeof(Location));
		for (int y = 0; y < map->Height; y++) {
			map->Locations[x][y].Point.X = x;
			map->Locations[x][y].Point.Y = y;
		}
        if (map->Locations[x] == NULL) {
            fprintf(stderr, "Failed to allocate %d bytes for map row %d\n", (int)(map->Height * sizeof(Location)), x);
            return false;
        }
    }

	map->Doors = calloc(map->DoorCount, sizeof(Point*));

    return true;
}

void _FreeMap(Map* map) {
    if (map == NULL) return;
	if (map->Locations != NULL) {
		for (int i = 0; i < map->Width; i++) {
			if (map->Locations != NULL) {
				free(map->Locations[i]);
			}
		}
		free(map->Locations);
		map->Width = 0;
		map->Height = 0;
	}

	if (map->Doors != NULL) {
		free(map->Doors);
		map->DoorCount = 0;
	}
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
int GetNumberOfDoorsInBoardingMethod(FILE* file) {
	long int initialFileCursorLocation = ftell(file);
	fseek(file, 0, SEEK_SET);

	int numberOfDoors = 0;
	char ch1=fgetc(file), ch2;
	while (ch2 = fgetc(file) != EOF) {
		if (ch1 == ',' && ch2 == 'D') {
			numberOfDoors++;
		}
		ch1 = ch2;
	}

	fseek(file, initialFileCursorLocation, SEEK_SET);
	return numberOfDoors;
}
