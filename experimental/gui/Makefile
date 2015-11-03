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
CXX = g++

# Default compiler parameters
# -Wall 		Shows all warnings when compiling, always use this!
# -std=c++11 	Enables the C++11 standard mode
CXXFLAGS = -Wall -g -std=c++11 `pkg-config gtkmm-3.0 --cflags`

# gtkmm library flags
LDLIBS = `pkg-config gtkmm-3.0 --libs`

PROGRAM = main
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPEND = .depend

.PHONY: clean

$(PROGRAM): $(OBJS)
	$(CXX) $^ -g -o $@ $(LDLIBS)

# Object file rules:
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

# Dependencies
.depend:
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $(SRCS) > $(DEPEND)

all: .depend $(PROGRAM)

clean:
	rm -f $(OBJS)
	rm -f $(PROGRAM)
	rm -f $(DEPEND)

-include $(DEPEND)
