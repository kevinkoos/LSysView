#include "header.h"
#include <ctime>

/* LSysView - A viewing applicaiton for Lindenmayer systems
 * This application allows a user to view and interact with various 
 * L-systems of their own making as well as those pre-defined.
 * Uses OpenGL, GLUT, and AntTweakBars
 * 
 * Author: Kevin Koos
 */

// title of these windows:
const char *WINDOWTITLE = { "LSysView" };

// initial window size:
const int INIT_WINDOW_HEIGHT = { 1000 };
const int INIT_WINDOW_WIDTH = { 1400 };

// minimum allowable scale factor:
const float MINSCALE = { 0.05f };

// window background color (rgba):
const GLfloat BACKCOLOR[ ] = { 0.5f , 0.5f , 0.5f , 1.0f };

// line width for the axes:
const GLfloat AXES_WIDTH   = { 3.0f };
const GLfloat AXES_LENGTH = { 3.0f };

// active mouse buttons (or them together):
const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };

// non-constant global variables:
float   Time;                   // float time
float   fPointScale;            // scale of the turtles
float	Scale;					// scaling factor
int		MainWindow;				// window id for main graphics window
int     Generation;             // current Generation number of l system
int     iFramePerCycle;         // # of frames between animated system updates
int     iSkip;                  // # of line segments to draw each frame when bDraw true
int     iCounter;               // counter
int		Xmouse, Ymouse;			// mouse values
int		ActiveButton;			// current button that is down
bool	bAxes;					// true means to draw the axes
bool    bAnimate;               // true mean to activate animation
bool	bProjection;            // otho or perspective
bool    bDraw;                  // true means draw animation
bool    bTranslate;             // true is auto-translate on
bool    bScale;                 // true means to scale the scene down once, set false after
bool    bHue;                   // true means to add a continuously changing hue to the curve
bool    bOneIter;               // true means draw one turtle iteration
bool    bCompIter;              // true means draw until the curve is complete
bool    bAutoRotate;            // true means to rotate on the y axis
bool    bExit;                  // true means to exit applicaiton

// display lists
GLuint AxesList;
GLuint SphereList;

