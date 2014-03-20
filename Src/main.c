#include <stdlib.h>
#include <stdio.h>
#include "game.h"
/*
 * 
 */
int main(int argc, char** argv) {
    //    srand (time(NULL));
    board_type * b = createBoard(7,6);

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

	char* output = toString(b);
	printf("%s\n", output);
	free(output);


    }
    int i, j;
    for(i=0; i<b->n_cols; ++i){
	for(j=0; j < b->n_rows; j++){
	    deletepoint(b->grid[i][j]);
	}
	free(b->grid[i]);
    }
    deleteboard(b);

    return 0;

}


