CXX = g++
CXXFLAGS = -Wall -O3

All: gobang

gobang: main.o gomoku.o gomoku.hpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	/bin/rm -f *.o *.gch *.out gobang
