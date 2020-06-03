#include "gomoku.hpp"

using namespace std;

void runGame(bool l, int n) {
	gomoku game(l, n);
	while(game.getGameState()==0) {
		game.printBoard();
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
	int n = 11;
	string temp;

	for(int i = 1; i < argc; i++) {
		temp = argv[i];
		if(temp=="-n") n = atoi(argv[i+1]);
		if(temp=="-l") l = true;
	}

	if(n < 5 || n > 26) {
		cerr << "Board size must be in the range 5 - 26" << endl;
		exit(0);
	}

	runGame(l, n);

	return 0;
}