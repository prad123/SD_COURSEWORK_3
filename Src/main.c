#include <stdlib.h>
#include <stdio.h>
#include "game.h"
/*
 * 
 */
int main(int argc, char** argv) {
    srand (time(NULL));
    board_type * b = createBoard(7,6);

    int input;
    while((winnerIs(b)==0) && validMovesLeft(b))
    {
	if(cp(b) == PLAYER_ONE)	
	{
	    scanf("%d", &input);
	    makeMove(b,input);// Make it so!
	}
	else
	    makeMove(b, getReasonedMove(b));// Make it so!

	char* output = toString(b);
	printf("%s\n", output);
	free(output);


    }
    int i, j;
    for(i=0; i<b->cols; ++i){
	for(j=0; j < b->rows; j++){
	    deletepoint(b->grid[i][j]);
	}
	free(b->grid[i]);
    }
    deleteboard(b);

    return 0;

}


