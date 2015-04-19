/*
 * main.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: sparks
 */

#include <iostream>
#include <stdexcept>
#include <string>

#include "sudoku_board.hpp"
#include "sudoku_solver.hpp"

using sudoku::sudoku_board;
using sudoku::sudoku_solver;
using std::string;
using std::getline;
using std::cout;
using std::endl;
using std::cerr;

int main() {

	sudoku_solver solver;

	for(string line; getline(std::cin, line);) {
		try {

			sudoku_board board(line);

			cout << "Solving board: " << endl;
			cout << board << endl;
			sudoku_board solved_board = solver.solve(board);
			cout << "Solved board: " << endl;
			cout << solved_board << endl;

			string message;
			cout << "Solved board validation: " << (solver.validate_board(solved_board, message)? "Valid" : "Not valid: " + message) << endl;

		} catch (std::runtime_error & err) {
			cerr << "Cannot validate board, error: " << err.what() << endl;
		}
	}

	return 0;
}
