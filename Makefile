# Simple Makefile for multiple executeables + X other files/classes
# Main files .cxx, Classes in .cpp and .h
# Define INCLUDE, LIBS and EXE accordingly
INCLUDE = -I$(BOOSTROOT) -I./.
LIBS = -L$(BOOSTROOT)/stage/lib -lboost_thread -lboost_system

# Define compiler and flags
CXX = g++
CFLAGS = -g -O0 -Wall $(INCLUDE)
LDFLAGS = $(LIBS)

SRC = $(wildcard *.cpp) 
TARGET = $(wildcard *.cxx)
OBJ = $(SRC:.cpp=.o)
OOBJ = $(TARGET:.cxx=.o)
EXE = $(TARGET:.cxx=)

all: $(EXE)

%.o: %.cxx $(OBJ)
	$(CXX) -c $(CFLAGS) $< -o $@

%.o: %.cpp 
	$(CXX) -c $(CFLAGS) $< -o $@

%: %.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $< $(OBJ) -o $@

.PHONY : clean
clean:
	rm -f $(OBJ) $(OOBJ)
	rm -f $(EXE)
