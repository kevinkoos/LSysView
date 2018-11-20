/* Name: gui.cpp
 * Description: source related to the gui and callbacks
 * Author: Kevin Koos
 */ 
#include "header.h"

//safelty close everything
void Terminate(void* dt) {
    glutSetWindow( MainWindow );
    glDeleteLists(AxesList, 1);
    glDeleteLists(SphereList, 1);
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
    bAnimate = false;
    ms = 0;
    Time = 0;
    iDrawSpeed = 4;
    MS_PER_CYCLE = 50;
    Generation = Lsys->get_gen();
    iSkip = 1;
    
    //reset object rotation
    QuatReset(g_Rotation);
    QuatReset(g_Rotation_Scene);
}

void CompleteDrawHandle(void* data) {
    NextIteration(true);
}

void NextIterHandle(void* data) {
    NextIteration(false); //draw complete scene
}

void AnimateDraw(void* data) {
    bDraw = true;
}

void ResetDrawing(void* data) {
    //clear vectors
    vertices.clear();
    prev_vertices.clear();
    current_turtles.clear();
    turt_sys.clear();
    systems.clear();
    
    //initalize
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    prev_vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    turt_sys.push_back(Turtle());
    systems.push_back(Lsys->get());
    
    Generation = Lsys->get_gen();
    bDraw = false;
}

void NextLsystem(void* data) {
    Lsys->next();
    Generation = Lsys->get_gen();
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
    TwDefine(" Controls size='250 350' color='160 160 160' ");
    
    TwAddVarRW(bar, "Scale", TW_TYPE_FLOAT, &Scale, 
               " min=0.01 max=2.5 step=0.01 keyIncr=s keyDecr=S help='Scale the object (1=original size).' ");

    TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation, 
               " label='Object rotation' opened=true help='Change the object orientation.' ");
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddVarRW(bar, "Axes", TW_TYPE_BOOLCPP, &AxesOn, " key=a ");
    TwAddVarRW(bar, "Projection", TW_TYPE_BOOLCPP, &WhichProjection, " key=p ");
    TwAddVarRW(bar, "Animation", TW_TYPE_BOOLCPP, &bAnimate, " key=f ");
    TwAddVarRW(bar, "Animation Speed", TW_TYPE_INT32, &MS_PER_CYCLE, " min=1 max=100 step=1 ");
    TwAddVarRW(bar, "Animation Skip", TW_TYPE_INT32, &iSkip, " min=1 max=10 step=1 ");
    
    TwAddVarRW(bar, "Color", TW_TYPE_COLOR3F, &color, " colormode=hls ");
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddButton(bar, "Complete Draw", &CompleteDrawHandle, NULL, NULL);
    TwAddButton(bar, "Next Iteration", &NextIterHandle, NULL, NULL);
    TwAddButton(bar, "Animated Draw", &AnimateDraw, NULL, NULL);
    TwAddButton(bar, "Reset Lsys", &ResetDrawing, NULL, NULL);
    TwAddButton(bar, "Next Lsys", &NextLsystem, NULL, NULL);
    TwAddVarRO(bar, "Generation:", TW_TYPE_INT32, &Generation, NULL);
    
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddButton(bar, "RESET", &Reset, NULL, " key=R ");
    TwAddButton(bar, "EXIT", &Terminate, NULL, " key=q ");
    
}






