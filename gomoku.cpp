#include "gomoku.hpp"

using namespace std;

gomoku::gomoku(bool l, int n) {
	this->n = n;
	this->l = l;
	noWin = 0;
	board = new uchar[n*n];
	for(int y = 0; y < n; y++) {
		for(int x = 0; x < n; x++) {
			board[x+n*y] = 0;
		}
	}

	usrwin = new int[5*n-17];
	oppwin = new int[5*n-17];
	for(int i = 0; i < 5*n-17; i++) {
		usrwin[i]==0;
		oppwin[i]==0;
	}
}

gomoku::~gomoku() {
	delete [] board;
	delete [] usrwin;
	delete [] oppwin;
}

void gomoku::printBoard() {
	wstring assets = L" XO";
	for(int y = -1; y < n; y++) {
		for(int x = -1; x < n; x++) {
			if(x==-1 && y==-1) cout << ' ';
			else if(x==-1) cout << y+1;
			else if(y==-1) cout << (char)(x+'a');
			else cout << (char)assets[board[x+n*y]];
		}
		cout << endl;
	}
}

gomoku::uchar gomoku::getGameState() {
	return noWin;
}

void gomoku::addToBoard(pair<int, int> move) {
	cout << "Move Played: " << (char)('a'+move.first) << (move.second+1) << endl;
	board[move.first+n*move.second] = l?2:1;
}

bool gomoku::isMove(string move) {
	if(move[0]>='a'&&move[0]<='z') {
		try {
			stoi(move.substr(1));
			return true;
		} catch(invalid_argument) {};
	}
	return false;
}

bool gomoku::isValidMove(string move) {
	int x = move[0]-'a';
	int y = stoi(move.substr(1))-1;
	if(y<0||y>=n||x>=n) return false;
	return board[n*y+x]==0;
}

pair<int, int> gomoku::getUserInput() {
	bool good=false;
	string usrInput;
	pair<int, int> move;

	while(!good) {
		cout << ">";
		cin >> usrInput;
		if(!isMove(usrInput))
		{
			cout << "Error: " << usrInput << " is not recognized as a move, please try again" << endl;
		}
		else if(!isValidMove(usrInput))
		{
			cout << "Error: move " << usrInput << " cannot be played, please try another move" << endl;
		}
		else good=true;
	}
	cout << endl;
	move.first = usrInput[0]-'a';
	move.second = stoi(usrInput.substr(1))-1;
	return move;
}

void gomoku::userMove() {
	cout << (l?"Light":"Dark") << " player (human) plays now" << endl;
	pair<int, int> move = getUserInput();
	addToBoard(move);
	checkGG(move, true);
}

void gomoku::oppMove() {
	cout << (l?"Dark":"Light") << " player (computer) plays now" << endl;
	pair<int,int> move = best();
	addToBoard(move);
	checkGG(move, false);
}

int gomoku::eval(pair<int, int> move) {
	return 0;
}

pair<int, int> gomoku::best() {
	pair<int, int> out(0, 0);
	for(int y = 0; y < n; y++) {
		for(int x = 0; x < n; x++) {
			if(board[n*y+x]==0) {
				out = {x, y};
				return out;
			}
		}
	}
	return out;
}

void gomoku::checkGG(pair<int, int> lastMove, bool isUsrMove) {
	int* tmp = oppwin;
	if(isUsrMove) tmp = usrwin;
	usrwin[lastMove.first]++;
	usrwin[lastMove.second+n]++;
}