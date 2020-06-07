#include "gomoku.hpp"
#include <chrono>

using namespace std;

gomoku::gomoku(bool l, int n, int m) {
	this->n = n;
	this->l = l;
	minimax_depth=m;
	playerEval=compEval=0;
	noWin = 0;
	board = new char[n*n];
	for(int y = 0; y < n; y++) {
		for(int x = 0; x < n; x++) {
			board[x+n*y] = 0;
		}
	}
}

gomoku::~gomoku() {
	delete [] board;
}

void gomoku::printBoard() {
	string assets = l?".LD":".DL";
	for(int y = -1; y < n; y++) {
		for(int x = -1; x < n; x++) {
			if(x==-1 && y==-1) cout << "  ";
			else if(x==-1) cout << (y+1>9 ? to_string(y+1) : to_string(y+1) + ' ');
			else if(y==-1) cout << (char)(x+'a');
			else cout << (char)assets[board[x+n*y]];
		}
		cout << endl;
	}
}

char gomoku::getGameState() {
	return noWin;
}

void gomoku::addToBoard(pair<int, int> move, bool user) {
	cout << "Move Played: " << (char)('a'+move.first) << (move.second+1) << endl;
	board[move.first+n*move.second] = user ? 1:2;
	successorMoves.erase(move);

	for(int y = move.second-1<0 ? 0 : move.second-1; y < move.second+2 && y<n; y++) {
		for(int x = move.first-1<0 ? 0 : move.first-1; x<move.first+2 && x<n; x++) {
			if(board[n*y+x]==0) {
				successorMoves.insert(pair<int, int>(x, y));
			}
		}
	} 
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
	addToBoard(move, true);
}

void gomoku::oppMove() {
	cout << (l?"Dark":"Light") << " player (computer) plays now" << endl;
	pair<int,int> move = bestMove();
	addToBoard(move, false);
}

