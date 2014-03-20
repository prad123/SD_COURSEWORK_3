#ifndef __GAME__
#define __GAME__

#define NCOLS 7
#define NROWS 6

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

	point_type grid[7][6];	//grid of points
	int  column_height[7];	//number of tokens in a column

	int n_cols;		//number of columns
	int n_rows;		//number of rows

	int moves_history[7*6];	//last moved steps
	int total_moves;	

	int current_player;
	point_type * consecutive_lines[69*4]; //winning line formation
    } ;
typedef struct board board_type;

inline point_type  newPoint(int x, int y);
inline void setPlayer(point_type * p, int player);
inline int getPlayer(point_type * p);
board_type * createBoard(int x, int y);
void deleteboard(board_type* p);
int validMove(board_type * b, int column);
void makeMove(board_type * b, int column);
void undoMove(board_type * b);
inline int validMovesLeft(board_type * b);
int getReasonedMove(board_type* b);
int winnerIs(board_type * b);
char * toString(board_type * b);

#endif
