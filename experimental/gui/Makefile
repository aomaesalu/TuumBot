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
# Experimental GUI application makefile                                        #
# Contributors:                                                                #
#   Ants-Oskar Mäesalu                                                         #
################################################################################

# Compiler
CXX=g++

LIBFLAGS = `pkg-config gtkmm-3.0 --cflags --libs`

# Default compiler parameters
# -Wall 		Shows all warnings when compiling, always use this!
# -std=c++11 	Enables the C++11 standard mode
CXXFLAGS = -Wall -std=c++11 $(LIBFLAGS)

PROGRAM = main
SRCS =
OBJS = main.o
MAINSRC = main.cpp

all: build

build:
	$(CXX) $(MAINSRC) -o $(PROGRAM) $(LIBFLAGS)


clean:
	rm -f $(OBJS)
	rm -f $(PROGRAM)
