#ifndef STRUCTSANDENUMS_HEADER
#define STRUCTSANDENUMS_HEADER

#include <stdbool.h>

enum XAxis { Col_a, Col_b, Col_c, Col_d, Col_e, Col_f, Col_g, Col_h, Col_i, Col_j, Col_k, Col_l, Col_m, Col_n, Col_o, Col_p, Col_q, Col_r, Col_s, Col_t};
typedef enum XAxis XAxis;

struct Point
{
	enum XAxis X;
	int Y;
};
typedef struct Point Point;

struct Person
{
	int WalkingSpeed;
	struct Point CurrentPos;
	struct Point Target;
	bool IsSeated;
	int StartingDoorID;
	char PersonCharacter;
	int LuggageCount;
	bool IsBackingUp;
	int ShuffleDelay;
	int CrossDelay;
	int StepsTaken;
	bool MovedLastTurn;
	struct Point NextMove;
};
typedef struct Person Person;

struct BaseSimulationRules
{
	int ShuffleDelay;
	int CrossDelay;
};
typedef struct BaseSimulationRules BaseSimulationRules;

#endif