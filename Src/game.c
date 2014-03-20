#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include "game.h"

int minValue(board_type * cB, int ply);
int maxValue(board_type * cB, int ply);

point_type * newPoint(int x, int y)
{
    point_type * p = (point_type*)malloc(sizeof(point_type));
    p->x=x;
    p->y=y;
    p->player=EMPTY;
    return p;
}
void deletepoint(point_type* p)
{
    free(p);
}

void setPlayer(point_type * p, int player)
{
    p->player=player;
}

int getPlayer(point_type * p)
{
    return p->player;
}


point_type*** generateCL(point_type *** grid)
{
    point_type *** lines = (point_type ***)malloc(69 * sizeof(point_type **));
    int i;
    int count =0;
    int y;
    int x;
    int t;
    for(y=0;y<6;y++)
    {

	for( x=0;x<4;x++)
	{
	    point_type ** temp = (point_type **)malloc(4 * sizeof(point_type *));
	    for(i=x;i<x+4;i++)
	    {
		temp[i-x]=grid[i][y];
	    }
	    lines[count]=temp;
	    count++;
	}
    }

    for( x=0;x<7;x++)
    {
	for( y=0;y<3;y++)
	{
	    point_type ** temp = (point_type**)malloc(4 * sizeof(point_type *));
	    for( i=y;i<y+4;i++)
		temp[i-y]=grid[x][i];
	    lines[count]=temp;
	    count++;
	}

    }

    for( x=0;x<4;x++)
    {
	for( y=0;y<3;y++)
	{
	    point_type** temp = (point_type**)malloc(4 * sizeof(point_type *));
	    for( t=x,i=y;t<x+4 && i<y+4;t++,i++)
		temp[i-y]=grid[t][i];
	    lines[count]=temp;
	    count++;
	}

    }
    for( x=0;x<4;x++)
    {
	for( y=5;y>2;y--)
	{
	    point_type ** temp = (point_type **)malloc(4 * sizeof(point_type *));
	    for( t=x,i=y;t<x+4 && i>-1;t++,i--)
		temp[t-x]=grid[t][i];
	    lines[count]=temp;
	    count++;
	}
    }
    return lines;
}

board_type * createBoard(int cols, int rows)
{
    board_type * p = (board_type*)malloc(sizeof(board_type));
    p->n_cols=cols;
    p->n_rows=rows;
    p->total_moves=-1;
    p->current_player=PLAYER_ONE;
    p->column_height = (int *)malloc(p->n_cols * sizeof(int));
    p->grid = (point_type ***)malloc(p->n_cols * sizeof(point_type **));
    int x;
    int y;
    for(x = 0; x < p->n_cols; x++)
    {
	p->grid[x] =(point_type **)malloc(p->n_rows * sizeof(point_type *));
	p->column_height[x] = 0;
	for(y = 0; y< p->n_rows; y++)
	{
	    p->grid[x][y] = newPoint(x,y);
	}
    }
    p->moves_history = (int *)malloc(p->n_cols * p->n_rows * sizeof(int));

    p->consecutive_lines = generateCL(p->grid);
    return p;
}

void deleteboard(board_type* b)
{
    int i = 0;
    for(i=0; i < 69; ++i){
	free(b->consecutive_lines[i]);
    }
    free(b->consecutive_lines);
    free(b->grid);
    free(b->column_height);
    free(b->moves_history);
    free(b);
}

int validMove(board_type * b, int column)
{
    return b->column_height[column] < b->n_rows;
}

void makeMove(board_type * b, int column)
{
	int row = b->column_height[column];
    setPlayer(b->grid[column][row],b->current_player);

    b->column_height[column]++;
    b->total_moves++;
    b->moves_history[b->total_moves]=column;
    b->current_player = -b->current_player;
}


