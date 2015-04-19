/*
 * sudoku_solver.hpp
 *
 *  Created on: Apr 19, 2015
 *      Author: sparks
 */

#pragma once

#include <set>
#include <string>

#include "sudoku_board.hpp"

namespace sudoku {

class sudoku_solver {

private:
	void update_possible_values(sudoku_board & board);
	bool update_actual_values(sudoku_board & board);
	std::set<int> numbers_in_row(const sudoku_board & board, int row);
	std::set<int> numbers_in_column(const sudoku_board & board, int column);
	std::set<int> numbers_in_block(const sudoku_board & board, int block);

public:
	sudoku_board solve(const sudoku_board & board);
	bool validate_board(const sudoku_board & board, std::string & error_message);

};


}
