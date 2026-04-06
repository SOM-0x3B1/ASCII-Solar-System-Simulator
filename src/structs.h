#ifndef ASCII_SSS_STRUCTS_H
#define ASCII_SSS_STRUCTS_H

#include <stdbool.h>
#include <time.h>


//============= Vector geometry =================

typedef struct Vector {
    double x;
    double y;
} Vector;

typedef struct Point {
    long long int x;
    long long int y;
} Point;



//============= Simulation =================

/** A node of a trail; a queue element. */
typedef struct Trail {
    Point position;
    struct Trail *next;
} Trail;

/** A queue of trail points. */
typedef struct TrailQueue {
    Trail *top;
    int length;
    int capacity; // Max capacity of the queue, after it automatically dequeues the last element
} TrailQueue;


typedef enum BodyEditableProperty {
    BODY_PROPERTY_NAME,
    BODY_PROPERTY_MASS,
    BODY_PROPERTY_R,
    BODY_PROPERTY_POS,
    BODY_PROPERTY_VEL,
    bodyEditableProperty_MAX
} BodyEditableProperty;


/** A celestial body. */
typedef struct Body {
    char name[13];
    Vector position;
    Vector velocity;
    double r; // Radius
    double mass;
    char color;
    TrailQueue trail;
} Body;

typedef struct BodyArray {
    Body *data;
    int length;
    int capacity; // Currently allocated size (will be doubled when exceeded)
} BodyArray;

typedef struct Simulation {
    double solarMass;                   // The mass of the sun; every gravitational effect is relative to this!
    double detectCollisionPercentage;   // The percentage of the minimum radius overlap that triggers a collision event.
    bool fullSpeed;                     // Overrides the FPS limit -> the simulation will run at maximum speed
    bool pausedByUser;                  // Is the simulation pasued by the user
    int trailSpacingCounter;            // Measures the time passed since the last trail point.
    BodyArray bodyArray;

    Body *sun;
    Body *following;  // The body that the camera follows (NULL, if the camera is free).
    Body *editedBody; // The body that is currently being edited by the body editor.
} Simulation;



//============= Menu & GUI =================

typedef enum EditMenuMainOption {
    OPTION_ADD_BODY,
    OPTION_EDIT_BODY,
    OPTION_DELETE_BODY,
    OPTION_FOLLOW_BODY,
    OPTION_TOGGLE_DETAILS,
    OPTION_TOGGLE_G_RANGE,
    OPTION_TOGGLE_TRAILS,
    OPTION_IMPORT_SYSTEM,
    OPTION_EXPORT_SYSTEM,
    OPTION_EXIT,
    EditMenuMainOption_MAX
} EditMenuMainOption;


typedef enum EditMenuSTATE {
    EDIT_MENU_STATE_MAIN,
    EDIT_MENU_STATE_ADD_BODY,
    EDIT_MENU_STATE_EDIT_BODY,
    EDIT_MENU_STATE_EDIT_BODY_SET,
    EDIT_MENU_STATE_DELETE_BODY,
    EDIT_MENU_STATE_FOLLOW_BODY,
} EditMenuSTATE;

typedef enum BodyEditorState {
    BODY_SET_NAME,
    BODY_SET_MASS,
    BODY_SET_R,
    BODY_SET_POS,
    BODY_SET_V,
} BodyEditorState;


#define EARTH_ANIMATION_FRAMES 240
#define EARTH_ANIMATION_HEIGHT 30
#define EARTH_ANIMATION_WIDTH 107

typedef struct Gui {
    char mainMenu_animation[EARTH_ANIMATION_FRAMES][EARTH_ANIMATION_HEIGHT][EARTH_ANIMATION_WIDTH + 1]; // [frame][y][x]
    char mainMenu_title[13][53];       // Title art
    int mainMenu_animation_frameIndex; // Current frame
    EditMenuSTATE editMenu_state;
    BodyEditorState bodyEditor_state;
    int cursorPos; // The current position of the selection cursor
    Point textPos; // The position where the cursor should start after entering text input mode
} Gui;



//============= Render =================

#define LAYER_COUNT 6

/** A 2D matrix that can be written onto the console. */
typedef struct Layer {
    char **text; // 2D array
    bool enabled;
} Layer;

// All layers are stored here
typedef struct LayerInstances {
    Layer overlayLayer;
    Layer menuLayer;
    Layer infoLayer;
    Layer bodyLayer;
    Layer rangeLayer;
    Layer trailLayer;
} LayerInstances;

typedef struct LayerStatic {
    LayerInstances layerInstances;  // All layers are declared here --> can be directly accessed here
    Layer *layers[LAYER_COUNT];     // The array of layers in order of priority (0. > 1. > ...)
} LayerStatic;

typedef struct Screen {
    int width;
    int height;
    char *buffer;       // Used as the buffer of the console
    size_t bufferSize;  // height * width * sizeof(char)
    Point offset;       // The top-left position of the camera
    int fps;
    int targetFPS;
    time_t frameCountResetedTime; // The last time FPS was evaluated
    int frameCount;     // Frames since last reset (~1s)
} Screen;



//============= Error =================

typedef enum Error {
    SUCCESS,
    ERR_SETTINGS_OPEN_FILE,
    ERR_SETTINGS_PARAMETER,
    ERR_SETTIGNS_VALUE,
    ERR_MAIN_MENU_ART_MISSING,
    ERR_MEMORY,
    ERR_ADDBODY_VALUE,
    ERR_FS_FILENAME,
    ERR_EXPORT_WRITE,
    ERR_IMPORT_OPEN_FILE,
    ERR_IMPORT_VALUE
} Error;



//============= Program =================

typedef enum ProgramState {
    PROGRAM_STATE_MAIN_MENU,
    PROGRAM_STATE_EDIT_MENU,
    PROGRAM_STATE_SIMULATION,
    PROGRAM_STATE_TEXT_INPUT,
    PROGRAM_STATE_PLACING_BODY,
} ProgramState;

typedef enum TextInputDest {
    TEXT_INPUT_BODY_EDITOR,
    TEXT_INPUT_EXPORT,
    TEXT_INPUT_IMPORT,
} TextInputDest;

typedef struct Program {
    ProgramState state; // The current state of the program.
    TextInputDest textInputDest; // Which part of the program should process the text input.
    double sleepTime;   // Regulates simulation speed (and FPS)
    Error error;        // Is not SUCCESS if the program has encountered an unadressed error.
    bool exiting;       // Exits the main program loop.
} Program;


#endif //ASCII_SSS_STRUCTS_H
