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

point_type * newPoint(int x, int y);
void setState(point_type * p, int player);
int getState(point_type * p);
board_type * createBoard(int x, int y);
void deleteboard(board_type* p);
int validMove(board_type * b, int column);
void makeMove(board_type * b, int column);
void undoMove(board_type * b);
int validMovesLeft(board_type * b);
int getReasonedMove(board_type* b);
int winnerIs(board_type * b);
char * toString(board_type * b);

#endif