int gomoku::eval(bool computerPlaysNext) {
	/*check all rows, columns, and diagonals for threat patterns
	 * possible threats:
	 * 0: "bbbbb" - win - value = 1<<20
	 * 1: "bbbb" - irrefutable = 1<<10
	 * 2: "bb bb", "bbb", "bb b", "b bb", "wbbbb", "bbbbw" - requiring immediate refutation = 1<<5
	 * 3: "bb", "wbbb", "b  b", "b b" - potential to become a threat - 5
	 * 4: "b" - not a threat value = 1
	 */

	int pscore=0, cscore=0;
	int yboard[n*n];
	int row1=0, row2=0, oe1=0, oe2=0;
	
	for(int y = 0; y < n; y++) {
		for(int x = 0; x < n; x++) {
			char tmp=board[n*y+x];
			yboard[n*x+y]=tmp;
			if(tmp==0) {
				if(row1>0) {
					oe1++;
					pscore+=score(!computerPlaysNext, row1, oe1);
					row1=0;
				}
				if(row2>0) {
					oe2++;
					cscore+=score(computerPlaysNext, row2, oe2);
					row2=0;
				}
				oe1=oe2=1;
			}
			if(tmp==1) {
				if(row2>0) {
					cscore+=score(computerPlaysNext, row2, oe2);
					row2=0;
				}
				oe2=0;
				row1++;
			} 
			if(tmp==2) {
				if(row1>0) {
					pscore+=score(!computerPlaysNext, row1, oe1);
					row1=0;
				}
				oe1=0;
				row2++;
			}
		}
		if(row1>0) pscore+=score(!computerPlaysNext, row1, oe1);
		if(row2>0) cscore+=score(computerPlaysNext, row2, oe2);
		if(pscore>2<<10 || cscore>2<<10) return cscore-pscore;
		row1=row2=oe1=oe2=0;
	}

	for(int y = 0; y < n; y++) {
		for(int x = 0; x < n; x++) {
			char tmp=yboard[n*y+x];
			if(tmp==0) {
				if(row1>0) {
					oe1++;
					pscore+=score(!computerPlaysNext, row1, oe1);
					row1=0;
				}
				if(row2>0) {
					oe2++;
					cscore+=score(computerPlaysNext, row2, oe2);
					row2=0;
				}
				oe1=oe2=1;
			}
			if(tmp==1) {
				if(row2>0) {
					cscore+=score(computerPlaysNext, row2, oe2);
					row2=0;
				}
				oe2=0;
				row1++;
			} 
			if(tmp==2) {
				if(row1>0) {
					pscore+=score(!computerPlaysNext, row1, oe1);
					row1=0;
				}
				oe1=0;
				row2++;
			}
		}
		if(row1>0) pscore+=score(!computerPlaysNext, row1, oe1);
		if(row2>0) cscore+=score(computerPlaysNext, row2, oe2);
		if(pscore>2<<10 || cscore>2<<10) return cscore-pscore;
		row1=row2=oe1=oe2=0;
	}

	for(int x = -n+5; x < n-4; x++) {
		int y = 0, tmpx=x;
		while(y < n && tmpx < n) {
			if(tmpx>=0) {
				char tmp = board[n*y+tmpx];
				if(tmp==0) {
					if(row1>0) {
						oe1++;
						pscore+=score(!computerPlaysNext, row1, oe1);
						row1=0;
					}
					if(row2>0) {
						oe2++;
						cscore+=score(computerPlaysNext, row2, oe2);
						row2=0;
					}
					oe1=oe2=1;
				}
				if(tmp==1) {
					if(row2>0) {
						cscore+=score(computerPlaysNext, row2, oe2);
						row2=0;
					}
					oe2=0;
					row1++;
				} 
				if(tmp==2) {
					if(row1>0) {
						pscore+=score(!computerPlaysNext, row1, oe1);
						row1=0;
					}
					oe1=0;
					row2++;
				}
			}
			tmpx++;
			y++;
		}
		if(row1>0) pscore+=score(!computerPlaysNext, row1, oe1);
		if(row2>0) cscore+=score(computerPlaysNext, row2, oe2);
		if(pscore>2<<10 || cscore>2<<10) return cscore-pscore;
		row1=row2=oe1=oe2=0;
	}
	
	for(int x = -n+5; x < n-4; x++) {
		int y = n-1, tmpx=x;
		while(y >= 0 && tmpx < n) {
			if(tmpx>=0) {
				char tmp = board[n*y+tmpx];
				if(tmp==0) {
					if(row1>0) {
						oe1++;
						pscore+=score(!computerPlaysNext, row1, oe1);
						row1=0;
					}
					if(row2>0) {
						oe2++;
						cscore+=score(computerPlaysNext, row2, oe2);
						row2=0;
					}
					oe1=oe2=1;
				}
				if(tmp==1) {
					if(row2>0) {
						cscore+=score(computerPlaysNext, row2, oe2);
						row2=0;
					}
					oe2=0;
					row1++;
				} 
				if(tmp==2) {
					if(row1>0) {
						pscore+=score(!computerPlaysNext, row1, oe1);
						row1=0;
					}
					oe1=0;
					row2++;
				}
			}
			tmpx++;
			y--;
		}
		if(row1>0) pscore+=score(!computerPlaysNext, row1, oe1);
		if(row2>0) cscore+=score(computerPlaysNext, row2, oe2);
		if(pscore>2<<10 || cscore>2<<10) return cscore-pscore;
		row1=row2=oe1=oe2=0;
	}

	return cscore-pscore;
}

int gomoku::score(bool playsNext, int consecAmt, int openEnds) {

	if(consecAmt>=5) return 1<<20;
	if(openEnds==0 || consecAmt==0) return 0;
	switch(consecAmt) {
		case 4: 
			switch(openEnds) {
				case 1:
					if(playsNext) return 1<<19;
					return 100;
				case 2:
					return 1<<19;
			}
		case 3:
			switch(openEnds) {
				case 1:
					if(playsNext) return 15;
					return 10;
				case 2:
					if(playsNext) return 2<<10;
					return 100;
			}
		case 2: 
			switch(openEnds) {
				case 1:
					return 5;
				case 2:
					return 10;
			}
		case 1:
			switch(openEnds) {
				case 1:
					return 1;
				case 2:
					return 2;
			}
		default: return 0;
	}
}

