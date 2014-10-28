# Simple Makefile for multiple executeables + X other files/classes
# Main files .cxx, Classes in .cpp and .h
# Define INCLUDE, LIBS and EXE accordingly

MAKEFLAGS=--warn-undefined-variables

INCLUDE = -I$(BOOST_INC) -I./.
LIBS = -L$(BOOST_LIB) -lboost_thread -lboost_system

# Define compiler and flags
CXX = g++
CFLAGS = -g -O0 -Qunused-arguments -Wall -Wno-unused-variable $(INCLUDE)
LDFLAGS = $(LIBS)

SRC = $(wildcard *.cpp) 
TARGET = $(wildcard *.cxx)
OBJ = $(SRC:.cpp=.o)
OOBJ = $(TARGET:.cxx=.o)
EXE = $(TARGET:.cxx=)

all: $(EXE) $(OBJ) $(OOBJ)

%.o: %.cxx $(OBJ)
	@$(CXX) -c $(CFLAGS) $< -o $@
	@echo "[Compiling] $@"

%.o: %.cpp 
	@$(CXX) -c $(CFLAGS) $< -o $@
	@echo "[Compiling] $@"

%: %.o
	@$(CXX) $(CFLAGS) $(LDFLAGS) $< $(OBJ) -o $@
	@echo "[Linking] $@"

.PHONY: clean
clean:
	rm -f $(OBJ) $(OOBJ)
	rm -f $(EXE)
