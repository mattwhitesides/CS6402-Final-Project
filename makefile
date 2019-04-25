# Matt Whitesides
# CS6402 - Adv Data Mining
# 3/19/2019

# NOTES: Need gcc version 8 or more.

IDIR = Include
CC = g++
CFLAGS = -Wall -I$(IDIR) -lstdc++fs -std=c++17

ODIR = Obj
ODIR_MULTI = Obj/Multi
ODIR_TEST = Obj/Test
SDIR = Src
LDIR = ../Lib

LIBS = -lm

_DEPS = preprocess.hpp process.hpp catch.hpp ngraph.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_DEPS_MULTI = preprocess.hpp multi_process.hpp catch.hpp ngraph.hpp
DEPS_MULTI = $(patsubst %,$(IDIR)/%,$(_DEPS_MULTI))

_OBJ = preprocess.o process.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ_MULTI = preprocess.o multi_process.o main.o
OBJ_MULTI = $(patsubst %,$(ODIR_MULTI)/%,$(_OBJ_MULTI))

_OBJ_TEST = preprocess.o process.o tests.o
OBJ_TEST = $(patsubst %,$(ODIR_TEST)/%,$(_OBJ_TEST))

# Default to build the multi threaded project exe.
multi: $(OBJ_MULTI)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)	

# Default to build the main project exe.
single: $(OBJ)	
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)	

# To build the test cases test.exe.
test: $(OBJ_TEST) 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(OBJ_MULTI): $(ODIR_MULTI)/%.o: $(SDIR)/%.cpp $(DEPS_MULTI) | $(ODIR_MULTI)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR_MULTI):
	mkdir -p $(ODIR_MULTI)

$(OBJ): $(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) | $(ODIR)	
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	mkdir -p $(ODIR)

$(OBJ_TEST): $(ODIR_TEST)/%.o: $(SDIR)/%.cpp $(DEPS) | $(ODIR_TEST)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR_TEST):
	mkdir -p $(ODIR_TEST)	

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f $(ODIR_MULTI)/*.o *~ core $(INCDIR)/*~ 
	rm -f $(ODIR_TEST)/*.o *~ core $(INCDIR)/*~ 
	rm -R $(ODIR)
	rm -f *.exe