#ifndef HEADER_H
#define HEADER_H

// base libraries 
#include <stdlib.h>
#include <ctype.h>

// math
#define _USE_MATH_DEFINES
#include <cmath>

// stupid stuff for stupid windows
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

// opengl, glu, and glut includes
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <glm/trigonometric.hpp>

#include <AntTweakBar.h>

#include "lsystem.hpp"
#include "turtle.hpp"
#include <cstdio>
#include <list>

//#define GLUT_DISABLE_ATEXIT_HACK
#define MS_PER_CYCLE 1000

// function prototypes:
void	Animate( );
void    NextIteration(void*);
void	Display( );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	Reset(void*);
void    Terminate(void*);
void	Resize( int, int );
void	Visibility( int );
void	Axes( float );
void	HsvRgb( float[3], float [3] );
void    InitGUI(void);
void    MouseMotion( int x, int y );
void    MouseButton( int button, int state, int x, int y );
void    Keyboard( unsigned char c, int x, int y );

void SetQuaternionFromAxisAngle(const float *axis, float angle, float *quat);
void ConvertQuaternionToMatrix(const float *quat, float *mat);
void MultiplyQuaternions(const float *q1, const float *q2, float *qout);
void QuatReset(float* q);

// welcome to extern hell

// title of these windows:
extern const char *WINDOWTITLE;
extern const char *GLUITITLE;

// what the glui package defines as true and false:
extern const int GLUITRUE;
extern const int GLUIFALSE;

// the escape key:
#define ESCAPE		0x1b

// initial window size:
extern const int INIT_WINDOW_HEIGHT;
extern const int INIT_WINDOW_WIDTH;

// minimum allowable scale factor:
extern const float MINSCALE;

// which projection:
enum Projections {
	ORTHO,
	PERSP
};

// active mouse buttons (or them together):
extern const int LEFT;
extern const int MIDDLE;
extern const int RIGHT;

// window background color (rgba):
extern const GLfloat BACKCOLOR[];

// line width for the axes:
extern const GLfloat AXES_WIDTH;
extern const GLfloat AXES_LENGTH;

// axis
extern const float x_axis[];
extern const float y_axis[];
extern const float z_axis[];

// non-constant global variables:
extern int		ActiveButton;			// current button that is down
extern bool		AxesOn;					// != 0 means to draw the axes
extern int		MainWindow;				// window id for main graphics window
extern float	Scale;					// scaling factor
extern bool		WhichProjection;		// ORTHO or PERSP
extern int		Xmouse, Ymouse;			// mouse values
extern bool     Frozen;
extern float    Time;
extern int      ms;

extern TwBar* bar;

//rotations
extern float g_Rotation[];
extern float g_Rotation_Scene[];
extern float g_Scene_Angle_x;
extern float g_Scene_Angle_y;
extern float color[];

extern bool loop;
extern Lsystem* Lsys;
extern std::list<std::string>   systems;
extern std::list<Turtle>        turt_sys;
extern std::vector<glm::vec3>   current_turtles;
extern std::vector<glm::vec3>   vertices;
extern std::vector<glm::vec3>   prev_vertices;

#endif
