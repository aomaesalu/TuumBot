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
# TODO: Add optimisation
CXXFLAGS = -Wall -std=c++11

DIR_SRC = src
DIR_OBJ = obj
DIR_LIB = lib
DIR_BIN = bin
DIR_TEST = test

################################################################################
# Settings and compilation rules for the modules                               #
################################################################################

# Compiler flag -Idir specifies, that there are includes in the 'dir' directory
LIB_CXXFLAGS = $(CXXFLAGS) -Iinclude

LIB_OBJS = $(LIB_COILGUN_OBJS) $(LIB_COMMUNICATION_OBJS) $(LIB_COMPUTER_VISION_OBJS) $(LIB_MATHEMATICS_OBJS) $(LIB_MOVEMENT_OBJS) $(LIB_OBJECTS_OBJS) $(LIB_TRIBBLER_OBJS)

LIBS = $(LIB_COILGUN) $(LIB_COMMUNICATION) $(LIB_COMPUTER_VISION) $(LIB_MATHEMATICS) $(LIB_MOVEMENT) $(LIB_OBJECTS) $(LIB_TRIBBLER)

# Coilgun ######################################################################

LIB_COILGUN_OBJS =
LIB_COILGUN = $(DIR_LIB)/coilgun.a

$(LIB_COILGUN): $(LIB_COILGUN_OBJS)
	mkdir -p $(DIR_LIB)
	ar cr $(LIB_COILGUN) $(LIB_COILGUN_OBJS)

# Communication ################################################################

LIB_COMMUNICATION_OBJS =
LIB_COMMUNICATION = $(DIR_LIB)/communication.a

$(LIB_COMMUNICATION): $(LIB_COMMUNICATION_OBJS)
	mkdir -p $(DIR_LIB)
	ar cr $(LIB_COMMUNICATION) $(LIB_COMMUNICATION_OBJS)

# Computer vision ##############################################################

LIB_COMPUTER_VISION_OBJS =
LIB_COMPUTER_VISION = $(DIR_LIB)/computer-vision.a

$(LIB_COMPUTER_VISION): $(LIB_COMPUTER_VISION_OBJS)
	mkdir -p $(DIR_LIB)
	ar cr $(LIB_COMPUTER_VISION) $(LIB_COMPUTER_VISION_OBJS)

# Mathematics ##################################################################

LIB_MATHEMATICS_OBJS = obj/modules/mathematics/point2d.o obj/modules/mathematics/line.o obj/modules/mathematics/circle.o obj/modules/mathematics/rectangle.o
LIB_MATHEMATICS = $(DIR_LIB)/mathematics.a

$(LIB_MATHEMATICS): $(LIB_MATHEMATICS_OBJS)
	mkdir -p $(DIR_LIB)
	ar cr $(LIB_MATHEMATICS) $(LIB_MATHEMATICS_OBJS)

# Movement #####################################################################

LIB_MOVEMENT_OBJS =
LIB_MOVEMENT = $(DIR_LIB)/movement.a

$(LIB_MOVEMENT): $(LIB_MOVEMENT_OBJS)
	mkdir -p $(DIR_LIB)
	ar cr $(LIB_MOVEMENT) $(LIB_MOVEMENT_OBJS)

# Objects ######################################################################

LIB_OBJECTS_OBJS = $(DIR_OBJ)/modules/objects/object.o obj/modules/objects/ball.o $(DIR_OBJ)/modules/objects/field.o $(DIR_OBJ)/modules/objects/field1vs1.o $(DIR_OBJ)/modules/objects/field2vs2.o $(DIR_OBJ)/modules/objects/goal.o $(DIR_OBJ)/modules/objects/robot.o $(DIR_OBJ)/modules/objects/robotSelf.o $(DIR_OBJ)/modules/objects/robotAlly.o $(DIR_OBJ)/modules/objects/robotEnemy.o
LIB_OBJECTS = $(DIR_LIB)/objects.a

$(LIB_OBJECTS): $(LIB_OBJECTS_OBJS)
	mkdir -p $(DIR_LIB)
	ar cr $(LIB_OBJECTS) $(LIB_OBJECTS_OBJS)

# Tribbler #####################################################################

LIB_TRIBBLER_OBJS =
LIB_TRIBBLER = $(DIR_LIB)/tribbler.a

$(LIB_TRIBBLER): $(LIB_TRIBBLER_OBJS)
	mkdir -p $(DIR_LIB)
	ar cr $(LIB_TRIBBLER) $(LIB_TRIBBLER_OBJS)

################################################################################
# Settings and compilation rules for the programs                              #
################################################################################

PROGRAMS = $(PROGRAMS_CALIBRATION) $(PROGRAMS_COMPETITION) $(PROGRAMS_SIMULATION)

################################################################################
# Settings and compilation rules for the calibration programs                  #
################################################################################

PROGRAMS_CALIBRATION = $(PROGRAM_CALIBRATION_COLOR) $(PROGRAM_CALIBRATION_LENS) $(PROGRAM_CALIBRATION_PERSPECTIVE) $(PROGRAMS_CALIBRATION_POSITION) $(PROGRAMS_CALIBRATION_SPEED)

# Color ########################################################################

PROGRAM_CALIBRATION_COLOR_OBJS =
PROGRAM_CALIBRATION_COLOR = $(DIR_BIN)/calibration-color

