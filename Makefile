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
# Makefile                                                                     #
# Contributors:                                                                #
#   Ants-Oskar Mäesalu                                                         #
# Usage instructions:                                                          #
#   TODO                                                                       #
################################################################################

# Compiler
CXX=g++

# Default compiler parameters
# -Wall 		Shows all warnings when compiling, always use this!
# -std=c++11 	Enables the C++11 standard mode
CXXFLAGS = -Wall -std=c++11

################################################################################
# Settings and compilation rules for the modules.                              #
################################################################################

# Compiler flag -Idir specifies, that there are includes in the 'dir' directory
LIB_CXXFLAGS = $(CXXFLAGS) -Iinclude

LIB_OBJS = $(LIB_COILGUN_OBJS) $(LIB_COMMUNICATION_OBJS) $(LIB_COMPUTER_VISION_OBJS) $(LIB_MATHEMATICS_OBJS) $(LIB_MOVEMENT_OBJS) $(LIB_OBJECTS_OBJS) $(LIB_TRIBBLER_OBJS)

LIBS = $(LIB_COILGUN) $(LIB_COMMUNICATION) $(LIB_COMPUTER_VISION) $(LIB_MATHEMATICS) $(LIB_MOVEMENT) $(LIB_OBJECTS) $(LIB_TRIBBLER)

# Coilgun ######################################################################

LIB_COILGUN_OBJS =
LIB_COILGUN = lib/coilgun.a

# TODO: Coilgun module object compilation rules

$(LIB_COILGUN): $(LIB_COILGUN_OBJS)
	ar cr $(LIB_COILGUN) $(LIB_COILGUN_OBJS)

# Communication ################################################################

LIB_COMMUNICATION_OBJS =
LIB_COMMUNICATION = lib/communication.a

# TODO: Communication module object compilation rules

$(LIB_COMMUNICATION): $(LIB_COMMUNICATION_OBJS)
	ar cr $(LIB_COMMUNICATION) $(LIB_COMMUNICATION_OBJS)

# Computer vision ##############################################################

LIB_COMPUTER_VISION_OBJS =
LIB_COMPUTER_VISION = lib/computer-vision.a

# TODO: Computer vision module object compilation rules

$(LIB_COMPUTER_VISION): $(LIB_COMPUTER_VISION_OBJS)
	ar cr $(LIB_COMPUTER_VISION) $(LIB_COMPUTER_VISION_OBJS)

# Mathematics ##################################################################

LIB_MATHEMATICS_OBJS = obj/modules/mathematics/point2d.o obj/modules/mathematics/line.o obj/modules/mathematics/circle.o obj/modules/mathematics/rectangle.o
LIB_MATHEMATICS = lib/mathematics.a

obj/modules/mathematics/point2d.o: src/modules/mathematics/point2d.cpp
	mkdir -p obj/modules/mathematics
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/mathematics/point2d.cpp -o obj/modules/mathematics/point2d.o
obj/modules/mathematics/line.o: src/modules/mathematics/line.cpp
	mkdir -p obj/modules/mathematics
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/mathematics/line.cpp -o obj/modules/mathematics/line.o
obj/modules/mathematics/circle.o: src/modules/mathematics/circle.cpp
	mkdir -p obj/modules/mathematics
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/mathematics/circle.cpp -o obj/modules/mathematics/circle.o
obj/modules/mathematics/rectangle.o: src/modules/mathematics/rectangle.cpp
	mkdir -p obj/modules/mathematics
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/mathematics/rectangle.cpp -o obj/modules/mathematics/rectangle.o

$(LIB_MATHEMATICS): $(LIB_MATHEMATICS_OBJS)
	mkdir -p lib
	ar cr $(LIB_MATHEMATICS) $(LIB_MATHEMATICS_OBJS)

# Movement #####################################################################

LIB_MOVEMENT_OBJS =
LIB_MOVEMENT = lib/movement.a

