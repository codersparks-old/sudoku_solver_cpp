/*
 * sudoku_solver.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: sparks
 */

#include <string>
#include <stdexcept>
#include <set>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include "sudoku_solver.hpp"
#include "sudoku_board.hpp"

using sudoku::sudoku_solver;
using sudoku::sudoku_board;
using std::runtime_error;
using std::make_pair;
using std::set_union;
using std::to_string;
using std::inserter;
using std::string;
using std::vector;
using std::pair;
using std::find;
using std::set;
using std::map;

sudoku_board sudoku_solver::solve(const sudoku_board & orig_board) {

	sudoku_board board = orig_board;

	string error_message;
	if (!this->validate_board(board, error_message)) {
		throw runtime_error("Supplied board is invalid: " + error_message);
	}

	bool updated = true;

	while(updated == true) {
		update_possible_values(board);
		updated = update_actual_values(board);
	}

	return board;
}

bool sudoku_solver::validate_board(const sudoku_board & board,
		string & error_message) {

	// first we check the rows
	for (int row = 0; row < 9; row++) {
		vector<int> numbers;

		for (int col = 0; col < 9; col++) {
			int val = board.get_actual_value(row, col);

			if (val > -1) {
				if (find(numbers.begin(), numbers.end(), val)
						!= numbers.end()) {
					error_message = "Error in cell (" + to_string(row) + ","
							+ to_string(col) + ") duplicate value (row): "
							+ to_string(val);
					return false;
				} else {
					numbers.push_back(val);
				}
			}
		}
	}

	// Check the columns
	for (int col = 0; col < 9; col++) {
		vector<int> numbers;

		for (int row = 0; row < 9; row++) {
			int val = board.get_actual_value(row, col);

			if (val > -1) {
				if (find(numbers.begin(), numbers.end(), val)
						!= numbers.end()) {
					error_message = "Error in cell (" + to_string(row) + ","
							+ to_string(col) + ") duplicate value (column): "
							+ to_string(val);
					return false;
				} else {
					numbers.push_back(val);
				}
			}
		}
	}

	// Finally check the block
	for (int block = 0; block < 9; block++) {

		pair<int, int> starting_coords = board.get_starting_coords_for_block(
				block);

		vector<int> numbers;

		for (int row = starting_coords.first; row < (starting_coords.first + 3);
				row++) {

			for (int col = starting_coords.second;
					col < (starting_coords.second + 3); col++) {
				int val = board.get_actual_value(row, col);
				if (val != -1) {
					if (find(numbers.begin(), numbers.end(), val)
							!= numbers.end()) {
						error_message = "Error in cell (" + to_string(row) + ","
								+ to_string(col) + ") duplicate value (block): "
								+ to_string(val);
						return false;
					} else {
						numbers.push_back(val);
					}
				}
			}
		}
	}

	// if we get here we have a valid board
	return true;

}

void sudoku_solver::update_possible_values(sudoku_board & board) {
	set<int> all_nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	map<int, set<int>> row_numbers_map;
	for (int row = 0; row < 9; row++) {
		row_numbers_map.insert(make_pair(row, numbers_in_row(board, row)));
	}

	map<int, set<int>> col_numbers_map;
	for (int col = 0; col < 9; col++) {
		col_numbers_map.insert(make_pair(col, numbers_in_column(board, col)));
	}

	map<int, set<int>> block_numbers_map;
	for (int block = 0; block < 9; block++) {
		block_numbers_map.insert(
				make_pair(block, numbers_in_block(board, block)));
	}

	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			int block = board.get_block_for_coords(row, col);

			set<int> row_nums = row_numbers_map.at(row);
			set<int> col_nums = col_numbers_map.at(col);
			set<int> block_nums = block_numbers_map.at(block);

			// As we have to do an intermediary step
			set<int> temp_nums;
			set<int> impossible_numbs;
			set_union(row_nums.begin(), row_nums.end(), col_nums.begin(),
					col_nums.end(), inserter(temp_nums, temp_nums.begin()));

			set_union(block_nums.begin(), block_nums.end(), temp_nums.begin(),
					temp_nums.end(),
					inserter(impossible_numbs, impossible_numbs.begin()));

			// now loop over thes number to remove from possible vals
			for (int i : impossible_numbs) {
				board.remove_possible_value(row, col, i);
			}

			set<int> possible_nums;
			set_difference(all_nums.begin(), all_nums.end(),
					impossible_numbs.begin(), impossible_numbs.end(),
					inserter(possible_nums, possible_nums.begin()));

			// now add possibles
			for (int i : possible_nums) {
				board.add_possible_value(row, col, i);
			}

		}
	}

}

bool sudoku_solver::update_actual_values(sudoku_board & board) {

	bool updated = false;

	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			if (board.get_actual_value(row, col) != -1) {
				continue;
			}

			int possible_count = 0;
			int actual_value_candidate = -1;

			for (int i = 1; i < 10; i++) {
				if (board.is_possible_value(row, col, i)) {
					++possible_count;
					actual_value_candidate = i;
				}

				// if there are more than one possible values then
				// we can break early as we cannot update the actual
				// value
				if (possible_count > 1) {
					actual_value_candidate = -1;
					break;
				}
			}

			if (actual_value_candidate != -1) {
				board.set_actual_value(row, col, actual_value_candidate);
				updated = true;
			}
		}

	}
	return updated;
}

std::set<int> sudoku_solver::numbers_in_row(const sudoku_board & board,
		int row) {

	set<int> the_set;

	for (int col = 0; col < 9; col++) {
		int val = board.get_actual_value(row, col);
		if (val != -1) {
			the_set.insert(val);
		}
	}

	return the_set;
}

std::set<int> sudoku_solver::numbers_in_column(const sudoku_board & board,
		int column) {
	set<int> the_set;

	for (int row = 0; row < 9; row++) {
		int val = board.get_actual_value(row, column);
		if (val != -1) {
			the_set.insert(val);
		}
	}

	return the_set;
}

std::set<int> sudoku_solver::numbers_in_block(const sudoku_board & board,
		int block) {
	set<int> the_set;

	pair<int, int> starting_coords = board.get_starting_coords_for_block(block);

	for (int row = starting_coords.first; row < (starting_coords.first + 3);
			row++) {

		for (int col = starting_coords.second;
				col < (starting_coords.second + 3); col++) {
			int val = board.get_actual_value(row, col);
			if (val != -1) {
				the_set.insert(val);
			}
		}
	}

	return the_set;

}

