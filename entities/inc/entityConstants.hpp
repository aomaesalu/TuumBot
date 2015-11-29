/**
 *  @file constants.hpp
 *  Entities' constants header.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 29 November 2015
 */

#ifndef PHYSICAL_CONSTANTS_H
#define PHYSICAL_CONSTANTS_H

// Physical constants in millimeters
#define FIELD_WIDTH 3100                // Football field width
#define FIELD_LENGTH 4600               // Football field length
#define FIELD_CENTER_RADIUS 400         // Football field center circle radius          // Unused
#define DEFENSE_AREA_LARGER_WIDTH 1250  // Goal defense area larger width               // Unused
#define DEFENSE_AREA_SMALLER_WIDTH 350  // Goal defense area smaller width              // Unused
#define DEFENSE_AREA_RADIUS 500         // Goal defense area corner curve radius        // Unused
#define LINE_WIDTH 50                   // Field line width                             // Unused
#define GOAL_WIDTH 700                  // Goal width
#define GOAL_LENGTH 250                 // Goal length
#define GOAL_HEIGHT 200                 // Goal height
#define GOAL_BEACON_HEIGHT 400          // Goal beacon height                           // Unused
#define BLUE_GOAL_POSITION_X -1         // Blue goal's position x-coordinate            // Wrong
#define BLUE_GOAL_POSITION_Y -1         // Blue goal's position y-coordinate            // Wrong
#define YELLOW_GOAL_POSITION_X -1       // Yellow goal's position x-coordinate          // Wrong
#define YELLOW_GOAL_POSITION_Y -1       // Yellow goal's position y-coordinate          // Wrong
#define BALL_DIAMETER 43                // Ball diameter
#define ROBOT_MAXIMUM_DIAMETER 350      // Robots' maximum allowed diameter
#define ROBOT_MAXIMUM_HEIGHT 350        // Robots' maximum allowed height               // Unused
#define ROBOT_MARKER_WIDTH 20           // Single robot ally/enemy marker width
#define ROBOT_MARKER_MAX_HEIGHT 20      // Lower marker edge maximum height from ground // Unused

// Implementation-specific physical constants in millimeters
#define ROBOT_DIAMETER 350              // Our robot's diameter
#define ROBOT_INITIAL_POSITION_X -1     // Our robot's initial position x-coordinate
#define ROBOT_INITIAL_POSITION_Y -1     // Out robot's initial position y-coordinate

#endif // PHYSICAL_CONSTANTS_H
