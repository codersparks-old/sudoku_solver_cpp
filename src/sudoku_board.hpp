/*
 * sudoku_board.hpp
 *
 *  Created on: Apr 19, 2015
 *      Author: sparks
 */

#pragma once

#include <array>
#include <string>
#include <utility>
#include <map>
#include <iostream>

#include "sudoku_cell.hpp"

namespace sudoku {

class sudoku_board {

private:
	std::array<sudoku_cell, 81> m_cell_array;
	void initialise(const std::map<std::pair<int, int>, int> & initial_values);
	int get_cell_array_index(const std::pair<int,int> & location) const;
	int get_cell_array_index(int row, int column) const;

public:
	sudoku_board();
	sudoku_board(const std::string & initial_values);
	sudoku_board(const std::map<std::pair<int, int>, int> & initial_values);

	void add_possible_value(int row, int column, int value);
	void remove_possible_value(int row, int column, int value);
	bool is_possible_value(int row, int column, int value) const;
	int get_actual_value(int row, int column) const;
	void set_actual_value(int row, int column, int value);
	int get_block_for_coords(int row, int column) const;
	std::pair<int, int> get_starting_coords_for_block(int row, int col) const;
	std::pair<int, int> get_starting_coords_for_block(int block) const;

};

std::ostream & operator<<(std::ostream & os, const sudoku_board & in);

} // end of namespace sudoku
