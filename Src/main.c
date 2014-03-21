#include <stdlib.h>
#include <stdio.h>
//#include <unistd>
#include "game.h"

char user_name[255];

void showWinner(board_type *b)
{
	int winner = winnerIs(b);
	if(EMPTY == winner)
	{
		printf("It's a draw\n");
	} 
	else if(PLAYER_ONE == winner)
	{
		printf("%s Connected Four, You Win!\n", user_name);
	}
	else if(PLAYER_TWO == winner)
	{
		printf("Computer Wins!\n");
	}

}

void showWelcome(){

	printf("\nWelcome to Connect4\n");
	printf("The goal of the game is to be the first of 2 ");
	printf("competitors \nto place 4 game pieces in a row on the ");
	printf("game grid - vertically,\nhorizontally, or diagonally.\n");

	printf("\nPlese enter your name (max 255 characters): ");
	scanf("%s",user_name);
	printf("\nHi %s, Let's play connect4\n\n", user_name);
}

void user_turn(board_type* b){
	int input;
	printf("%s's turn. ", user_name);
	printf("Choose a column (valid range is 1-7): ");
	do
	{
		scanf("%d", &input);
		if(input < 1 || input > 7)
		{
			printf("Invalid input. ");
			printf("Plese enter a value in the range 1-7: ");
		}
		else if(!validMove(b, input-1)) 
		{
			printf("That column is full, ");
			printf("Please enter a new column: ");
		
		}
		else
		{
			makeMove(b,input-1);
			system("clear");
			printf("Your last move.\n\n");
			break;
		}
 
	}while(1);
}

void computer_turn(board_type* b){
	printf("Computer's turn...\n\n");
	makeMove(b, getReasonedMove(b));
}

int main(int argc, char** argv) {

	showWelcome();

	board_type * b = createBoard(NCOLS, NROWS);
	if(NULL == b){
		printf("Unable to generate board. System low on memory.\n");
		printf("Application aborting...\n");
		return 1;
	}

	int input;
	while((winnerIs(b)==0) && validMovesLeft(b))
	{
		if(getCurrentPlayer(b) == PLAYER_ONE)	
		{
			user_turn(b);
		}
		else
		{
			computer_turn(b);
		}

		printf("%s\n", toString(b));
	}

	showWinner(b);
	deleteboard(b);
	printf("GAME OVER!\n\n");
	return 0;

}


