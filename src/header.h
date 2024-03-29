#ifndef HEADER_H
#define HEADER_H

// base libraries 
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <algorithm>

// math
#define _USE_MATH_DEFINES
#include <cmath>

// stupid stuff for stupid windows
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include <GL/glew.h>
#endif
// opengl, glu, and glut includes
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include <glm/trigonometric.hpp>

#include <AntTweakBar.h>

#include "lsystem.hpp"
#include "turtle.hpp"
#include <list>
#include <fstream>


//#define GLUT_DISABLE_ATEXIT_HACK

// function prototypes:
void	Animate( );
void    NextIteration();
void    NextIterHandle(void*);
void    CompleteDrawHandle(void*);
void    AnimateDraw(void*);
void	Display( );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	Reset(void*);
void	Resize( int, int );
void	Visibility( int );
void	Axes( float );
void	HsvRgb( float[3], float [3] );
void    InitGUI(void);
void    MouseMotion( int x, int y );
void    MouseButton( int button, int state, int x, int y );
void    ResetDrawing(void*);

void SetQuaternionFromAxisAngle(const float *axis, float angle, float *quat);
void ConvertQuaternionToMatrix(const float *quat, float *mat);
void MultiplyQuaternions(const float *q1, const float *q2, float *qout);
void QuatReset(float* q);

// welcome to extern hell

// title of these windows:
extern const char *WINDOWTITLE;

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
extern float    Time;
extern float	Scale;					// scaling factor
extern float    fPointScale;
extern int      iAniScale;
extern int      iCurrentGen;
extern int		MainWindow;				// window id for main graphics window
extern int      iFramePerCycle;
extern int		ActiveButton;			// current button that is down
extern int      iCounter;
extern int		Xmouse, Ymouse;			// mouse values
extern bool		bAxes;					// != 0 means to draw the axes
extern bool		bProjection;		// ORTHO or PERSP
extern bool     bAnimate;
extern bool     bTranslate;
extern bool     bScale;
extern bool     bHue;
extern bool     bExit;
extern bool     bOneIter;
extern bool     bCompIter;
extern bool     bAutoRotate;


extern TwBar* bar;

extern GLuint AxesList;
extern GLuint SphereList;

//rotations
extern float g_Rotation[];
extern float g_Rotation_Scene[];
extern float g_Scene_Angle_x;
extern float g_Scene_Angle_y;
extern float color[];
extern float colorp[];

extern Lsystem* Lsys;
extern std::list<std::string>   systems;
extern std::list<Turtle>        turt_sys;
extern std::vector<glm::vec3>   current_turtles;
extern std::vector<glm::vec3>   vertices;
extern std::vector<glm::vec3>   prev_vertices;
extern bool bDraw;
extern int  Generation;
extern int  iSkip;
extern float angle;

extern glm::vec3 autoScale;

// l-system

extern float angle;
extern std::string axiom;
extern std::vector<std::string> rules;
extern std::string vars;

#endif
