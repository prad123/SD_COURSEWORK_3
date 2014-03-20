#include <stdlib.h>
#include <stdio.h>
#include "game.h"
/*
 * 
 */
int main(int argc, char** argv) {
	board_type * b = createBoard(NCOLS, NROWS);
	if(NULL == b){
		printf("Unable to generate board. Possible cause system low in memory.\n");
		printf("Application aborting...\n");
		return 1;
	}

	int input;
	while((winnerIs(b)==0) && validMovesLeft(b))
	{
		if(getCurrentPlayer(b) == PLAYER_ONE)	
		{
			scanf("%d", &input);
			if(input < 7 && validMove(b, input)){
				makeMove(b,input);
			} else {
				printf("Invalid input... try again\n");
			}
		}
		else
			makeMove(b, getReasonedMove(b));

		//char* output = toString(b);
		printf("%s\n", toString(b));
		//free(output);


	}
	deleteboard(b);

	return 0;

}


