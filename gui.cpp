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

// resets vars associated with "camera", resets obj and scene rotations
void ResetCamera(void* data) {
            
    autoScale[0] = 1;
    autoScale[1] = 1;
    autoScale[2] = 1;
    
    //reset object rotation
    QuatReset(g_Rotation);
    QuatReset(g_Rotation_Scene);
    
    Scale  = 1.0;
    pScale = 1.0;
}

// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene
void Reset(void* data) {
	AxesOn = false;
	Scale  = 1.0;
    pScale = 1.0;
    bScale = false;
	WhichProjection = PERSP;
    bAnimate = true;
    bTranslate = true;
    ms = 0;
    Time = 0;
    MS_PER_CYCLE = 10;
    Generation = Lsys->get_gen();
    iSkip = 1;
        
    //reset camera stuff
    ResetCamera(NULL);
    
    //reset lsys
    ResetDrawing(NULL);
}

void CompleteDrawHandle(void* data) {
    if(!vertices.empty()){
        ResetDrawing(NULL);
    }
    while(!turt_sys.empty()) {
        NextIteration();
    }
    bScale = true;
}

void NextIterHandle(void* data) {
    NextIteration(); 
}

void AnimateDraw(void* data) {
    if(!vertices.empty()){
        ResetDrawing(NULL);
    }
    bDraw = true;
}

void NextLsystem(void* data) {
    Lsys->next();
    Generation = Lsys->get_gen();
}

void AutoScale(void* data) {
    bScale = true;
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
    TwDefine(" Controls size='250 500' color='110 110 110' ");
    
    TwAddVarRW(bar, "Scale", TW_TYPE_FLOAT, &Scale, 
               " min=0.001 max=2.5 step=0.001 keyIncr=s keyDecr=S help='Scale the object (1=original size).' ");
    TwAddVarRW(bar, "Turtle Scale", TW_TYPE_FLOAT, &pScale, 
               " min=0.01 max=5 step=0.01 help='Scale the size of the turtles.' ");

    TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation, 
               " label='Object rotation' opened=true help='Change the object orientation.' ");
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddVarRW(bar, "Axes", TW_TYPE_BOOLCPP, &AxesOn, " key=a ");
    TwAddVarRW(bar, "Projection", TW_TYPE_BOOLCPP, &WhichProjection, " key=p ");
    TwAddVarRW(bar, "Animation", TW_TYPE_BOOLCPP, &bAnimate, " key=f ");
    TwAddVarRW(bar, "Animation Speed", TW_TYPE_INT32, &MS_PER_CYCLE, " min=1 max=100 step=1 ");
    TwAddVarRW(bar, "Animation Skip", TW_TYPE_INT32, &iSkip, " min=1 max=10 step=1 ");
    TwAddVarRW(bar, "Auto-Center", TW_TYPE_BOOLCPP, &bTranslate, " key=t ");
    TwAddButton(bar, "Scale Down", &AutoScale, NULL, " key=s ");
    TwAddVarRW(bar, "Color Hue", TW_TYPE_BOOLCPP, &bHue, NULL);
    
    TwAddVarRW(bar, "Color", TW_TYPE_COLOR3F, &color, " colormode=hls ");
    TwAddVarRW(bar, "Turtle Color", TW_TYPE_COLOR3F, &colorp, " colormode=hls ");
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddButton(bar, "Complete Draw", &CompleteDrawHandle, NULL, NULL);
    TwAddButton(bar, "Next Iteration", &NextIterHandle, NULL, NULL);
    TwAddButton(bar, "Animated Draw", &AnimateDraw, NULL, NULL);
    TwAddButton(bar, "Reset Lsys", &ResetDrawing, NULL, NULL);
    TwAddButton(bar, "Next Lsys", &NextLsystem, NULL, NULL);
    TwAddVarRO(bar, "Generation:", TW_TYPE_INT32, &Generation, NULL);
    
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddButton(bar, "RESET CAMERA", &ResetCamera, NULL, NULL);
    TwAddButton(bar, "RESET", &Reset, NULL, " key=R ");
    TwAddButton(bar, "EXIT", &Terminate, NULL, " key=q ");
    
}