# TODO: Movement module object compilation rules

$(LIB_MOVEMENT): $(LIB_MOVEMENT_OBJS)
	ar cr $(LIB_MOVEMENT) $(LIB_MOVEMENT_OBJS)

# Objects ######################################################################

LIB_OBJECTS_OBJS = obj/modules/objects/object.o obj/modules/objects/ball.o obj/modules/objects/field.o obj/modules/objects/field1vs1.o obj/modules/objects/field2vs2.o obj/modules/objects/goal.o obj/modules/objects/robot.o obj/modules/objects/robotSelf.o obj/modules/objects/robotAlly.o obj/modules/objects/robotEnemy.o
LIB_OBJECTS = lib/objects.a

obj/modules/objects/object.o: src/modules/objects/object.cpp
	mkdir -p obj/modules/objects
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/objects/object.cpp -o obj/modules/objects/object.o
obj/modules/objects/ball.o: src/modules/objects/ball.cpp
	mkdir -p obj/modules/objects
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/objects/ball.cpp -o obj/modules/objects/ball.o
obj/modules/objects/field.o: src/modules/objects/field.cpp
	mkdir -p obj/modules/objects
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/objects/field.cpp -o obj/modules/objects/field.o
obj/modules/objects/field1vs1.o: src/modules/objects/field1vs1.cpp
	mkdir -p obj/modules/objects
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/objects/field1vs1.cpp -o obj/modules/objects/field1vs1.o
obj/modules/objects/field2vs2.o: src/modules/objects/field2vs2.cpp
	mkdir -p obj/modules/objects
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/objects/field2vs2.cpp -o obj/modules/objects/field2vs2.o
obj/modules/objects/goal.o: src/modules/objects/goal.cpp
	mkdir -p obj/modules/objects
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/objects/goal.cpp -o obj/modules/objects/goal.o
obj/modules/objects/robot.o: src/modules/objects/robot.cpp
	mkdir -p obj/modules/objects
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/objects/robot.cpp -o obj/modules/objects/robot.o
obj/modules/objects/robotSelf.o: src/modules/objects/robotSelf.cpp
	mkdir -p obj/modules/objects
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/objects/robotSelf.cpp -o obj/modules/objects/robotSelf.o
obj/modules/objects/robotAlly.o: src/modules/objects/robotAlly.cpp
	mkdir -p obj/modules/objects
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/objects/robotAlly.cpp -o obj/modules/objects/robotAlly.o
obj/modules/objects/robotEnemy.o: src/modules/objects/robotEnemy.cpp
	mkdir -p obj/modules/objects
	$(CXX) $(LIB_CXXFLAGS) -c src/modules/objects/robotEnemy.cpp -o obj/modules/objects/robotEnemy.o

$(LIB_OBJECTS): $(LIB_OBJECTS_OBJS)
	ar cr $(LIB_OBJECTS) $(LIB_OBJECTS_OBJS)

# Tribbler #####################################################################

LIB_TRIBBLER_OBJS =
LIB_TRIBBLER = lib/tribbler.a

# TODO: Tribbler module object compilation rules

$(LIB_TRIBBLER): $(LIB_TRIBBLER_OBJS)
	ar cr $(LIB_TRIBBLER) $(LIB_TRIBBLER_OBJS)

################################################################################
# Settings and compilation rules for the programs.                             #
################################################################################

PROGRAMS = $(PROGRAMS_CALIBRATION) $(PROGRAMS_COMPETITION) $(PROGRAMS_SIMULATION)

################################################################################
# Settings and compilation rules for the calibration programs.                 #
################################################################################

PROGRAMS_CALIBRATION = $(PROGRAM_CALIBRATION_COLOR) $(PROGRAM_CALIBRATION_LENS) $(PROGRAM_CALIBRATION_PERSPECTIVE) $(PROGRAMS_CALIBRATION_POSITION) $(PROGRAMS_CALIBRATION_SPEED)

