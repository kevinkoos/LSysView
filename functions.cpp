/* Name: functions.cpps
 * Desciprion: utility functions for sample.cpp separated for convience and readability
 * Author: Kevin Koos
 */

#include "header.h"

const float ANGFACT = { 0.5f };
const float SCLFACT = { 0.005f };

const int   GLUT_WHEEL_DOWN = { 3 };
const int   GLUT_WHEEL_UP   = { 4 };

// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void InitGraphics( ) {
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:
	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_WIDTH , INIT_WINDOW_HEIGHT);

	// open the window and set its title:
	MainWindow = glutCreateWindow( WINDOWTITLE );
    glutCreateMenu(NULL);
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:
	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );
    
    // GLUT callbacks
	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );
	glutIdleFunc( Animate );
    
	// init glew (a window must be open to do this):
#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK ) {
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
}

// return the number of seconds since the start of the program:
float ElapsedSeconds( ) {
	// get # of milliseconds since the start of the program:
	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:
	return (float)ms / 1000.f;
}

// called when user resizes the window:
void Resize( int width, int height ) {
	// don't really need to do anything since window size is
	// checked each time in Display( ):
    TwWindowSize(width, height);
    
	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}

// handle a change to the window's visibility:
void Visibility ( int state ) {
	if( state == GLUT_VISIBLE ) {
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
}


///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////
// the stroke characters 'X' 'Y' 'Z' :
static float xx[ ] = { 0.f, 1.f, 0.f, 1.f };

static float xy[ ] = { -.5f, .5f, .5f, -.5f };

static int xorder[ ] = { 1, 2, -3, 4 };

static float yx[ ] = { 0.f, 0.f, -.5f, .5f };

static float yy[ ] = { 0.f, .6f, 1.f, 1.f };

static int yorder[ ] = { 1, 2, 3, -2, 4 };

static float zx[ ] = { 1.f, 0.f, 1.f, 0.f, .25f, .75f };

static float zy[ ] = { .5f, .5f, -.5f, -.5f, 0.f, 0.f };

static int zorder[ ] = { 1, 2, 3, 4, -5, 6 };

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)
void Axes( float length ) {
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;
	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ ) {
			int j = xorder[i];
			if( j < 0 ) {
                
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ ) {
			int j = yorder[i];
			if( j < 0 ) {
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ ) {
			int j = zorder[i];
			if( j < 0 ) {
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );
void HsvRgb( float hsv[3], float rgb[3] ) {
	// guarantee valid input:
	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:
	if( s == 0.0 ) {
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	float i = floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r, g, b;			// red, green, blue
	switch( (int) i ) {
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

/* ======================== CALLBACKS =======================*/

// called when the mouse button transitions down or up:
void MouseButton( int button, int state, int x, int y ) {
    //redirect to anttweakbars and run only it doesn't handle it
    if ( !TwEventMouseButtonGLUT(button, state, x, y)) {
        	int b = 0;			// LEFT, MIDDLE, or RIGHT
        // get the proper button bit mask:
        switch( button ) {
            case GLUT_LEFT_BUTTON:
                b = LEFT;		break;

            case GLUT_MIDDLE_BUTTON:
                b = MIDDLE;		break;

            case GLUT_RIGHT_BUTTON:
                b = RIGHT;		break;

            case 3:     //wheel up
                Scale += 0.005;  break;
                
            case 4:     //wheel down
                Scale -= 0.005;  break;
                
            default:
                b = 0;
                fprintf( stderr, "Unknown mouse button: %d\n", button );
        }
        
        // scale guard
        if(Scale < 0.001) { Scale = 0.001; }
        
        // button down sets the bit, up clears the bit:
        if( state == GLUT_DOWN ) {
            Xmouse = x;
            Ymouse = y;
            ActiveButton |= b;		// set the proper bit
        } else {
            ActiveButton &= ~b;		// clear the proper bit
        }
            
    }

}

// called when the mouse moves while a button is down:
void MouseMotion( int x, int y ) {
    //redirect to atb and run our callback only if atb doesn't handle it
    if ( !TwEventMouseMotionGLUT(x, y) ) {
        float q_1[4];                           //quaternion {x,y,z,a}
        float dy = y - Ymouse;
        float dx = x - Xmouse;
        
        if( ( ActiveButton & LEFT ) != 0 ) {
            //x mouse rotation
            g_Scene_Angle_x = ANGFACT*dy;
            SetQuaternionFromAxisAngle(y_axis, g_Scene_Angle_x, q_1);
            MultiplyQuaternions(q_1, g_Rotation_Scene, g_Rotation_Scene);
            //y mouse rotation
            g_Scene_Angle_y = ANGFACT*dx;
            SetQuaternionFromAxisAngle(x_axis, g_Scene_Angle_y, q_1);
            MultiplyQuaternions(q_1, g_Rotation_Scene, g_Rotation_Scene);
        }

        if( ( ActiveButton & MIDDLE ) != 0 ) {
            Scale += SCLFACT * (float) ( dx - dy );
            if(Scale < 0.01) { Scale = 0.01; }
        }
        
        

        Xmouse = x;			// new current position
        Ymouse = y;
        
    }

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}
