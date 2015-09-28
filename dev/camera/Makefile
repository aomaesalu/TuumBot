# Compiler
CXX=g++

# Default compiler parameters
# -Wall 		Shows all warnings when compiling, always use this!
# -std=c++11 	Enables the C++11 standard mode
CXXFLAGS = -Wall -std=c++11

PROGRAM = camera_test
SRCS = main.cpp webcam.cpp
OBJS = main.o webcam.o

main.o: main.cpp webcam.h
webcam.o: webcam.h webcam.cpp

all: build

build: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(PROGRAM)
