CXX = g++
CXXFLAGS = -Wall -g

All: gobang

gobang: main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean: /.bin/rm -f *.o *.gch gobang