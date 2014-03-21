#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include "game.h"

int minValue(board_type * cB, int ply);
int maxValue(board_type * cB, int ply);

point_type newPoint(int x, int y)
{
	point_type p;
	p.x=x;
	p.y=y;
	p.player=EMPTY;
	return p;
}

void setPlayer(point_type * p, int player)
{
	p->player=player;
}

int getPlayer(point_type * p)
{
	return p->player;
}

int generateVerticalLines(board_type* b, int count){
	int y	  = 0;
	int x	  = 0;
	int i	  = 0;
	const int max_consec_moves_possible=4;

	for(y=0;y<b->n_rows;y++)
	{

		for( x=0;x<max_consec_moves_possible;x++)
		{
			for(i=x;i<x+CONSEC_TOKENS_TO_WIN;i++)
			{
				b->consecutive_lines[count]=&b->grid[i][y];
				count++;
			}
		}
	}

	return count;
}

int generateHorizontalLines(board_type* b, int count){
	int y	  = 0;
	int x	  = 0;
	int i	  = 0;
	const int max_consec_moves_possible=3;
	
	for( x=0;x<b->n_cols;x++)
	{
		for( y=0;y<max_consec_moves_possible;y++)
		{
			for( i=y;i<y+CONSEC_TOKENS_TO_WIN;i++)
			{
				b->consecutive_lines[count]=&b->grid[x][i];
				count++;
			}
		}

	}
	return count;
}

int generateLeftLeaningDiagonal(board_type* b, int count){
	int y	  = 0;
	int x	  = 0;
	int t	  = 0;
	int i	  = 0;

	const int x_limit    = 4;
	const int y_limit    = 3;
	const int consec_tok = CONSEC_TOKENS_TO_WIN;

	for( x=0;x<x_limit;x++)
	{
		for( y=0;y<y_limit;y++)
		{

			for( t=x,i=y;t<x+consec_tok && i<y+consec_tok;t++,i++){
				b->consecutive_lines[count]=&b->grid[t][i];
				count++;
			}
		}

	}
	return count;

}

int generateRightLeaningDiagonal(board_type* b, int count){
	int y	  = 0;
	int x	  = 0;
	int t	  = 0;
	int i	  = 0;
	
	const int x_limit    = 4;
	const int y_begin    = 5;
	const int y_limit    = 2;
	const int consec_tok = CONSEC_TOKENS_TO_WIN;

	for( x=0;x<x_limit;x++)
	{
		for( y=y_begin;y>y_limit;y--)
		{
			for( t=x,i=y;t<x+consec_tok && i>-1;t++,i--){
				b->consecutive_lines[count]=&b->grid[t][i];
				count++;
			}
		}
	}
	return count;
}

void generateConsecutiveLines(board_type* b)
{
	int nextPos = 0;
	nextPos = generateVerticalLines(b, nextPos);
	nextPos = generateHorizontalLines(b, nextPos);
	nextPos = generateLeftLeaningDiagonal(b, nextPos);
	generateRightLeaningDiagonal(b, nextPos);
}

board_type * createBoard(int cols, int rows)
{
	int x = 0;
	int y = 0;

	board_type * p = (board_type*)malloc(sizeof(board_type));
	if(NULL == p)
	{
		return NULL;
	}

	p->n_cols	 =cols;
	p->n_rows	 =rows;
	p->total_moves	 =-1;
	p->current_player=PLAYER_ONE;

	for(x = 0; x < p->n_cols; x++)
	{
		p->column_height[x] = 0;
		for(y = 0; y < p->n_rows; y++)
		{
			p->grid[x][y] = newPoint(x,y);
		}
	}

	generateConsecutiveLines(p);
	return p;
}

void deleteboard(board_type* b)
{
	free(b);
}

int validMove(board_type * b, int column)
{
	return b->column_height[column] < b->n_rows;
}

void makeMove(board_type * b, int column)
{
	int row = b->column_height[column];
	setPlayer(&b->grid[column][row],b->current_player);

	b->column_height[column]++;
	b->total_moves++;
	b->moves_history[b->total_moves]=column;
	b->current_player = -b->current_player;
}


void undoMove(board_type * b)
{
	int column 	  = b->moves_history[b->total_moves];
	int last_position = b->moves_history[b->total_moves];
	int row 	  = b->column_height[last_position];

	setPlayer(&b->grid[column][row-1],(EMPTY));

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
	int i = 0;

	for( i=0;i<CONSEC_TOKENS_TO_WIN;i++){
		if(getPlayer(points[i])==PLAYER_ONE) 
		{
			playerone++;
		}
		 else if(getPlayer(points[i])==PLAYER_TWO) 
		{
			playertwo++;
		}
	}
 
	if(playerone>0 && playertwo==0)
	{
		return playerone;
	} 
	else if(playertwo > 0 && playerone == 0)
	{
		return -playertwo;
	}

	return 0;
}

int getStrength(board_type * b)
{
	int sum=0;
	int weights[] = {0,1,10,50,600};
	int i;
	int score = 0;
	for( i=0;i<POSSIBLE_CONSEC_MOVES;i+=4)
	{
		score = getScore(&b->consecutive_lines[i]);
		if(score > 0)
		{
			sum += weights[abs(score)];
		} 
		else 
		{
			sum -= weights[abs(score)];
		}
	}
	return sum+(b->current_player==PLAYER_ONE?WEIGHTAGE:-WEIGHTAGE);
}

int winnerIs(board_type * b)
{
	int i;
	for( i=0;i<POSSIBLE_CONSEC_MOVES;i+=4)
	{
		if(getScore(&b->consecutive_lines[i])==CONSEC_TOKENS_TO_WIN)
		{
			return PLAYER_ONE;
		}
		else if(getScore(&b->consecutive_lines[i])==-CONSEC_TOKENS_TO_WIN)
		{
			return PLAYER_TWO;
		}
	}
	return 0;

}

char * toString(board_type * b)
{

	static char temp[NROWS*(NCOLS+4)+1] = "\0";

	char * curr = temp;
	int y;
	int x;
	for( y=b->n_rows-1;y>-1;y--){

		*curr = '\t';
		curr++;
		*curr = '|';
		curr++;
		for( x=0;x < b->n_cols;x++)
		{
			if(getPlayer(&b->grid[x][y])==EMPTY)
			{
				*curr = '-';
			}
			else if(getPlayer(&b->grid[x][y])==PLAYER_ONE)
			{
				*curr = 'O';
			}
			else
			{
				*curr = 'X';
			}
			curr++;
		}
		*curr = '|';
		curr++;
		*curr = '\n';
		curr++;
	}
	
	*curr = '\0';
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
	for( i=0;i<b->n_cols;i++)
	{
		moves[i] = INT_MIN;
		if(validMove(b, i))
		{
			makeMove(b,i);
			moves[i] = minValue(b,SEARCH_DEPTH);
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
	for( i=0;i<b->n_cols;i++)
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
	for( i=0;i<b->n_cols;i++)
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
