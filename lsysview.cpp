#include "header.h"
#include <ctime>

/* LSysView - A viewing applicaiton for Lindenmayer systems
 * This application allows a user to view and interact with various 
 * L-systems of thier own making as well as those pre-defined.
 * Uses OpenGL, GLUT, and AntTweakBars
 * 
 * Author: Kevin Koos
 */


// title of these windows:
const char *WINDOWTITLE = { "LSysView" };

// initial window size:
const int INIT_WINDOW_HEIGHT = { 1000 };
const int INIT_WINDOW_WIDTH = { 1000 };

// minimum allowable scale factor:
const float MINSCALE = { 0.05f };

// ENUMS //

// window background color (rgba):
const GLfloat BACKCOLOR[ ] = { 0.5, 0.5, 0.5, 1. };

// line width for the axes:
const GLfloat AXES_WIDTH   = { 3. };
const GLfloat AXES_LENGTH = { 3. };

// active mouse buttons (or them together):
const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// non-constant global variables:
int		ActiveButton;			// current button that is down
bool	AxesOn;					// true means to draw the axes
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
bool    bAnimate;               // true mean to activate animation
bool	WhichProjection;        // otho or perspective
int		Xmouse, Ymouse;			// mouse values
float   Time;                   // float time
int     ms;                     // ms counter
bool    bDraw;                  // true means draw animation
int     Generation;             // current Generation number of l system
int     iSkip;                  // # of line segments to draw each frame when bDraw true
bool    bTranslate;             // true is auto-translate on
float   pScale;                 // scale of the turtles
bool    bScale;
bool    bHue;

// display lists
GLuint AxesList;
GLuint SphereList;

// rotations and atb stuff
float g_Rotation[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float g_Rotation_Scene[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float g_Scene_Angle_x;          // rotation from mouse x movement
float g_Scene_Angle_y;          // rotation from mouse y movement
float mat[4*4]; // rotation matrix
float quat[4];
float color[] = { 1.0f, 1.0f, 1.0f };
float colorp[] = { 0.0f, 1.0f, 1.0f };
float cx, cy, cz;
float rgb[3];
float hsv[3];
float addColor;
int   MS_PER_CYCLE;

// lsystem and turtles
Lsystem* Lsys;
std::list<std::string>   systems;
std::list<Turtle>        turt_sys;
std::vector<glm::vec3>   current_turtles;
std::vector<glm::vec3>   vertices;
std::vector<glm::vec3>   prev_vertices;

//tmp stuff
glm::vec3 autoScale;

//3d hilbert
float angle = 90;
std::string axiom = "X";
std::vector<std::string> rules{"^<XF^<XFX+F^>>XFXvF->>XFX+F>X+>"};
std::string vars = "X";

//2d hilbert
// float angle = 90;
// std::string axiom = "A";
// std::vector<std::string> rules{"-BF+AFA+FB-","+AF-BFB-FA+"};
// std::string vars = "AB";

//fractal plant
// float angle = 45;
// std::string axiom = "X";
// std::vector<std::string> rules{"F+[[X]-X]-F[-FX]+X","FF"};
// std::string vars = "XF";

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

// animates the scene, 
void Animate( ) {
    //make ms a counter
    //exact timing per machine is unclear but this is easier to work with
    if(bAnimate && bDraw) { ms += 1; }
    ms %= MS_PER_CYCLE;
    
    if(turt_sys.empty()) {
        bDraw = false;
    }
    
	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}

void AutoScale() {
    // auto-scale WIP
    float x_min = 0, y_min = 0, z_min = 0;
    float max_min;
    if (bScale && vertices.size() > 1) {
        x_min = vertices[0].x;
        y_min = vertices[0].y;
        z_min = vertices[0].z;
        for(int i = 1; i < vertices.size(); i++) {
            x_min = std::min(x_min,vertices[i].x);
            y_min = std::min(y_min,vertices[i].y);
            z_min = std::min(z_min,vertices[i].z);
        }
        //get the maximum minimumm, scale by uniform amount
        max_min = std::max(x_min, y_min);
        max_min = std::max(max_min, z_min);
        max_min = abs(max_min);
        
        autoScale[0] = 1/(2*(abs(cx)-max_min));
        autoScale[1] = 1/(2*(abs(cy)-max_min));
        autoScale[2] = 1/(2*(abs(cz)-max_min));
    }
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

    // since we are using glScalef( ), be sure normals get unitized:
	glEnable( GL_NORMALIZE );
    
    // set the eye position, look-at position, and up-vector:
    gluLookAt( 0., 0., 5.0f,     0.0f, 0.0f, 0.0f,     1., 0., 0. );

    // auto-translate to center on origin
    cx = 0; cy = 0; cz = 0;
    if (bTranslate || bScale) {
        for(int i = 0; i < vertices.size(); i++) {
            cx += vertices[i].x;
            cy += vertices[i].y;
            cz += vertices[i].z;
        }
        cx /= vertices.size();
        cy /= vertices.size();
        cz /= vertices.size();
    }

    if (bScale) {
        AutoScale();
        bScale = false;
    }
    
    glScalef(autoScale[0], autoScale[1], autoScale[2]);
    
    // compute an iteration if specified
    if(bAnimate && bDraw && ms == 0) {
        for(int i = 0; i < iSkip; i++) {
            NextIteration();
        }
    }
    
    // rotate the scene and scale
    ConvertQuaternionToMatrix(g_Rotation_Scene, mat);
    glMultMatrixf(mat);
    glScalef(Scale, Scale, Scale);

	// possibly draw the axes:
	if( AxesOn != 0 ) {
		glColor3f( 1., 1., 1. );
		glCallList( AxesList );
	}
    
    // Rotate and draw shape
    glPushMatrix();

    // apply object rotation
    ConvertQuaternionToMatrix(g_Rotation, mat);
    glMultMatrixf(mat);

    
    // translate center to origin
    if (bTranslate) {
        glTranslatef(-cx, -cy, -cz);
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
        glScalef(pScale, pScale, pScale);
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