void undoMove(board_type * b)
{
	int column = b->moves_history[b->total_moves];
	int last_position = b->moves_history[b->total_moves];
	int row = b->column_height[last_position];
    setPlayer(b->grid[column][row-1],(EMPTY));
    //setPlayer(b->grid[b->moves_history[b->total_moves]][b->column_height[b->moves_history[b->total_moves]]-1],(EMPTY));
    b->column_height[b->moves_history[b->total_moves]]--;
    b->total_moves--;
    b->current_player = -b->current_player;
}


int validMovesLeft(board_type * b)
{
    return b->total_moves < ((b->n_cols*b->n_rows)-1);
}

int getScore(point_type * points[]) {

    int playerone=0;

    int playertwo=0;
    int i;
    for( i=0;i<4;i++){
	if(getPlayer(points[i])==PLAYER_ONE) {
	    playerone++;
	} else if(getPlayer(points[i])==PLAYER_TWO) {
	    playertwo++;
	}
    } 
    if(playerone>0 && playertwo==0){
	return playerone;
    } else if(playertwo > 0 && playerone == 0){
	return -playertwo;
    }

    return 0;
}

int getStrength(board_type * b)
{
    int sum=0;
    int weights[] = {0,1,10,50,600};
    int i;
    for( i=0;i<69;i++)
    {
	sum+=(getScore(b->consecutive_lines[i])>0)?weights[abs(getScore(b->consecutive_lines[i]))]:-weights[abs(getScore(b->consecutive_lines[i]))];
    }
    return sum+(b->current_player==PLAYER_ONE?16:-16);
}

int winnerIs(board_type * b)
{
    int i;
    for( i=0;i<69;i++)
    {
	if(getScore(b->consecutive_lines[i])==4)
	{
	    return PLAYER_ONE;
	}
	else if(getScore(b->consecutive_lines[i])==-4)
	{
	    return PLAYER_TWO;
	}
    }
    return 0;

}

char * toString(board_type * b)
{

    char * temp = (char *)malloc(b->n_rows*(b->n_cols+1)*sizeof(char)+1);

    char * curr = temp;
    int y;
    int x;
    for( y=b->n_rows-1;y>-1;y--){

	for( x=0;x < b->n_cols;x++)
	{
	    if(getPlayer(b->grid[x][y])==EMPTY)
		*curr = '-';
	    else if(getPlayer(b->grid[x][y])==PLAYER_ONE)
		*curr = 'O';
	    else
		*curr = 'X';
	    curr++;
	}
	*curr = '\n';
	curr++;
    }
    return temp;
}

int getCurrentPlayer(board_type * b)
{
    return b->current_player;
}


int getReasonedMove(board_type * b)
{
    int moves[7];
    int highest = 0;
    int i;
    for( i=0;i<7;i++)
    {
	moves[i] = INT_MIN;
	if(validMove(b, i))
	{
	    makeMove(b,i);
	    moves[i] = minValue(b,4);
	    if(moves[i]>=moves[highest])
		highest=i;
	    undoMove(b);
	} 
    }
    return highest;
}

int minValue(board_type * b, int ply)
{
    int moves[7];
    int lowest = 0;
    int i;
    for( i=0;i<7;i++)
    {
	moves[i] = INT_MAX;
	if(validMove(b,i))
	{
	    makeMove(b,i);
	    if((winnerIs(b) == 0) && ply>0)
	    {
		moves[i] = maxValue(b,ply-1);
	    }
	    else 
	    {

		moves[i] = -getStrength(b);


	    }
	    if(moves[i]<moves[lowest])
		lowest=i;
	    undoMove(b);
	}

    }

    return moves[lowest];

}

int maxValue(board_type * b, int ply)
{

    int moves[7];
    int highest = 0;
    int i;
    for( i=0;i<7;i++)
    {
	moves[i] = INT_MAX;
	if(validMove(b,i))
	{
	    makeMove(b,i);
	    if((winnerIs(b) == 0) && ply>0)
	    {
		moves[i] = minValue(b, ply-1);
	    }
	    else 
		moves[i] =-getStrength(b);
	    if(moves[i]<moves[highest])
		highest=i;
	    undoMove(b);
	}

    }

    return moves[highest];

}




