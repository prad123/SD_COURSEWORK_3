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
    p->state=EMPTY;
    return p;
}
void deletepoint(point_type* p)
{
    free(p);
}

void setState(point_type * p, int player)
{
    p->state=player;
}

int getState(point_type * p)
{
    return p->state;
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
    p->cols=cols;
    p->rows=rows;
    p->lm=-1;
    p->cp=PLAYER_ONE;
    p->heights = (int *)malloc(p->cols * sizeof(int));
    p->grid = (point_type ***)malloc(p->cols * sizeof(point_type **));
    int x;
    int y;
    for(x = 0; x < p->cols; x++)
    {
	p->grid[x] =(point_type **)malloc(p->rows * sizeof(point_type *));
	p->heights[x] = 0;
	for(y = 0; y< p->rows; y++)
	{
	    p->grid[x][y] = newPoint(x,y);
	}
    }
    p->moves = (int *)malloc(p->cols * p->rows * sizeof(int));

    p->cl = generateCL(p->grid);
    return p;
}

void deleteboard(board_type* p)
{
    int i = 0;
    for(i=0; i < 69; ++i){
	free(p->cl[i]);
    }
    free(p->cl);
    free(p->grid);
    free(p->heights);
    free(p->moves);
    free(p);
}

int validMove(board_type * b, int column)
{
    return b->heights[column]<b->rows;
}

void makeMove(board_type * b, int column)
{
    setState(b->grid[column][b->heights[column]],b->cp);

    b->heights[column]++;
    b->lm++;
    b->moves[b->lm]=column;
    b->cp=-b->cp;
}


void undoMove(board_type * b)
{

    setState(b->grid[b->moves[b->lm]][b->heights[b->moves[b->lm]]-1],(EMPTY));
    b->heights[b->moves[b->lm]]--;
    b->lm--;
    b->cp=-b->cp;
}


int validMovesLeft(board_type * b)
{
    return b->lm<((b->cols*b->rows)-1);
}

int getScore(point_type * points[]) {

    int playerone=0;

    int playertwo=0;
    int i;
    for( i=0;i<4;i++){
	if(getState(points[i])==PLAYER_ONE)
	    playerone++;
	else if(getState(points[i])==PLAYER_TWO)
	    playertwo++;
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
	sum+=(getScore(b->cl[i])>0)?weights[abs(getScore(b->cl[i]))]:-weights[abs(getScore(b->cl[i]))];
    }
    return sum+(b->cp==PLAYER_ONE?16:-16);
}

int winnerIs(board_type * b)
{
    int i;
    for( i=0;i<69;i++)
    {
	if(getScore(b->cl[i])==4)
	{
	    return PLAYER_ONE;
	}
	else if(getScore(b->cl[i])==-4)
	{
	    return PLAYER_TWO;
	}
    }
    return 0;

}

char * toString(board_type * b)
{

    char * temp = (char *)malloc(b->rows*(b->cols+1)*sizeof(char)+1);

    char * curr = temp;
    int y;
    int x;
    for( y=b->rows-1;y>-1;y--){

	for( x=0;x<b->cols;x++)
	{
	    if(getState(b->grid[x][y])==EMPTY)
		*curr = '-';
	    else if(getState(b->grid[x][y])==PLAYER_ONE)
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

int cp(board_type * b)
{
    return b->cp;
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




