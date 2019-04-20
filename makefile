# Matt Whitesides
# CS6402 - Adv Data Mining
# 3/19/2019

# NOTES: Need gcc version 8 or more.

IDIR = Include
CC = g++
CFLAGS = -Wall -I$(IDIR) -lstdc++fs -std=c++17

ODIR = Obj
ODIR_TEST = Obj/Test
SDIR = Src
LDIR = ../Lib

LIBS = -lm

_DEPS = preprocess.hpp process.hpp catch.hpp ngraph.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = preprocess.o process.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ_TEST = preprocess.o process.o tests.o
OBJ_TEST = $(patsubst %,$(ODIR_TEST)/%,$(_OBJ_TEST))

# Default to build the main project exe.
main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# To build the test cases test.exe.
test: $(OBJ_TEST) 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(OBJ): $(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_TEST): $(ODIR_TEST)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 