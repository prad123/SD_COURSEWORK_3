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
	assert_int_equal(PLAYER_ONE, getState(pt));
	
	//test player 2 option
	setState(pt, PLAYER_TWO);
	assert_int_equal(PLAYER_TWO, getState(pt));

	free(pt);
	
}

void test_get_score(){
	point_type pt[4];

	point_type* ppt[4];
	ppt[0] = &pt[0];
	ppt[1] = &pt[1];
	ppt[2] = &pt[2];
	ppt[3] = &pt[3];

	setState(&pt[0], PLAYER_ONE);
	setState(&pt[1], PLAYER_ONE);
	setState(&pt[2], PLAYER_ONE);
	setState(&pt[3], PLAYER_ONE);
	
	assert_int_equal(4, getScore(ppt));
	
	//printf("Score %ld\n", getScore(ppt));
	setState(&pt[0], PLAYER_TWO);
	setState(&pt[1], PLAYER_TWO);
	setState(&pt[2], PLAYER_TWO);
	setState(&pt[3], PLAYER_TWO);
	
	//printf("Score %ld\n", getScore(ppt));
	assert_int_equal(-4, getScore(ppt));
	
}

void test_fixture_moves(){
	test_fixture_start();
	run_test(test_valid_moves);
	run_test(test_moves_left);
	run_test(test_undo_move);
	test_fixture_end();
}

void test_fixture_state(){
	test_fixture_start();
	run_test(test_state);
	run_test(test_get_score);
	test_fixture_end();
}

void all_tests(){
	test_fixture_moves();
	test_fixture_state();
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
