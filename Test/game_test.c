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
	
	setState(&pt[0], PLAYER_TWO);
	setState(&pt[1], PLAYER_TWO);
	setState(&pt[2], PLAYER_TWO);
	setState(&pt[3], PLAYER_TWO);
	
	assert_int_equal(-4, getScore(ppt));
	
}

//helper function to simulate winning moves
void winning_move(int expected_winner){
	assert_true(winnerIs(g_board) == 0);

	makeMove(g_board, 0); //player one moves
	makeMove(g_board, 1); //player two moves
	assert_true(winnerIs(g_board) == 0);

	makeMove(g_board, 0); //player one moves
	makeMove(g_board, 2); //player two moves
	assert_true(winnerIs(g_board) == 0);

	makeMove(g_board, 0); //player one moves
	makeMove(g_board, 3); //player two moves

	assert_true(winnerIs(g_board) == 0);
	makeMove(g_board, 0); //player one moves
	makeMove(g_board, 3); //player two moves

	assert_int_equal(expected_winner, winnerIs(g_board));
}

void test_winner_is_player_one(){
	//we simulate player one winning
	//by making winning moves	
	winning_move(PLAYER_ONE);
}

void test_winner_is_player_two(){
	makeMove(g_board, 6); //dummy move by player 1
	winning_move(PLAYER_TWO);
}

void test_horizontal_win(){
	assert_true(winnerIs(g_board) == 0);

	makeMove(g_board, 0); //player one moves
	makeMove(g_board, 4); //player two moves
	assert_true(winnerIs(g_board) == 0);

	makeMove(g_board, 1); //player one moves
	makeMove(g_board, 5); //player two moves
	assert_true(winnerIs(g_board) == 0);

	makeMove(g_board, 2); //player one moves
	makeMove(g_board, 6); //player two moves

	assert_true(winnerIs(g_board) == 0);
	makeMove(g_board, 3); //player one moves
	makeMove(g_board, 6); //player two moves

	assert_int_equal(PLAYER_ONE, winnerIs(g_board));
}

void test_diagonal_win(){
	assert_true(winnerIs(g_board) == 0);

	makeMove(g_board, 0); //player one moves
	makeMove(g_board, 1); //player two moves
	assert_true(winnerIs(g_board) == 0);

	makeMove(g_board, 1); //player one moves
	makeMove(g_board, 2); //player two moves
	assert_true(winnerIs(g_board) == 0);

	makeMove(g_board, 3); //player one moves
	makeMove(g_board, 2); //player two moves

	assert_true(winnerIs(g_board) == 0);
	makeMove(g_board, 2); //player one moves
	makeMove(g_board, 3); //player two moves
	
	assert_true(winnerIs(g_board) == 0);
	makeMove(g_board, 4); //player one moves
	makeMove(g_board, 3); //player two moves

	assert_true(winnerIs(g_board) == 0);
	makeMove(g_board, 3); //player one moves

	assert_int_equal(PLAYER_ONE, winnerIs(g_board));
}

void test_reasoned_move(){
	makeMove(g_board,0);
	assert_int_equal(3, getReasonedMove(g_board));
	
	makeMove(g_board,1);
	assert_int_equal(6, getReasonedMove(g_board));
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

void test_fixture_winner(){
	test_fixture_start();
	run_test(test_winner_is_player_one);
	test_fixture_end();
	
	test_fixture_start();
	run_test(test_winner_is_player_two);
	test_fixture_end();


	test_fixture_start();
	run_test(test_horizontal_win);
	test_fixture_end();

	test_fixture_start();
	run_test(test_diagonal_win);
	test_fixture_end();
}

void test_fixture_game_logic(){
	test_fixture_start();
	run_test(test_reasoned_move);
	test_fixture_end();	
}

void all_tests(){
	test_fixture_moves();
	test_fixture_state();
	test_fixture_winner();
	test_fixture_game_logic();
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
