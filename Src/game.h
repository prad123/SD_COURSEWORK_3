#ifndef __GAME__
#define __GAME__

static const int PLAYER_ONE = 1;
static const int PLAYER_TWO = -1;
static const int EMPTY = 0;

struct point
{
    int x;		//x position
    int y;		//y position
    int player;		//PLAYER_ONE, PLAYER_TWO or EMPTY
} ;

typedef struct point point_type;

struct board {

	point_type*** grid;	//grid of points
	int * column_height;	//number of tokens in a column

	int n_cols;		//number of columns
	int n_rows;		//number of rows

	int * moves_history;	//last moved steps
	int total_moves;	

	int current_player;
	point_type *** consecutive_lines; //winning line formation
    } ;
typedef struct board board_type;

point_type * newPoint(int x, int y);
void setPlayer(point_type * p, int player);
int getPlayer(point_type * p);
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