$(PROGRAM_CALIBRATION_COLOR): $(PROGRAM_CALIBRATION_COLOR_OBJS)
	mkdir -p $(DIR_BIN)
	ar cr $(PROGRAM_CALIBRATION_COLOR) $(PROGRAM_CALIBRATION_COLOR_OBJS)

# Lens #########################################################################

PROGRAM_CALIBRATION_LENS_OBJS =
PROGRAM_CALIBRATION_LENS = $(DIR_BIN)/calibration-lens

$(PROGRAM_CALIBRATION_LENS): $(PROGRAM_CALIBRATION_LENS_OBJS)
	mkdir -p $(DIR_BIN)
	ar cr $(PROGRAM_CALIBRATION_LENS) $(PROGRAM_CALIBRATION_LENS_OBJS)

# Perspective ##################################################################

PROGRAM_CALIBRATION_PERSPECTIVE_OBJS =
PROGRAM_CALIBRATION_PERSPECTIVE = $(DIR_BIN)/calibration-perspective

$(PROGRAM_CALIBRATION_PERSPECTIVE): $(PROGRAM_CALIBRATION_PERSPECTIVE_OBJS)
	mkdir -p $(DIR_BIN)
	ar cr $(PROGRAM_CALIBRATION_PERSPECTIVE) $(PROGRAM_CALIBRATION_PERSPECTIVE_OBJS)

# Position #####################################################################

PROGRAM_CALIBRATION_POSITION_OBJS =
PROGRAM_CALIBRATION_POSITION = $(DIR_BIN)/calibration-position

$(PROGRAM_CALIBRATION_POSITION): $(PROGRAM_CALIBRATION_POSITION_OBJS)
	mkdir -p $(DIR_BIN)
	ar cr $(PROGRAM_CALIBRATION_POSITION) $(PROGRAM_CALIBRATION_POSITION_OBJS)

# Speed ########################################################################

PROGRAM_CALIBRATION_SPEED_OBJS =
PROGRAM_CALIBRATION_SPEED = $(DIR_BIN)/calibration-speed

$(PROGRAM_CALIBRATION_SPEED): $(PROGRAM_CALIBRATION_SPEED_OBJS)
	mkdir -p $(DIR_BIN)
	ar cr $(PROGRAM_CALIBRATION_SPEED) $(PROGRAM_CALIBRATION_SPEED_OBJS)

################################################################################
# Settings and compilation rules for the competition programs                  #
################################################################################

PROGRAMS_COMPETITION = $(PROGRAM_COMPETITION_1VS1) $(PROGRAM_COMPETITION_2VS2)

# 1vs1 #########################################################################

PROGRAM_COMPETITION_1VS1_OBJS =
PROGRAM_COMPETITION_1VS1 = $(DIR_BIN)/competition-1vs1

$(PROGRAM_COMPETITION_1VS1): $(PROGRAM_COMPETITION_1VS1_OBJS)
	mkdir -p $(DIR_BIN)
	ar cr $(PROGRAM_COMPETITION_1VS1) $(PROGRAM_COMPETITION_1VS1_OBJS)

# 2vs2 #########################################################################

PROGRAM_COMPETITION_2VS2_OBJS =
PROGRAM_COMPETITION_2VS2 = $(DIR_BIN)/competition-2vs2

$(PROGRAM_COMPETITION_2VS2): $(PROGRAM_COMPETITION_2VS2_OBJS)
	mkdir -p $(DIR_BIN)
	ar cr $(PROGRAM_COMPETITION_2VS2) $(PROGRAM_COMPETITION_2VS2_OBJS)

################################################################################
# Settings and compilation rules for the simulation programs                   #
################################################################################

PROGRAMS_SIMULATION = $(PROGRAM_SIMULATION_1VS1) $(PROGRAM_SIMULATION_2VS2)

# 1vs1 #########################################################################

PROGRAM_SIMULATION_1VS1_OBJS =
PROGRAM_SIMULATION_1VS1 = $(DIR_BIN)/simulation-1vs1

$(PROGRAM_SIMULATION_1VS1): $(PROGRAM_SIMULATION_1VS1_OBJS)
	mkdir -p $(DIR_BIN)
	ar cr $(PROGRAM_SIMULATION_1VS1) $(PROGRAM_SIMULATION_1VS1_OBJS)

# 2vs2 #########################################################################

PROGRAM_SIMULATION_2VS2_OBJS =
PROGRAM_SIMULATION_2VS2 = $(DIR_BIN)/simulation-2vs2

$(PROGRAM_SIMULATION_2VS2): $(PROGRAM_SIMULATION_2VS2_OBJS)
	mkdir -p $(DIR_BIN)
	ar cr $(PROGRAM_SIMULATION_2VS2) $(PROGRAM_SIMULATION_2VS2_OBJS)

################################################################################
# Make rules                                                                   #
################################################################################

# Compile all of the source files into corresponding object files

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(LIB_CXXFLAGS) -c -o $@ $<

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

all: $(LIBS) $(PROGRAMS)

# clean - Target that cleans all of the compiled files #########################

clean-obj:
	rm -rf $(DIR_OBJ)
clean-lib:
	rm -rf $(DIR_LIB)
clean-bin:
	rm -rf $(DIR_BIN)
clean-test:
	rm -rf $(DIR_TEST)

clean:
	make clean-obj clean-lib clean-bin clean-test

# test - Target that builds the test application ###############################

test: ;
# TODO
