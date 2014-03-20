#ifndef __GAME__
#define __GAME__

static const int PLAYER_ONE = 1;
static const int PLAYER_TWO = -1;
static const int EMPTY = 0;

struct point
{
    int x, y, state;
} ;

typedef struct point point_type;

struct board {

	point_type*** grid;
	int * heights;

	int cols;
	int rows;

	int * moves;
	int lm;

	int cp;
	point_type *** cl;
    } ;
typedef struct board board_type;

point_type * newPoint(int a, int b);
void deletepoint(point_type* p);
void setState(point_type * a, int player);
int getState(point_type * a);
point_type*** generateCL(point_type *** grid);
board_type * createBoard(int a, int b);
void deleteboard(board_type* p);
int validMove(board_type * b, int column);
void makeMove(board_type * b, int column);
void undoMove(board_type * b);
int validMovesLeft(board_type * b);
int getScore(point_type * points[]);

int getStrength(board_type * b);
int winnerIs(board_type * b);
char * toString(board_type * b);
int cp(board_type * b);
int getReasonedMove(board_type * cB);
int minValue(board_type * cB, int ply);
int maxValue(board_type * cB, int ply);

#endif