# Color ########################################################################

PROGRAM_CALIBRATION_COLOR_OBJS =
PROGRAM_CALIBRATION_COLOR = bin/calibration-color

# Lens #########################################################################

PROGRAM_CALIBRATION_LENS_OBJS =
PROGRAM_CALIBRATION_LENS = bin/calibration-lens

# Perspective ##################################################################

PROGRAM_CALIBRATION_PERSPECTIVE_OBJS =
PROGRAM_CALIBRATION_PERSPECTIVE = bin/calibration-perspective

# Position #####################################################################

PROGRAM_CALIBRATION_POSITION_OBJS =
PROGRAM_CALIBRATION_POSITION = bin/calibration-position

# Speed ########################################################################

PROGRAM_CALIBRATION_SPEED_OBJS =
PROGRAM_CALIBRATION_SPEED = bin/calibration-speed

################################################################################
# Settings and compilation rules for the competition programs.                 #
################################################################################

PROGRAMS_COMPETITION = $(PROGRAM_COMPETITION_1VS1) $(PROGRAM_COMPETITION_2VS2)

# 1vs1 #########################################################################

PROGRAM_COMPETITION_1VS1_OBJS =
PROGRAM_COMPETITION_1VS1 = bin/competition-1vs1

# 2vs2 #########################################################################

PROGRAM_COMPETITION_2VS2_OBJS =
PROGRAM_COMPETITION_2VS2 = bin/competition-2vs2

################################################################################
# Settings and compilation rules for the simulation programs.                  #
################################################################################

PROGRAMS_SIMULATION = $(PROGRAM_SIMULATION_1VS1) $(PROGRAM_SIMULATION_2VS2)

# 1vs1 #########################################################################

PROGRAM_SIMULATION_1VS1_OBJS =
PROGRAM_SIMULATION_1VS1 = bin/simulation-1vs1

# 2vs2 #########################################################################

PROGRAM_SIMULATION_2VS2_OBJS =
PROGRAM_SIMULATION_2VS2 = bin/simulation-2vs2

################################################################################
# Make rules.                                                                  #
################################################################################

# build - Target that builds all of the necessary libraries ####################

build: $(LIBS)

coilgun: $(LIB_COILGUN)
communication: $(LIB_COMMUNICATION)
computer-vision: $(LIB_COMPUTER_VISION)
mathematics: $(LIB_MATHEMATICS)
movement: $(LIB_MOVEMENT)
objects: $(LIB_OBJECTS)
tribbler: $(LIB_TRIBBLER)

# calibration - Target that builds only the calibration programs ###############

calibration: $(PROGRAMS_CALIBRATION)

calibration-color: $(PROGRAM_CALIBRATION_COLOR)
calibration-lens: $(PROGRAM_CALIBRATION_LENS)
calibration-perspective: $(PROGRAM_CALIBRATION_PERSPECTIVE)
calibration-position: $(PROGRAM_CALIBRATION_POSITION)
calibration-speed: $(PROGRAM_CALIBRATION_SPEED)

# competition - Target that builds only the competition programs ###############

competition: $(PROGRAMS_COMPETITION)

competition-1vs1: $(PROGRAM_COMPETITION_1VS1)
competition-2vs2: $(PROGRAM_COMPETITION_2VS2)

# simulation - Target that builds only the simulation programs #################

simulation: $(PROGRAMS_SIMULATION)

simulation-1vs1: $(PROGRAM_SIMULATION_1VS1)
simulation-2vs2: $(PROGRAM_SIMULATION_2VS2)

# all - Target that builds all of the executables ##############################

all: $(PROGRAMS)

# clean - Target that cleans all of the compiled files #########################

clean:
	rm -f $(LIB_OBJS)
	rm -f $(LIBS)
	rm -f $(PROGRAMS)
# TODO

# test - Target that builds the test application ###############################

test: ;
# TODO
