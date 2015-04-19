/*
 * sudoku_cell.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: sparks
 */

#include <stdexcept>
#include <string>

#include "sudoku_cell.hpp"

using std::to_string;
using std::out_of_range;
using sudoku::sudoku_cell;

sudoku_cell::sudoku_cell() :
		m_actual_value { -1 }, m_possible_values { true, true, true, true, true,
				true, true, true, true } {
}

sudoku_cell::sudoku_cell(int actual_value) :
		m_actual_value { -1 }, m_possible_values { false, false, false, false,
				false, false, false, false, false } {

	// We initially set to -1 and then update it using
	// the set_actual_value function
	this->set_actual_value(actual_value);
}

sudoku_cell::sudoku_cell(std::array<bool, 9> possible_values) :
		m_actual_value { -1 }, m_possible_values(possible_values) {
}

void sudoku_cell::set_actual_value(int value) {
	if (value < 1 || value > 9) {
		throw out_of_range(
				"Actual value must be between 1 and 9 supplied: "
						+ to_string(value));
	}

	for (int i = 0; i < 9; i++) {
		m_possible_values[i] = false;
	}

	m_possible_values[value - 1] = true;

	m_actual_value = value;
}

void sudoku_cell::update_possible_value(int value, bool possible) {
	if (value < 1 || value > 9) {
		throw out_of_range(
				"Actual value must be between 1 and 9 supplied: "
						+ to_string(value));
	}

	m_possible_values[value - 1] = possible;
}

bool sudoku_cell::is_possible_value(int value) const {
	if (value < 1 || value > 9) {
		throw out_of_range(
				"Actual value must be between 1 and 9 supplied: "
						+ to_string(value));
	}

	return m_possible_values[value -1 ];
}

int sudoku_cell::get_actual_value() const {
	return m_actual_value;
}
