################################################################################
# Tuum                                                                         #
# Robotex 2015                                                                 #
# University of Tartu                                                          #
# Team:                                                                        #
#   Kristjan Kanarbik                                                          #
#   Meelik Kiik                                                                #
#   Ants-Oskar Mäesalu                                                         #
#   Mart Müllerbeck                                                            #
#   Kaur Viiklaid                                                              #
#   Indrek Virro                                                               #
#   Mihkel Väljaots                                                            #
################################################################################
# Experimental camera makefile                                                 #
# Contributors:                                                                #
#   Ants-Oskar Mäesalu                                                         #
################################################################################

# Compiler
CXX=g++

# Default compiler parameters
# -Wall 		Shows all warnings when compiling, always use this!
# -std=c++11 	Enables the C++11 standard mode
CXXFLAGS = -Wall -std=c++11

PROGRAM = main
SRCS = main.cpp camera.cpp
OBJS = main.o camera.o

main.o: main.cpp camera.h
camera.o: cameraConstants.h camera.h camera.cpp

all: build

build: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(PROGRAM)
