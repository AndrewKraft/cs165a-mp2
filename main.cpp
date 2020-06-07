#include "gomoku.hpp"

using namespace std;

void runGame(bool l, int n, int m) {
	gomoku game(l, n, m);
	while(game.getGameState()==0) {
		//game.printBoard();
		if(l) {
			game.oppMove();
			l=false;
		} else {
			game.userMove();
			l=true;
		}
	}

}

int main(int argc, char** argv) {
	bool l = false;
	int n = 11, m = 4;
	string temp;

	for(int i = 1; i < argc; i++) {
		temp = argv[i];
		if(temp=="-n") n = atoi(argv[i+1]);
		if(temp=="-l") l = true;
		if(temp=="-m") m = atoi(argv[i+1]);
	}

	if(n < 5 || n > 26) {
		cerr << "Board size must be in the range 5 - 26" << endl;
		exit(0);
	}

	if(m < 1) {
		cerr << "Minimax depth cannot be less than 1" << endl;
		exit(0);
	}

	runGame(l, n, m);

	return 0;
}