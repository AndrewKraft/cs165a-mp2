#ifndef GOMOKU_HPP
#define GOMOKU_HPP
#include <iostream>

class gomoku {
public:
	typedef unsigned char uchar;

	gomoku(bool l, int n);
	~gomoku();

	void printBoard();

	void userMove();
	void oppMove();

	uchar getGameState();
	
private:
	uchar noWin, * board;
	int n, *usrwin, *oppwin;
	bool l;

	void checkGG(std::pair<int, int> lastMove, bool isUsrMove);

	std::pair<int, int> getUserInput();
	void addToBoard(std::pair<int, int> move);

	bool isMove(std::string move);
	bool isValidMove(std::string move);

	int eval(std::pair<int, int> move);
	std::pair<int, int> best();

};

#endif