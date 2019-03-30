CPP=g++
DEBUG=-g
PROFILE=-pg
CPPFLAGS=-std=c++11 -Wall -Wextra -c
LOCALLIB=/usr/lib
LINKING=-o
INCLUDES=-I

LIBS=-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system
DEPS=
OBJECTS=tetromino.o board.o game.o main.o

all: fallblox

debug: CPPFLAGS+=$(DEBUG)
debug: all

fallblox: $(OBJECTS)
	$(CPP) $^ $(LINKING) $@ $(PRELIBS) $(LIBS)

%.o: %.cpp $(DEPS)
	$(CPP) $(CPPFLAGS) $< $(LINKING) $@ 

clean: 
	rm -rf *.o fallblox
