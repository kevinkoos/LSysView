/* Name: gui.cpp
 * Description: source related to the gui and callbacks
 * Author: Kevin Koos
 */ 
#include "header.h"

//safelty close everything
void Terminate(void* dt) {
    glutSetWindow( MainWindow );
    glFinish( );
    glutDestroyWindow( MainWindow );
    TwTerminate();
}

// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene
void Reset(void* dt) {
	AxesOn = 1;
	Scale  = 1.0;
	WhichProjection = PERSP;
    Frozen = false;
    ms = 0;
    Time = 0;
    //reset object rotation
    QuatReset(g_Rotation);
    QuatReset(g_Rotation_Scene);
}

void CompleteDrawHandle(void* dt) {
    NextIteration(true);
}

void NextIterHandle(void* dt) {
    NextIteration(false); //draw complete scene
}

void AnimateDraw(void* dt) {
    bDraw = true;
}

void InitGUI(void) {
    
    //initalize anttweakbars
    TwBar* bar;
    
    //initalize tweak bars
    TwInit(TW_OPENGL, NULL);
    
    //redirect glut callbacks to anttweakbars callbacks
    glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);    
    glutMouseFunc((GLUTmousebuttonfun)MouseButton);
    glutMotionFunc((GLUTmousemotionfun)MouseMotion);
    glutSpecialFunc( (GLUTspecialfun)TwEventSpecialGLUT );
    glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    
    //allows for the use of modifier keys
    TwGLUTModifiersFunc(glutGetModifiers);
        

    
    // create tweak bar
    bar = TwNewBar("Controls");
    TwDefine(" GLOBAL help='Basic tweak bar.' ");
    TwDefine(" Controls size='200 300' color='160 160 160' ");
    
    TwAddVarRW(bar, "Scale", TW_TYPE_FLOAT, &Scale, 
               " min=0.01 max=2.5 step=0.01 keyIncr=s keyDecr=S help='Scale the object (1=original size).' ");

    TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation, 
               " label='Object rotation' opened=true help='Change the object orientation.' ");
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddVarRW(bar, "Axes", TW_TYPE_BOOLCPP, &AxesOn, " key=a ");
    TwAddVarRW(bar, "Projection", TW_TYPE_BOOLCPP, &WhichProjection, " key=p ");
    TwAddVarRW(bar, "Animation", TW_TYPE_BOOLCPP, &Frozen, " key=f ");
    
    TwAddVarRW(bar, "Color", TW_TYPE_COLOR3F, &color, " colormode=hls ");
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddButton(bar, "Complete Draw", &CompleteDrawHandle, NULL, NULL);
    TwAddButton(bar, "Next Iteration", &NextIterHandle, NULL, NULL);
    TwAddButton(bar, "Animated Draw", &AnimateDraw, NULL, NULL);
    
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddButton(bar, "RESET", &Reset, NULL, " key=R ");
    TwAddButton(bar, "EXIT", &Terminate, NULL, " key=q ");
    
}






