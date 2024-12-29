#ifndef _NUMERICAL_TIC_TAC_TOE_H
#define _4_IN_A_ROW_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

template <typename T>
class NumericalTicTacToeBoard : public Board<T> {
public:
    NumericalTicTacToeBoard();
    ~NumericalTicTacToeBoard();

    bool update_board(int x, int y, T symbol) override;
    void display_board() override;
    bool is_win() override;
    bool is_draw() override;
    bool game_is_over() override;

private:
    std::vector<T> used_numbers;
    bool check_line_sum(int r1, int c1, int r2, int c2, int r3, int c3);
    bool check_all_win_lines();
};

template <typename T>
class NumericalTicTacToePlayer : public Player<T> {
public:
    NumericalTicTacToePlayer(string name, T symbol, bool is_odd_player);
    void getmove(int& x, int& y) override;
    T getnumber();

private:
    bool is_odd_player;
};

template <typename T>
class NumericalTicTacToeRandomPlayer : public RandomPlayer<T> {
public:
    NumericalTicTacToeRandomPlayer(T symbol, bool is_odd_player);
    void getmove(int& x, int& y) override;
    T getnumber();

private:
    bool is_odd_player;
};

// IMPLEMENTATION

// Constructor for NumericalTicTacToeBoard
template <typename T>
NumericalTicTacToeBoard<T>::NumericalTicTacToeBoard() {
    this->rows = this->columns = 3;
    this->board = new T*[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->board[i] = new T[this->columns];
        for (int j = 0; j < this->columns; j++) {
            this->board[i][j] = 0;
        }
    }
    this->n_moves = 0;
    used_numbers.clear();
}

// Destructor to prevent memory leaks
template <typename T>
NumericalTicTacToeBoard<T>::~NumericalTicTacToeBoard() {
    for (int i = 0; i < this->rows; i++) {
        delete[] this->board[i];
    }
    delete[] this->board;
}

// Update board with special rules for Numerical Tic-Tac-Toe
template <typename T>
bool NumericalTicTacToeBoard<T>::update_board(int x, int y, T mark) {
    // Check board boundaries
    if (x < 0 || x >= this->rows || y < 0 || y >= this->columns) {
        return false;
    }

    // If mark is 0, it's an undo move
    if (mark == 0) {
        // Find and remove this number from used numbers
        auto it = std::find(used_numbers.begin(), used_numbers.end(), this->board[x][y]);
        if (it != used_numbers.end()) {
            used_numbers.erase(it);
        }
        this->board[x][y] = 0;
        this->n_moves--;
        return true;
    }

    // Check if cell is empty
    if (this->board[x][y] != 0) {
        return false;
    }

    // Check if number has already been used
    if (std::find(used_numbers.begin(), used_numbers.end(), mark) != used_numbers.end()) {
        return false;
    }

    // Place the mark
    this->board[x][y] = mark;
    used_numbers.push_back(mark);
    this->n_moves++;
    return true;
}

// Display the board
template <typename T>
void NumericalTicTacToeBoard<T>::display_board() {
    for (int i = 0; i < this->rows; i++) {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++) {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << (this->board[i][j] ? std::to_string(this->board[i][j]) : " ") << " |";
        }
        cout << "\n-----------------------------";
    }
    cout << endl;

    // Display used numbers
    cout << "Used Numbers: ";
    for (T num : used_numbers) {
        cout << num << " ";
    }
    cout << endl;
}

// Check for win condition
template <typename T>
bool NumericalTicTacToeBoard<T>::is_win() {
    return check_all_win_lines();
}

// Check if a specific line of three cells sums to 15
template <typename T>
bool NumericalTicTacToeBoard<T>::check_line_sum(int r1, int c1, int r2, int c2, int r3, int c3) {
    T num1 = this->board[r1][c1];
    T num2 = this->board[r2][c2];
    T num3 = this->board[r3][c3];

    // Check if all cells are filled
    if (num1 && num2 && num3) {
        return (num1 + num2 + num3 == 15);
    }
    return false;
}

// Check all possible win lines
template <typename T>
bool NumericalTicTacToeBoard<T>::check_all_win_lines() {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (check_line_sum(i, 0, i, 1, i, 2)) return true;
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (check_line_sum(0, j, 1, j, 2, j)) return true;
    }

    // Check diagonals
    if (check_line_sum(0, 0, 1, 1, 2, 2)) return true;
    if (check_line_sum(0, 2, 1, 1, 2, 0)) return true;

    return false;
}

// Check for draw condition
template <typename T>
bool NumericalTicTacToeBoard<T>::is_draw() {
    return (this->n_moves == 9 && !is_win());
}

// Check if game is over
template <typename T>
bool NumericalTicTacToeBoard<T>::game_is_over() {
    return is_win() || is_draw();
}

// Constructor for NumericalTicTacToePlayer
template <typename T>
NumericalTicTacToePlayer<T>::NumericalTicTacToePlayer(string name, T symbol, bool is_odd_player)
        : Player<T>(name, symbol), is_odd_player(is_odd_player) {}

// Get move from human player
template <typename T>
void NumericalTicTacToePlayer<T>::getmove(int& x, int& y) {
    cout << "\nPlease enter the row and column (0 to 2) separated by space: ";
    cin >> x >> y;
}

// Get number from human player
template <typename T>
T NumericalTicTacToePlayer<T>::getnumber() {
    T number;
    while (true) {
        cout << "Enter a " << (is_odd_player ? "odd" : "even") << " number (1-9): ";
        cin >> number;

        // Validate number based on player type (odd/even)
        if ((is_odd_player && number % 2 == 1 && number >= 1 && number <= 9) ||
            (!is_odd_player && number % 2 == 0 && number >= 2 && number <= 8)) {
            return number;
        }
        cout << "Invalid number. Try again.\n";
    }
}

// Constructor for NumericalTicTacToeRandomPlayer
template <typename T>
NumericalTicTacToeRandomPlayer<T>::NumericalTicTacToeRandomPlayer(T symbol, bool is_odd_player)
        : RandomPlayer<T>(symbol), is_odd_player(is_odd_player) {
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
}

// Get random move for computer player
template <typename T>
void NumericalTicTacToeRandomPlayer<T>::getmove(int& x, int& y) {
    x = rand() % this->dimension;
    y = rand() % this->dimension;
}

// Get random number for computer player
template <typename T>
T NumericalTicTacToeRandomPlayer<T>::getnumber() {
    while (true) {
        T number = (is_odd_player ? 1 + 2 * (rand() % 5) : 2 * (1 + rand() % 4));
        return number;
    }
}

#endif //_NUMERICAL_TIC_TAC_TOE_H