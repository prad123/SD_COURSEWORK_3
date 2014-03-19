#include <stdlib.h>
#include "seatest/seatest.h"
#include "../Src/game.h"

board_type* g_board;

const int rows = 6;
const int cols = 7;

void test_valid_moves(){
	int i = 0;
	for(i=0; i < rows; ++i){
		assert_true(validMove(g_board,4)==1);
		makeMove(g_board, 4);
	}
	assert_true(validMove(g_board,4)==0);
}

void test_moves_left(){
	int i, j;
	assert_true(validMovesLeft(g_board)==1);
	for(i=0; i < cols; ++i){
		for(j=0; j< rows; ++j){
			assert_true(validMovesLeft(g_board)==1);
			makeMove(g_board, i);
		}
	}
	assert_true(validMovesLeft(g_board)==0);
}

void test_undo_move(){
	//if I am able to insert 7th entry in a column
	//after undoing the 8th entry then my test succeeds
		
	int i = 0;
	for(i=0; i < rows; ++i){
		assert_true(validMove(g_board,4)==1);
		makeMove(g_board, 4);
	}

	assert_true(validMove(g_board,4)==0);
	undoMove(g_board);
	assert_true(validMove(g_board,4)==1);
	
}

void test_state(){
	point_type* pt = newPoint(0,0);
	
	//test player 1 option
	setState(pt, PLAYER_ONE);
	assert_int_equal(getState(pt), PLAYER_ONE);
	
	//test player 2 option
	setState(pt, PLAYER_TWO);
	assert_int_equal(getState(pt), PLAYER_TWO);

	free(pt);
	
}

void test_fixture_one(){
	test_fixture_start();
	run_test(test_state);
	run_test(test_valid_moves);
	run_test(test_moves_left);
	run_test(test_undo_move);
	test_fixture_end();
}

void all_tests(){
	test_fixture_one();
}

void test_setup(){
	g_board = createBoard(cols, rows);
}

void test_teardown(){
	free(g_board);
}

int main(int argc, char** argv){
	suite_setup(test_setup);
	suite_teardown(test_teardown);
	run_tests(all_tests);
}
