#ifndef RTX_CONSTANTS_H
#define RTX_CONSTANTS_H

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
#define GOAL_HEIGHT 200                 // Goal height                                  // Unused
#define GOAL_BEACON_HEIGHT 400          // Goal beacon height                           // Unused
#define BLUE_GOAL_POSITION_X -1         // Blue goal's position x-coordinate            // Wrong
#define BLUE_GOAL_POSITION_Y -1         // Blue goal's position y-coordinate            // Wrong
#define YELLOW_GOAL_POSITION_X -1       // Yellow goal's position x-coordinate          // Wrong
#define YELLOW_GOAL_POSITION_Y -1       // Yellow goal's position y-coordinate          // Wrong
#define BALL_DIAMETER 43                // Ball radius
#define ROBOT_MARKER_WIDTH 20           // Single robot ally/enemy marker width         // Unused
#define ROBOT_MARKER_MAX_HEIGHT 20      // Lower marker edge maximum height from ground // Unused


// Implementation-specific physical constants in millimeters
#define ROBOT_RADIUS 350                // Our robot's radius
#define ROBOT_INITIAL_POSITION_X -1     // Our robot's initial position x-coordinate
#define ROBOT_INITIAL_POSITION_Y -1     // Out robot's initial position y-coordinate


#define PI 3.1415926536

#endif // RTX_CONSTANTS_H
