/* Name: gui.cpp
 * Description: source related to the gui and callbacks
 * Author: Kevin Koos
 */ 
#include "header.h"

// non constant globals used by the gui
int iCurrentGen;
int iRules;
int numCurves;
int currentCurve;
TwBar* bar;
TwBar* stringbar;

//l-system vars
float angle;
std::string axiom;
std::vector<std::string> rules(10);
std::string vars;

//pre-defined curves from presets.txt
std::vector<struct Curve> DefinedCurves;

// struct to organize stored curve variables
struct Curve {
    std::string name;
    float angle;
    std::string axiom;
    std::vector<std::string> rules;
    std::string vars;
};

// Function Prototypes //
void InitGUI(void);
void SetLsystem(void* data);
void NewRule(void* data);
void LoadLsystem(void* data);
void Terminate(void* data);
void ResetDrawing(void* data);
void ResetCamera(void* data);
void Reset(void* data);
void CompleteDrawHandle(void* data);
void NextIterHandle(void* data);
void AnimateDraw(void* data);
void GenUp(void* data);
void GenDown(void* data);
void TW_CALL CopyStdStringToClient(std::string& destination, const std::string& source);
void SetEnumVals(TwEnumVal* curveEV);


// initalize gui elements
void InitGUI(void) {
    
    //initalize tweak bars
    TwInit(TW_OPENGL, NULL);
    
    //redirect glut callbacks to anttweakbars callbacks
    glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);    
    glutMouseFunc((GLUTmousebuttonfun)MouseButton);
    glutMotionFunc((GLUTmousemotionfun)MouseMotion);
    glutSpecialFunc( (GLUTspecialfun)TwEventSpecialGLUT );
    glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);

    // load preset curves from presets.txt in directory
    LoadPresets();

    // allows for the use of modifier keys
    TwGLUTModifiersFunc(glutGetModifiers);
            
    // Define the required callback function to copy a std::string
    TwCopyStdStringToClientFunc(CopyStdStringToClient);

    // create main tweak bar
    bar = TwNewBar("Controls");
    TwDefine(" GLOBAL help='Basic tweak bar.' ");
    TwDefine(" Controls size='250 400' color='110 110 110' ");
    
    TwAddVarRW(bar, "Scale", TW_TYPE_FLOAT, &Scale, 
        " min=0.01 step=0.01 keyIncr=s keyDecr=S help='Scale the object (1=original size).' ");
    TwAddVarRW(bar, "Turtle Scale", TW_TYPE_FLOAT, &fPointScale, 
        " min=0.01 max=5 step=0.01 help='Scale the size of the turtles.' ");
    TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation, 
        " label='Object rotation' opened=false help='Change the object orientation.' ");
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddVarRW(bar, "Axes", TW_TYPE_BOOLCPP, &bAxes, " key=a help='Turn the axes on or off.' ");
    TwAddVarRW(bar, "Projection", TW_TYPE_BOOLCPP, &bProjection,
        " key=p help='Change between perspective and orthographic projection.' ");
    TwAddVarRW(bar, "Auto-Rotate", TW_TYPE_BOOLCPP, &bAutoRotate,
        " help='Applies a steady rotation about the y-axis on the object.' ");
    TwAddVarRW(bar, "Animation", TW_TYPE_BOOLCPP, &bAnimate, " key=f help='Toggle on or off the animation.' ");
    TwAddVarRW(bar, "Animation Speed", TW_TYPE_INT32, &iFramePerCycle, NULL);
    TwAddVarRW(bar, "Animation Skip", TW_TYPE_INT32, &iSkip, 
        " min=1 max=10 step=1 help='Control the number of iterations to draw per draw when drawing the animation.' ");
    TwAddVarRW(bar, "Color Hue", TW_TYPE_BOOLCPP, &bHue,
        " help='Add a steadily changing color change to the curve. Working in order of drawing.' ");
    TwAddVarRW(bar, "Color", TW_TYPE_COLOR3F, &color,
        " colormode=hls help='Change the color of the curve.' ");
    TwAddVarRW(bar, "Turtle Color", TW_TYPE_COLOR3F, &colorp, 
        " colormode=hls help='Change the color of the turtles.' ");
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddButton(bar, "Complete Draw", &CompleteDrawHandle, NULL, 
        " help='Draw a complete scene of the curve.' ");
    TwAddButton(bar, "Next Iteration", &NextIterHandle, NULL, 
        " help='Draw the next iteration of the curve. Can be used mid animation if frozen.' ");
    TwAddButton(bar, "Animated Draw", &AnimateDraw, NULL,
        " help='Does an animated drawing of the curve. Can draw multiple turtles in branching systems.' ");
    TwAddVarRO(bar, "Current Gen:", TW_TYPE_INT32, &iCurrentGen, 
        " help='The current generation of the curve. Starts at generation 0.' ");
    TwAddButton(bar, "Gen UP", &GenUp, NULL, 
        " key=RIGHT help='Increase the current curve generation by 1 and do a complete draw.' ");
    TwAddButton(bar, "Gen Down", &GenDown, NULL, 
        " key=LEFT help='Decrease the current generation of the curve and do a complete draw.' ");
  
    
    TwAddSeparator(bar, NULL, NULL);
    TwAddButton(bar, "RESET CAMERA", &ResetCamera, NULL, NULL);
    TwAddButton(bar, "RESET", &Reset, NULL, " key=R ");
    TwAddButton(bar, "EXIT", &Terminate, NULL, " key=q ");
    
    // create the secondary tweak bar
    stringbar = TwNewBar("L-System");
    TwDefine(" L-System size='250 230' color='110 110 110' position='1120 15' valueswidth=150 ");
    
    {
        TwEnumVal* curveEV = new TwEnumVal[numCurves];
        SetEnumVals(curveEV);
        TwType curveType = TwDefineEnum("CurveType", curveEV, numCurves);
        TwAddVarRW(stringbar, "Curve", curveType, &currentCurve, " keyIncr=UP keyDecr=DOWN ");
        TwAddButton(stringbar, "Load L-System", &LoadLsystem, NULL, " key=RETURN ");
    }
    
    TwAddVarRW(stringbar, "Axiom", TW_TYPE_STDSTRING, &axiom, NULL);
    TwAddVarRW(stringbar, "Variables", TW_TYPE_STDSTRING, &vars, NULL);
    TwAddVarRW(stringbar, "Angle", TW_TYPE_FLOAT, &angle, 
        " min=0 max=360 step=0.1 precision=2 help='Increase or Decrease the working angle of the curve. Forces a complete draw of the curve.' ");
    TwAddButton(stringbar, "Add Rule", &NewRule, NULL, NULL);
    TwAddButton(stringbar, "Set L-System", &SetLsystem, NULL, NULL);    
}

