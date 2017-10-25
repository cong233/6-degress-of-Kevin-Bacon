# A simple makefile for CSE 100 P4

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
  CPPFLAGS += -O3
  LDFLAGS += -O3
else
  CPPFLAGS += -g
  LDFLAGS += -g
endif

all: pathfinder actorconnections extension



# include what ever source code *.hpp files pathfinder relies on (these are merely the ones thst were used in the solution)

pathfinder: ActorGraph.o UnionFind.hpp UnionFind.cpp

actorconnections: ActorGraph.o actorconnections.cpp UnionFind.hpp UnionFind.cpp

extension: ActorGraph.o extension.cpp UnionFind.hpp UnionFind.cpp

# include what ever source code *.hpp files ActorGraph relies on (these are merely the ones thst were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.hpp file that is already included with class/method headers

ActorGraph.o: Movie.hpp ActorNode.hpp ActorEdge.hpp ActorGraph.hpp


clean:
	rm -f pathfinder actorconnections extension *.o core*

