#include <iostream>
#include "BoardGame_Classes.h"
#include "numerical_tic_tac_toe.h"  // Add this line

using namespace std;

int main() {
    int gameChoice;
    cout << "Choose a game:\n";
    cout << "1. Classic Tic-Tac-Toe\n";
    cout << "2. Four-in-a-Row\n";
    cout << "3. Numerical Tic-Tac-Toe\n";
    cin >> gameChoice;

    switch(gameChoice) {
        // ... previous game cases ...

        case 3: {
            int choice;
            Player<int>* players[2];
            NumericalTicTacToeBoard<int>* B = new NumericalTicTacToeBoard<int>();
            string player1Name, player2Name;

            cout << "Welcome to Numerical Tic-Tac-Toe Game. :)\n";

            // Set up player 1 (odd numbers)
            cout << "Enter Player 1 (Odd Numbers) name: ";
            cin >> player1Name;
            cout << "Choose Player 1 type:\n";
            cout << "1. Human\n";
            cout << "2. Random Computer\n";
            cin >> choice;

            switch(choice) {
                case 1:
                    players[0] = new NumericalTicTacToePlayer<int>(player1Name, 'X', true);
                    break;
                case 2:
                    players[0] = new NumericalTicTacToeRandomPlayer<int>('X', true);
                    break;
                default:
                    cout << "Invalid choice for Player 1. Exiting the game.\n";
                    return 1;
            }

            // Set up player 2 (even numbers)
            cout << "Enter Player 2 (Even Numbers) name: ";
            cin >> player2Name;
            cout << "Choose Player 2 type:\n";
            cout << "1. Human\n";
            cout << "2. Random Computer\n";
            cin >> choice;

            switch(choice) {
                case 1:
                    players[1] = new NumericalTicTacToePlayer<int>(player2Name, 'O', false);
                    break;
                case 2:
                    players[1] = new NumericalTicTacToeRandomPlayer<int>('O', false);
                    break;
                default:
                    cout << "Invalid choice for Player 2. Exiting the game.\n";
                    return 1;
            }

            // Create the game manager and run the game
            GameManager<int> numerical_tic_tac_toe_game(B, players);

            // Custom game loop to handle number selection
            int x, y;
            int currentPlayer = 0;
            while (!B->game_is_over()) {
                B->display_board();

                // Get board position
                players[currentPlayer]->getmove(x, y);

                // Get number to place
                int number;
                if (dynamic_cast<NumericalTicTacToePlayer<int>*>(players[currentPlayer])) {
                    number = dynamic_cast<NumericalTicTacToePlayer<int>*>(players[currentPlayer])->getnumber();
                } else {
                    number = dynamic_cast<NumericalTicTacToeRandomPlayer<int>*>(players[currentPlayer])->getnumber();
                }

                // Try to update board
                if (B->update_board(x, y, number)) {
                    if (B->is_win()) {
                        B->display_board();
                        cout << players[currentPlayer]->getname() << " wins!\n";
                        break;
                    }
                    if (B->is_draw()) {
                        B->display_board();
                        cout << "Draw!\n";
                        break;
                    }
                    currentPlayer = 1 - currentPlayer;
                }
            }

            // Clean up
            delete B;
            for (int i = 0; i < 2; ++i) {
                delete players[i];
            }
            break;
        }
    }

    return 0;
}