// reset variables associated with the curve itself
void ResetDrawing(void* data) {    
    //clear vectors containing the previous curve
    vertices.clear();
    prev_vertices.clear();
    current_turtles.clear();
    turt_sys.clear();
    systems.clear();
    
    //initalize vectors to start a new drawing
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    prev_vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    turt_sys.push_back(Turtle());
    systems.push_back(Lsys->get(iCurrentGen));
    
    Generation = Lsys->get_gen();
    bDraw = false;
}

// resets vars associated with "camera", resets obj and scene rotations
void ResetCamera(void* data) {
    
    //reset object rotation
    QuatReset(g_Rotation);
    QuatReset(g_Rotation_Scene);
    
    Scale  = 1.0;
    fPointScale = 1.0;
}

// reset everything
void Reset(void* data) {
	bAxes = false;
    bScale = false;
    bExit = false;
	bProjection = PERSP;
    bAnimate = true;
    bTranslate = true;
    iCounter = 0;
    Time = 0;
    iFramePerCycle = 10;
    Generation = Lsys->get_gen();
    iCurrentGen = 0;
    iSkip = 1;
    SetLsystem(NULL);
                    
    autoScale[0] = 1;
    autoScale[1] = 1;
    autoScale[2] = 1;
    
    //reset camera stuff
    ResetCamera(NULL);
    
    //reset lsys
    ResetDrawing(NULL);
    
    //load first preset and set it as current
    LoadLsystem(NULL);
    SetLsystem(NULL);
}

