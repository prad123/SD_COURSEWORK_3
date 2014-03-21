#ifndef __GAME__
#define __GAME__

#define NCOLS 			   7
#define NROWS 			   6
#define CONSEC_TOKENS_TO_WIN  	   4
#define POSSIBLE_CONSEC_MOVES 	   (69*CONSEC_TOKENS_TO_WIN)
#define SEARCH_DEPTH		   4
#define WEIGHTAGE		   16

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

	point_type grid[NCOLS][NROWS];	//grid of points
	int  column_height[NCOLS];	//number of tokens in a column
	int n_cols;			//number of columns
	int n_rows;			//number of rows
	int moves_history[NCOLS*NROWS];	//last moved steps
	int total_moves;		//moves made so far in the game
	int current_player;		//whose turn
	point_type * consecutive_lines[POSSIBLE_CONSEC_MOVES]; //winning line formation
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
