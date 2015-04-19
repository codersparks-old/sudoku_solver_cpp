/*
 * sudoku_cell.hpp
 *
 *  Created on: Apr 19, 2015
 *      Author: sparks
 */

#pragma once

#include <array>

namespace sudoku {

class sudoku_cell {

private:
	int m_actual_value;
	std::array<bool, 9> m_possible_values;


public:
	sudoku_cell();
	sudoku_cell(int actual_value);
	sudoku_cell(std::array<bool, 9> possible_values);

	void set_actual_value(int value);
	void update_possible_value(int value, bool possible);
	bool is_possible_value(int value) const;
	int get_actual_value() const;
};

} // end of namespace sudoku