// add a new rule to the gui
void NewRule(void* data) {
    if (iRules < 10) {
        std::string name = "Rule ";
        iRules++;
        name.append(std::to_string(iRules));
        TwAddVarRW(stringbar, name.c_str(), TW_TYPE_STDSTRING, &(rules[iRules-1]), NULL);
    } else {
        std::cerr << "Too many rules." << std::endl;
    }
    
}


// sets the current lsystem to those in the gui vars
void SetLsystem(void* data) {
    delete Lsys;
    Lsys = new Lsystem(axiom, rules, vars);
    iCurrentGen = 0;
}

// load selected curve into gui vars
void LoadLsystem(void* data) {
    // get selected vars
    angle = DefinedCurves[currentCurve].angle;
    axiom = DefinedCurves[currentCurve].axiom;
    vars  = DefinedCurves[currentCurve].vars;
    
    // vector resizes with '=' operator, need to preserve original rules length 
    int CurveSize = DefinedCurves[currentCurve].rules.size();
    for(int i = 0; i < rules.size(); i++) {
        if(i < CurveSize) {
            rules[i] = DefinedCurves[currentCurve].rules[i];
        } else {
            rules[i] = "";
        }
    }
    
    // add new rules to gui for new ones from selected curve
    if (CurveSize > iRules) {
        int add = CurveSize - iRules;
        for(int i = 0; i < add; i++) {
            NewRule(NULL);
        }
    }
    
    // set the new curve
    SetLsystem(NULL);
    ResetDrawing(NULL);
}

// loads presets from a file names presets.txt into an array of struct called DefinedCurves
void LoadPresets() {
    std::ifstream file("presets.txt");
    std::string tmpStr;
    
    file >> numCurves;
    DefinedCurves.resize(numCurves);
    for(int i = 0; i < numCurves; i++) {
        file.ignore(1,'\n');
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(file,tmpStr);
        DefinedCurves[i].name = tmpStr;
        file >> DefinedCurves[i].angle;
        file >> DefinedCurves[i].axiom;
        file >> DefinedCurves[i].vars;
        for(int j = 0; j < DefinedCurves[i].vars.length(); j++) {
            file >> tmpStr;
            DefinedCurves[i].rules.push_back(tmpStr);
        }
    }
}

// sets the AntTweakBar enum values and name pairs to predefined curves
void SetEnumVals(TwEnumVal* curveEV) {
    for(int i = 0; i < numCurves; i++) {
        curveEV[i].Value = i;
        curveEV[i].Label = DefinedCurves[i].name.c_str();
    }
}

// safely close everything
// sets a bool because exiting must happen in the main loop for seg fault at exit
void Terminate(void* data) {
    bExit = true;
}

// does a complete draw of the lsystem, also scales down
void CompleteDrawHandle(void* data) {
    if(!vertices.empty()){
        ResetDrawing(NULL);
    }
    bCompIter = true;
    bScale = true;
}

// advances the drawing turtle system by one iteration
void NextIterHandle(void* data) {
    bOneIter = true;
}

// starts the animated drawing process
void AnimateDraw(void* data) {
    if(!vertices.empty()){
        ResetDrawing(NULL);
    }
    bDraw = true;
}

// up one iteration and do a complete draw
void GenUp(void* data) {
    if(iCurrentGen > Generation) { Lsys->next(); }
    iCurrentGen++;
    CompleteDrawHandle(NULL);
}

// down one iteration and do a complete draw
void GenDown(void* data) {
    iCurrentGen--;
    if(iCurrentGen < 0) { iCurrentGen = 0; } //no funny business
    CompleteDrawHandle(NULL);
}

// Function called to copy the content of a std::string (souceString) handled 
// by the AntTweakBar library to destinationClientString handled by our application
void TW_CALL CopyStdStringToClient(std::string& destination, const std::string& source) {
    destination = source;
}
