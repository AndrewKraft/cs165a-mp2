#ifndef GOMOKU_HPP
#define GOMOKU_HPP
#include <iostream>
#include <set>

class gomoku {
public:
	gomoku(bool l, int n, int m);
	~gomoku();

	void printBoard();

	void userMove();
	void oppMove();

	char getGameState();
	
private:
	char noWin, *board;
	bool l;
	int n, minimax_depth, playerEval, compEval;
	std::set<std::pair<int, int>> successorMoves;

	char checkGG();

	std::pair<int, int> getUserInput();
	void addToBoard(std::pair<int, int> move, bool user);

	bool isMove(std::string move);
	bool isValidMove(std::string move);

	int minimax(bool computerPlaysNext, int depth, int alpha, int beta, std::set<std::pair<int, int>> successors);
	int minimax(bool computerPlaysNext, int depth);



	int eval(bool computerPlaysNext);
	std::pair<int, int> bestMove();

	int score(bool playsNext, int consecAmt, int openEnds);
};

#endif