// X_O_game.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

//Global constants 
const char X = 'X';
const char O = 'O';
const char DRAW = 'D';
const char CONTINUE = 'C';
const char EMPTY = ' ';

//Functions prototypes
void instructions();
char askMove();
char humanPiece();
char opposite(char piece);
void displayBoard(const vector<char>& vec);
int askNumber();
bool isLegal(const vector<char>& board, int move);
int humanMove(const vector<char>& board);
char checkWinner(const vector<char>& board);
void announceResult(char winner, char human, char computer);
int computerMove(vector<char> board, char human, char computer);

int main()
{
	//Variables
	char human;
	char computer; 
	int move;
	char turn = X; //Who has X, begins the first

	//Playing board. At the beginning there are 9 of empty squares
	vector<char> board(9, EMPTY); 

	//Printing the game instructions 
	instructions();

	//Choosing a turn and a piece
	human = humanPiece();
	computer = opposite(human);

	while (checkWinner(board) == CONTINUE) {
		
		//Check the turn
		if (turn == human) {
			move = humanMove(board);
			board[move] = human;
			//change the turn
			turn = opposite(human);
		}
		else {
			move = computerMove(board, human, computer);
			board[move] = computer; 
			//change the turn
			turn = opposite(computer);
		}

		displayBoard(board);
	}
	
	announceResult(checkWinner(board), human, computer);

    return 0;
}// ********************** End of main ******************************

void instructions() {
	cout << "\n\n\n\nEnter a number to make your move \n\n\n";
	cout << "-------------\n";
	cout << "| 0 | 1 | 2 | \n\n";
	cout << "-------------\n";
	cout << "| 3 | 4 | 5 | \n\n";
	cout << "-------------\n";
	cout << "| 6 | 7 | 8 | \n\n";
	cout << "-------------\n\n\n";
}

char askMove() {
	cout << "Would you like to be the first?\n";
	char answer;
	do {
		cout << "Please, use 'y' or 'n'\n";
		cin >> answer;
	} while (answer != 'y' && answer != 'n');

	return answer;
}

char humanPiece() {
	char who = askMove();

	if (who == 'y') {
		cout << "\n\nI hope it will help you\n\n";
		return X; 
	}
	else {
		cout << "\n\nYou are really brave!!!\n\n";
		return O;
	}
}

char opposite(char piece) {
	if (piece == X) {
		return O;
	}
	else {
		return X;
	}
}

//We pass a constant reference since we don't need to change the board
void displayBoard(const vector<char>& vec) {
	
	cout << "-------------\n";
	cout << "| " << vec[0] << " | " << vec[1] << " | " << vec[2] << " | \n\n";
	cout << "-------------\n";
	cout << "| " << vec[3] << " | " << vec[4] << " | " << vec[5] << " | \n\n";
	cout << "-------------\n";
	cout << "| " << vec[6] << " | " << vec[7] << " | " << vec[8] << " | \n\n";
	cout << "-------------\n\n\n";
}

int askNumber() {
	
	int move;
	
	do {
		cout << "Please, enter a number between 0 and 8 in order to make your move\n\n";
		cin >> move;
	} while (move > 8 || move < 0);

	return move;
}

bool isLegal(const vector<char>& board, int move) {
	return board[move] == EMPTY; //it's possible to make a move if the square is empty
}

int humanMove(const vector<char>& board)
{
	int move = askNumber();
	while (!isLegal(board, move)) {
		displayBoard(board);
		cout << "Stupid, human being! This square is occuppied!!!\n\n";
		move = askNumber();
	}
	return move;
}

char checkWinner(const vector<char>& board) {
	
	int winnigRows[8][3] = {{0, 1, 2},
							{3, 4, 5},
							{6, 7, 8},
							{0, 3, 6},
							{1, 4, 7},
							{2, 5, 8},
							{0, 4, 8},
							{2, 4, 6} };

	//Checking if there's a winner
	for (unsigned int i = 0; i < 8; i++) {//There's 8 winning rows
		//We check that the first element is not empty and three elements are the same if
		//we place them according winning rows
		if (board[winnigRows[i][0]] != EMPTY &&
			board[winnigRows[i][0]] == board[winnigRows[i][1]] &&
			board[winnigRows[i][1]] == board[winnigRows[i][2]]) {

			//We return the first element of the winning row
			return board[winnigRows[i][0]];
		}
	}//End of FOR

	//Checking for empty squares
	if (count(board.begin(), board.end(), EMPTY) == 0) {
		return DRAW;
	}

	//if there's no a winner and there are empty squares, we'll continue the game
	return CONTINUE;
	
}//End of checkWinner

void announceResult(char winner, char human, char computer) {
	if (winner == human) {
		cout << "Congratulations!!! human being!!!\n\n";
	}

	if (winner == computer) {
		cout << "Computers are better than human beings!!!\n\n";
	}

	if (winner == DRAW) {
		cout << "It's a draw!!!\n\n";
	}
	
}

int computerMove(vector<char> board, char human, char computer) {
	
	int move;
	bool found = false;//Flag. False if the move is not found

	//Checking if a computer can win in just one move
	for (unsigned int i = 0; i < 9; i++) {
		if (isLegal(board, i)) {
			board[i] = computer;
			if (checkWinner(board) == computer) {
				move = i;
				found = true;
			}
			//Undo changes
			board[i] = EMPTY; 

		}
	}

	//The computer checks if a human can win in one move
	if (!found) {
		for (unsigned int i = 0; i < 9; i++) {
			if (isLegal(board, i)) {
				board[i] = human;
				if (checkWinner(board) == human) {
					move = i;
					found = true;
				}
				//Undo changes
				board[i] = EMPTY;
			}
		}
	}

	//Check for good moves
	if (!found) {
		//The best move is in the center, then in the middle of lines 
		//and the worst ones are in the corners
		const int goodMoves[9] = { 4, 1, 3, 5, 7, 0, 2, 6, 8 };

		//Loop through good moves
		for (unsigned int i = 0; i < 9; i++) {
			if (isLegal(board, goodMoves[i])) {
				return goodMoves[i];
			}
		}
	}

	return move;

}//End of computerMove