// rotations and atb stuff
float g_Rotation[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float g_Rotation_Scene[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float g_Rotation_Animation[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float g_Scene_Angle_x;          // rotation from mouse x movement
float g_Scene_Angle_y;          // rotation from mouse y movement
float g_Rotation_Angle = 0;     // rotation from auto rotation
float mat[4*4];                 // tmp rotation matrix
float color[] = { 1.0f, 1.0f, 1.0f };
float colorp[] = { 0.0f, 1.0f, 1.0f };
float rgb[3];
float hsv[3];
float addColor;
glm::vec3 autoScale;

// lsystem and turtles
Lsystem* Lsys;
std::list<std::string>   systems;
std::list<Turtle>        turt_sys;
std::vector<glm::vec3>   current_turtles;
std::vector<glm::vec3>   vertices;
std::vector<glm::vec3>   prev_vertices;
float p_angle = angle;

// main program:
int main( int argc, char *argv[ ] ) {
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)
	glutInit( &argc, argv );

	// setup all the graphics stuff:
	InitGraphics( );
        
    // inialize lsys
    Lsys = new Lsystem();
    
	// create the display structures that will not change:
	InitLists( );

    // setup gui and anttweaksbar
    InitGUI( );
    
	// init all the global variables used by Display( ):
	Reset(NULL);
    
	// draw the scene once and wait for some interaction:
	// (this will never return)
	glutSetWindow( MainWindow );
	glutMainLoop( );

	// this is here to make the compiler happy:
	return 0;
}

// animates the scene, 
void Animate( ) {
    // add one to the counter
    if(bAnimate && bDraw) { iCounter += 1; }
    iCounter %= iFramePerCycle;
    // stop drawing if we're done'
    if(turt_sys.empty()) {
        bDraw = false;
    }
    
	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}

// scale a drawing down uniformly by the largest edge size of the curves bounding box
void AutoScale() {
    // get the minimum and maxmum values for x,y, and z
    auto x_minmax = std::minmax_element(vertices.begin(), vertices.end(),
      [](const glm::vec3 &a, const glm::vec3 &b) { return a.x < b.x; });
    auto y_minmax = std::minmax_element(vertices.begin(), vertices.end(),
      [](const glm::vec3 &a, const glm::vec3 &b) { return a.y < b.y; });
    auto z_minmax = std::minmax_element(vertices.begin(), vertices.end(),
      [](const glm::vec3 &a, const glm::vec3 &b) { return a.z < b.z; });
    
    // determine the edge length
    float dx = abs((*x_minmax.second).x - (*x_minmax.first).x);
    float dy = abs((*y_minmax.second).y - (*y_minmax.first).y);
    float dz = abs((*z_minmax.second).z - (*z_minmax.first).z);
    
    // checks for 0s, 
    if (dx < 0.01) { dx = 1; }
    if (dy < 0.01) { dy = 1; }
    if (dz < 0.01) { dz = 1; }
    
    float maxmin;
    if (dx > dy && dx > dz) { maxmin = dx; }
    else if(dy > dz) { maxmin = dy; }
    else { maxmin = dz; }
    
    autoScale[0] = 1/maxmin;
    autoScale[1] = 1/maxmin;
    autoScale[2] = 1/maxmin;
}

// calculate the center of the curve through averages and centers curve on origin
void AutoTranslate() {
    float cx = 0, cy = 0, cz = 0;
    for(int i = 0; i < vertices.size(); i++) {
        cx += vertices[i].x;
        cy += vertices[i].y;
        cz += vertices[i].z;
    }
    cx /= vertices.size();
    cy /= vertices.size();
    cz /= vertices.size();
    glTranslatef(-cx, -cy, -cz);
}

// produces the next iteration in lsystem
// iterates through all turtles until they have all drawn once
// parses strings, interprets chars, and adds branches
// enum for chars is located in turtle.hpp
void NextIteration() {
    char c;
    int op_br;
    std::list<Turtle>::iterator turtle;
    std::list<std::string>::iterator t_string;

    turtle = turt_sys.begin();                      
    t_string = systems.begin(); 
    current_turtles.clear();
    // loop through all the turtles in the system
    while (turtle != turt_sys.end()) {
        // get next char in current turtle string
        c = (*t_string)[0];
        (*t_string).erase(0,1);
        // do action based off char, anything not specified is ignored
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
                
            case POLY:                              //draw a polygon to closing '}' turtle positions
                
                
                break;
                
            case END:                               //turtle is at end of string
                turtle = turt_sys.erase(turtle);    //remove turtle
                t_string = systems.erase(t_string); //remove its string
                break;
                
            default:                                //all other chars are ignored by turtle
                break;
        }
    }

}

// draw the complete scene:
void Display( ) {
	// set which window we want to do the graphics into:
	glutSetWindow( MainWindow );

	// erase the background:
	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // enable depth
    glEnable( GL_DEPTH_TEST );
    
	// specify shading to be flat:
	// glShadeModel( GL_FLAT );

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
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( bProjection == ORTHO ) {
        glOrtho( -3., 3., -3., 3., -100, 100000. );
    } else {
		gluPerspective( 90., 1., 0.1, 1000. );
    }

	// place the objects into the scene:
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

    // since we are using glScalef( ), be sure normals get unitized:
	glEnable( GL_NORMALIZE );
    
    // set the eye position, look-at position, and up-vector:
    gluLookAt( 0., 0., 1.0f,     0.0f, 0.0f, 0.0f,     1., 0., 0. );

    // if the angle has changed, draw the complete scene
    if (angle != p_angle) {
        CompleteDrawHandle(NULL);
        p_angle = angle;
    }
    
    // compute an iteration if specified
    if(bAnimate && bDraw && iCounter == 0) {
        for(int i = 0; i < iSkip; i++) {
            NextIteration();
        }
    } else if(bOneIter || bCompIter){ 
        do {
            
            NextIteration();
            
        } while(bCompIter && !turt_sys.empty());
        bOneIter = false;
        bCompIter = false;
    }
    
    // apply an auto rotation on the vertical axis (x-axis)
    if (bAutoRotate) {
        g_Rotation_Angle += 0.5;
        SetQuaternionFromAxisAngle(x_axis, g_Rotation_Angle, g_Rotation_Animation);
        ConvertQuaternionToMatrix(g_Rotation_Animation, mat);
        glMultMatrixf(mat);
    }
    
    // rotate the scene and scale
    ConvertQuaternionToMatrix(g_Rotation_Scene, mat);
    glMultMatrixf(mat);
    glScalef(Scale, Scale, Scale);

	// possibly draw the axes:
	if( bAxes != 0 ) {
		glColor3f( 1., 1., 1. );
		glCallList( AxesList );
	}
    
    // call autoscale. somewhat costly function to run every display so bScale controls it
    if (bScale) {
        AutoScale();
        bScale = false;
    }
    
    // apply the autoscaleing to the object
    glScalef(autoScale[0], autoScale[1], autoScale[2]);
    
    // Rotate and draw shape
    glPushMatrix();

    // apply object rotation
    ConvertQuaternionToMatrix(g_Rotation, mat);
    glMultMatrixf(mat);

    // translate center to origin

    if (bTranslate) {
        AutoTranslate();
    }

    // set hue color var if we're coloring
    if (bHue) {
        addColor = 360.0f/(float)vertices.size();
        hsv[0] = 0; hsv[1] = 1; hsv[2] = 1;
    }

    // draw the lines
    glColor3f(color[0], color[1], color[2]);
    glBegin( GL_LINES );
    for(int i = 0; i < vertices.size(); i++) {
        // apply a steady hue change 
        if(bHue) {
            hsv[0] += addColor;
            HsvRgb(hsv, rgb);
            glColor3f(rgb[0], rgb[1], rgb[2]);
            
        }
        glVertex3f(prev_vertices[i].x, prev_vertices[i].y, prev_vertices[i].z);
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
    }
    glEnd();
    
    // draw the turtles
    glColor3f(colorp[0], colorp[1], colorp[2]);
    for(int j = 0; j < current_turtles.size(); j++) {
        glPushMatrix();
        glTranslatef(current_turtles[j].x, current_turtles[j].y, current_turtles[j].z);
        glScalef(fPointScale, fPointScale, fPointScale);
        glCallList(SphereList);
        glPopMatrix();
    }

    glPopMatrix();
    
    // draw tweak bars
    TwDraw();

	// swap the double-buffered framebuffers:
	glutSwapBuffers( );

	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !
	glFlush( );
    
    if (bExit) {
        glutSetWindow( MainWindow );
        glFinish( );
        glDeleteLists(AxesList, 1);
        glDeleteLists(SphereList, 1);
        TwTerminate();
        glutDestroyWindow( MainWindow );
    }
}

// initialize the display lists that will not change:
void InitLists( ) {
	glutSetWindow( MainWindow );

	// the axes
	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( AXES_LENGTH );
		glLineWidth( 1. );
	glEndList( );
    
    // small sphere
    SphereList = glGenLists(1);
    glNewList( SphereList, GL_COMPILE );
        glutSolidSphere(0.1,5,5);
    glEndList();
}







