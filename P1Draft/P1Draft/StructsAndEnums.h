#ifndef STRUCTSANDENUMS_HEADER
#define STRUCTSANDENUMS_HEADER

#include <stdbool.h>

enum XAxis { Col_a, Col_b, Col_c, Col_d, Col_e, Col_f, Col_g, Col_h, Col_i, Col_j, Col_k, Col_l, Col_m, Col_n, Col_o, Col_p, Col_q, Col_r, Col_s, Col_t};
enum MethodIndex { MI_Random };
enum FunctionType { Line, Exp };

struct Point
{
	enum XAxis X;
	int Y;
};

struct Person
{
	int WalkingSpeed;
	struct Point CurrentPos;
	struct Point Target;
	bool IsSeated;
	int StartingDoorID;
	char PersonCharacter;
	int LuggageCount;
	int OrgLuggageCount;
	bool IsPuttingLuggage;
	bool BackingUp;
	int StepsTaken;
	int StartIndex;
};

struct FunctionDefinition
{
	enum FunctionType Type;
	double a;
	double b;
	double c;
};

struct MethodDefinition
{
	int FromY;
	int ToY;
	enum XAxis FromX;
	enum XAxis ToX;

	int MinLuggage;
	int MaxLuggage;
	struct FunctionDefinition LuggageFunction;

	int MinDoorChoice;
	int MaxDoorChoice;
	struct FunctionDefinition DoorFunction;

	int MinWalkSpeed;
	int MaxWalkSpeed;
	struct FunctionDefinition WalkSpeedFunction;
};

#endif