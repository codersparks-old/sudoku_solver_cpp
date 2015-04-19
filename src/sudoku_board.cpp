/*
 * sudoku_board.cpp


 *
 *  Created on: Apr 19, 2015
 *      Author: sparks
 */

#include <stdexcept>
#include <cstdlib>

#include "sudoku_board.hpp"

using std::div;
using std::map;
using std::endl;
using std::pair;
using std::stoi;
using std::string;
using std::to_string;
using std::make_pair;
using std::out_of_range;
using sudoku::sudoku_board;

// Constructors
sudoku_board::sudoku_board() {
	for (int i = 0; i < 81; i++) {

		sudoku_cell cell;

		m_cell_array[i] = cell;
	}
}

/*
 * Takes a line in to initialise the board, expects each
 * cell to be seperated with : and then comma seperated
 * for fields row, column, value
 * e.g.
 * 		1,2,3:2,4,5
 * would initialise cell in row 1, col 2 to the vlaue of
 * 3 and cell in row 2, col 4 to value of 5
 */
sudoku_board::sudoku_board(const std::string & initial_values) {
	map<pair<int, int>, int> initial_value_map;

	std::string::size_type tupple_pos = 0;
	std::string::size_type old_tupple_pos = tupple_pos;

	// there can be more than one tupple so we will loop over
	do {
		tupple_pos = initial_values.find(':', old_tupple_pos);

		string tupple = initial_values.substr(old_tupple_pos,
				tupple_pos - old_tupple_pos);

		// in the "tupple" there will be three parts seperated by commas
		std::string::size_type row_pos = tupple.find(',');
		if (row_pos == std::string::npos) {
			throw out_of_range("Can only find row in tupple: " + tupple);
		}

		string row_str = tupple.substr(0, row_pos);

		// Humans will use 1 as starting row
		int row = std::stoi(row_str) - 1;

		if (row < 0 || row > 8) {
			throw out_of_range(
					"Row must be between 1 and 9 (inclusive), supplied: "
							+ row_str);
		}

		std::string::size_type col_pos = tupple.find(',', row_pos + 1);
		if (col_pos == string::npos) {
			throw out_of_range("Can only find upto col in tupple: " + tupple);

		}

		string col_str = tupple.substr(row_pos + 1, col_pos - 1 - row_pos);
		int col = std::stoi(col_str) - 1;

		if (col < 0 || col > 8) {
			throw out_of_range(
					"Col must be between 1 and 9 (inclusive), supplied: "
							+ col_str);
		}

		string val_str = tupple.substr(col_pos + 1);
		int val = std::stoi(val_str);
		if (val < 1 || val > 9) {
			throw out_of_range(
					"Value must be between 1 and 9 (inclusive): supplied "
							+ val_str);
		}

		// tupple pos is the possition of the comma therefore we want to start from the next char
		old_tupple_pos = tupple_pos + 1;

		auto location = std::make_pair(row, col);

		initial_value_map[location] = val;

	} while (tupple_pos != std::string::npos);

	initialise(initial_value_map);
}

sudoku_board::sudoku_board(const map<pair<int, int>, int> & initial_values) {
	this->initialise(initial_values);
}

void sudoku_board::initialise(const map<pair<int, int>, int> & initial_values) {
	for (int i = 0; i < 81; i++) {
		sudoku_cell cell;
		m_cell_array[i] = cell;
	}

	for (auto const & it : initial_values) {
		int cell_index = this->get_cell_array_index(it.first);
		m_cell_array[cell_index].set_actual_value(it.second);
	}
}

void sudoku_board::add_possible_value(int row, int column, int value) {

	int cell_index = this->get_cell_array_index(row, column);
	m_cell_array[cell_index].update_possible_value(value, true);
}

void sudoku_board::remove_possible_value(int row, int column, int value) {

	int cell_index = this->get_cell_array_index(row, column);
	m_cell_array[cell_index].update_possible_value(value, false);
}

bool sudoku_board::is_possible_value(int row, int column, int value) const {

	int cell_index = this->get_cell_array_index(row, column);
	return m_cell_array[cell_index].is_possible_value(value);
}

int sudoku_board::get_actual_value(int row, int column) const {
	int cell_index = this->get_cell_array_index(row, column);
	return m_cell_array[cell_index].get_actual_value();
}

void sudoku_board::set_actual_value(int row, int column, int value) {

	int cell_index = this->get_cell_array_index(row, column);
	m_cell_array[cell_index].set_actual_value(value);
}

int sudoku_board::get_cell_array_index(const pair<int, int> & location) const {
	return get_cell_array_index(location.first, location.second);
}

int sudoku_board::get_cell_array_index(int row, int column) const {

	return (9 * row) + column;
}

// gets the 0 indexed block for given co-ordinates
int sudoku_board::get_block_for_coords(int row, int column) const {
	int row_quotant = div(row, 3).quot;
	int col_quotant = div(column, 3).quot;

	return (row_quotant * 3) + col_quotant;
}

pair<int, int> sudoku_board::get_starting_coords_for_block(int row,
		int column) const {
	int block = this->get_block_for_coords(row, column);
	return this->get_starting_coords_for_block(block);
}

pair<int, int> sudoku_board::get_starting_coords_for_block(int block) const {

	switch (block) {
	case 0:
		return make_pair(0, 0);
		break;
	case 1:
		return make_pair(0, 3);
		break;
	case 2:
		return make_pair(0, 6);
		break;
	case 3:
		return make_pair(3, 0);
		break;
	case 4:
		return make_pair(3, 3);
		break;
	case 5:
		return make_pair(3, 6);
		break;
	case 6:
		return make_pair(6, 0);
		break;
	case 7:
		return make_pair(6, 3);
		break;
	case 8:
		return make_pair(6, 6);
		break;
	default:
		throw out_of_range(
				"Block number must be between 0 and 8, supplied: " + block);
	}
}

std::ostream & sudoku::operator <<(std::ostream & os, const sudoku_board & in) {
	os << "  _________________________________________" << endl;

	for (int row = 0; row < 9; row++) {

		if (row != 0 && (row % 3) == 0) {
			os << " |_____________|_____________|_____________|" << endl;
		}

		os << " |  _   _   _  |  _   _   _  |  _   _   _  |" << endl;

		for (int col = 0; col < 9; col++) {
			int value = in.get_actual_value(row, col);

			string cell_value = (value == -1 ? "_" : to_string(value));

			if ((col % 3) == 0) {
				os << " | !" << cell_value << "!";
			} else {
				os << " !" << cell_value << "!";
			}

		}
		// finish row with numbers
		os << " |" << endl;
	}

	os << " |_____________|_____________|_____________|" << endl;

	return os;

}