int gomoku::minimax(bool computerPlaysNext, int depth) {
	if(depth==1) return eval(computerPlaysNext);

	if(computerPlaysNext) {
		int max=-1<<30;
		for(int y = 0; y < n; y++) {
			for(int x = 0; x < n; x++) {
				if(board[y*n+x]==0) {
					board[n*y+x]=2;
					int tmp = minimax(false, depth-1);
					board[n*y+x]=0;
					if(tmp > max) max = tmp;
				}
			}
		}
		return max;
	} else {
		int min=1<<30;
		for(int y = 0; y < n; y++) {
			for(int x = 0; x < n; x++) {
				if(board[y*n+x]==0) {
					board[n*y+x]=1;
					int tmp = minimax(true, depth-1);
					board[n*y+x]=0;
					if(tmp < min) min = tmp;
				}
			}
		}
		return min;
	}
}

int gomoku::minimax(bool computerPlaysNext, int depth, int alpha, int beta, set<pair<int, int>> successors) {
	if(depth==1) {
		int tmp = eval(computerPlaysNext);
		return tmp;
	}
	
	if(computerPlaysNext) {
		for(auto i : successors) {
			if(board[n*i.second+i.first]==0) {
				board[n*i.second+i.first]=2;
				set<pair<int, int>> moves(successors);
				for(int y = i.second-1<0 ? 0 : i.second-1; y < i.second+2 && y<n; y++) {
					for(int x = i.first-1<0 ? 0 : i.first-1; x<i.first+2 && x<n; x++) {
						if(board[n*y+x]==0) moves.insert(pair<int, int>(x, y));
					}
				}
				int tmp = minimax(false, depth-1, alpha, beta, moves);
				board[n*i.second+i.first]=0;
				if(tmp > alpha) alpha=tmp;
				if(alpha >= beta) return alpha;
			}
		}
		return alpha;
	} else {
		for(auto i : successors) {
			if(board[n*i.second+i.first]==0) {
				board[n*i.second+i.first]=1;
				set<pair<int, int>> moves(successors);
				for(int y = i.second-1<0 ? 0 : i.second-1; y < i.second+2 && y<n; y++) {
					for(int x = i.first-1<0 ? 0 : i.first-1; x<i.first+2 && x<n; x++) {
						if(board[n*y+x]==0) moves.insert(pair<int, int>(x, y));
					}
				}
				int tmp = minimax(true, depth-1, alpha, beta, moves);
				board[n*i.second+i.first]=0;
				if(tmp < beta) beta=tmp;
				if(alpha >= beta) return beta;
			}
		}
		return beta;
	}
}

pair<int, int> gomoku::bestMove() {
	int max=-1<<30;
	pair<int, int> out;

	for(auto i : successorMoves) {
		if(board[n*i.second+i.first]==0) {
			board[n*i.second+i.first]=2;
			set<pair<int, int>> moves(successorMoves);
			for(int y = i.second-1<0 ? 0 : i.second-1; y<i.second+2 && y<n; y++) {
				for(int x = i.first-1<0 ? 0 : i.first-1; x<i.first+2 && x<n; x++) {
					if(board[n*y+x]==0) moves.insert(pair<int, int>(x, y));
				}
			}
			int tmp = minimax(false, minimax_depth, -1<<30, 1<<30, moves);
			board[n*i.second+i.first]=0;
			if(max < tmp) {
				out = i;
				max = tmp;
			}
		}
	}
	if(max==-1<<30 && out.first==0 && out.second==0) out={n/2,n/2};
	return out;
}

char gomoku::checkGG() {
	return 0;
}