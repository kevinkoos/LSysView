#include "header.h"

//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Joe Graphics

// NOTE: There are a lot of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// to be used as an array size or as the case in a switch( ) statement.  So in
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch( ) statements.  Those are #defines.


// title of these windows:
const char *WINDOWTITLE = { "OpenGL / GLUT Sample -- Joe Graphics" };
const char *GLUITITLE   = { "User Interface Window" };

// what the glui package defines as true and false:
const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };

// the escape key:
#define ESCAPE		0x1b

// initial window size:
const int INIT_WINDOW_HEIGHT = { 800 };
const int INIT_WINDOW_WIDTH = { 600 };

// minimum allowable scale factor:
const float MINSCALE = { 0.05f };

// ENUMS //

// window background color (rgba):
const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };

// line width for the axes:
const GLfloat AXES_WIDTH   = { 3. };
const GLfloat AXES_LENGTH = { 3. };

// active mouse buttons (or them together):
const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// non-constant global variables:
int		ActiveButton;			// current button that is down
bool	AxesOn;					// != 0 means to draw the axes
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
bool    Frozen;
bool	WhichProjection;
int		Xmouse, Ymouse;			// mouse values
float   Time;
int     ms;

GLuint AxesList;

// rotations and atb stuff
float g_Rotation[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float g_Rotation_Scene[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float g_Scene_Angle_x;
float g_Scene_Angle_y;
float mat[4*4]; // rotation matrix
float quat[4];
float color[] { 1.0f, 1.0f, 1.0f };

// lsystem and turtles
Lsystem* Lsys;
std::list<std::string>   systems;
std::list<Turtle>        turt_sys;
std::vector<glm::vec3>   current_turtles;
std::vector<glm::vec3>   vertices;
std::vector<glm::vec3>   prev_vertices;
bool loop;

//tmp stuff
float angle = 90;
std::string axiom = "A";
std::vector<std::string> rules{"-BF+AFA+FB-","+AF-BFB-FA+"};
std::string vars = "AB";

// main program:
int main( int argc, char *argv[ ] ) {
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)
	glutInit( &argc, argv );

	// setup all the graphics stuff:
	InitGraphics( );

    // setup gui and anttweaksbar
    InitGUI( );
    
    Lsys = new Lsystem(axiom, rules, vars);
    Lsys->next();
    Lsys->next();
    Lsys->next();
    Lsys->next();
    systems.push_back(Lsys->get());
    turt_sys.push_back(Turtle());
    vertices.push_back(glm::vec3(0., 0., 0.));
    prev_vertices.push_back(glm::vec3(0., 0., 0.));
    
	// create the display structures that will not change:
	InitLists( );

	// init all the global variables used by Display( ):
	// this will also post a redisplay
	Reset(NULL);

	// draw the scene once and wait for some interaction:
	// (this will never return)
	glutSetWindow( MainWindow );
	glutMainLoop( );

	// this is here to make the compiler happy:
	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void Animate( ) {
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:
	// force a call to Display( ) next time it is convenient:
    //ms = glutGet( GLUT_ELAPSED_TIME );
    //ms %= MS_PER_CYCLE;
    //Time = (float)ms / (float)MS_PER_CYCLE;		// [0.,1.)
    
    //make ms a counter
    //not exact in cycle timing but allows smooth starts in animation
    //also useful for iterating turtle cycles
    if(Frozen) { ms += 1; } 
    ms %= MS_PER_CYCLE;
    Time = ms/(float)MS_PER_CYCLE;
    
    
    
    
	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}

// produces the next iteration in lsystem
void NextIteration(void* dt) {
    char c;
    int op_br;
    std::list<Turtle>::iterator turtle;
    std::list<std::string>::iterator t_string;
    do {
        turtle = turt_sys.begin();                      //turtle back to begining
        t_string = systems.begin(); 
        current_turtles.clear();
        while (turtle != turt_sys.end()) {
            c = (*t_string)[0];
            (*t_string).erase(0,1);
            
            switch(c) {
                case DRAW:
                case FORWARD:
                    prev_vertices.push_back((*turtle).get_pos());
                    (*turtle).forward();
                    vertices.push_back((*turtle).get_pos());
                    current_turtles.push_back((*turtle).get_pos());
                    turtle++;                                //next turtle
                    t_string++;                              //next turtle string
                    break;
            
                case YAW_UP:
                    (*turtle).yaw(angle);
                    break;
                    
                case YAW_DOWN:
                    (*turtle).yaw(-angle);
                    break;
                    
                case PITCH_UP:
                    (*turtle).pitch(angle);
                    break;
                    
                case PITCH_DOWN:
                    (*turtle).pitch(-angle);
                    break;
                    
                case ROLL_UP:
                    (*turtle).roll(angle);
                    break;
                    
                case ROLL_DOWN:
                    (*turtle).roll(-angle);
                    break;
                    
                case BRANCH:                            //add a branching path to tree, new turtle
                    //put all into a function later
                    op_br = 1;
                    systems.push_back(std::string());
                    turt_sys.push_back(Turtle(*turtle));
                    while(op_br != 0) {                 //count '[' / ']' until 0 
                        c = (*t_string)[0];
                        (*t_string).erase(0,1);         //chars from original string removed
                        if (c == '[') {
                            op_br++;
                        } else if (c == ']') {
                            op_br--;
                        }
                        systems.back().push_back(c);    //append chars to new turtle string
                    }
                    systems.back().pop_back();          //remove last char ']'
                    break;
                    
                case POLY:                              //draw a polygon from closing '}' turtle positions
                    //make a functions but when integrating into opengl sample app
                    
                    break;
                    
                case END:                               //turtle is at end of string
                    turtle = turt_sys.erase(turtle);    //remove turtle
                    t_string = systems.erase(t_string); //remove its string
                    break;
                    
                default:                                //all other chars are ignored by turtle
                    break;
            }
        }
    } while(loop && !turt_sys.empty());
}

// draw the complete scene:
void Display( ) {

    
	// set which window we want to do the graphics into:
	glutSetWindow( MainWindow );

	// erase the background:
	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_DEPTH_TEST );
    
	// specify shading to be flat:
	//glShadeModel( GL_FLAT );

	// set the viewport to a square centered in the window:
	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );

	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( WhichProjection == ORTHO )
		glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
	else
		gluPerspective( 90., 1.,	0.1, 1000. );

	// place the objects into the scene:
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// set the eye position, look-at position, and up-vector:
	gluLookAt( 0., 0., 5.,     0., 0., 0.,     0., 1., 0. );

    // rotate the scene and scale
    ConvertQuaternionToMatrix(g_Rotation_Scene, mat);
    glMultMatrixf(mat);
    glScalef(Scale, Scale, Scale);

    
	// possibly draw the axes:
	if( AxesOn != 0 ) {
		glColor3f( 1., 1., 1. );
		glCallList( AxesList );
	}

	// since we are using glScalef( ), be sure normals get unitized:
	glEnable( GL_NORMALIZE );
    
    
    // Rotate and draw shape
    glPushMatrix();

    SetQuaternionFromAxisAngle(y_axis, 360*Time, quat);
    ConvertQuaternionToMatrix(quat, mat);
    glMultMatrixf(mat);


    ConvertQuaternionToMatrix(g_Rotation, mat);
    glMultMatrixf(mat);
    
    glColor3f(color[0], color[1], color[2]);
    glBegin( GL_LINES );
    for(int i = 0; i < vertices.size(); i++) {
        glVertex3f(prev_vertices[i].x, prev_vertices[i].y, prev_vertices[i].z);
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
    }
    glEnd();
    
    
    glPopMatrix();
    
    // draw tweak bars
    TwDraw();

	// swap the double-buffered framebuffers:
	glutSwapBuffers( );

	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !
	glFlush( );
}

// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( ) 
void InitLists( ) {
	glutSetWindow( MainWindow );


	// create the axes:
	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( AXES_LENGTH );
		glLineWidth( 1. );
	glEndList( );
}







