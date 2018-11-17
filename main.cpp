/* Name: main.cpp
 * Author: Kevin Koos
 * Description: main file for testing parts of l-system viever applcation
 */
#include <vector>
#include <list>
#include <cmath>
#include <iostream>
#include "turtle.hpp"
#include "lsystem.hpp"

//TURTLE COMMANDS AND THEIR CHARACTERS
enum CHARS {
        DRAW =          'D',
        FORWARD =       'F',
        ROLL_UP =       '>',
        ROLL_DOWN =     '<',
        PITCH_UP =      '^',
        PITCH_DOWN =    'v',
        YAW_UP =        '+',
        YAW_DOWN =      '-',
        POLY =          '{',
        BRANCH =        '[',
        END =           '\0',
};

void print(glm::vec3 vec) {
    std::cout << "x: " << round(vec.x) << "  " << "y: " << round(vec.y) << "  " << "z: ";
    std::cout << round(vec.z) << std::endl;
}

void new_turtle(std::vector<std::string> sys, std::vector<Turtle> turts) {
    //use a reg expression to take current turtle string, grab [...] characters, make new turtle, with
    //the right character string and remove old from original turtle.
    //there is a copy constructor for doing this easily
    
}


int main(int argc, char** argv) {
    
    std::string axiom = "A";
    std::vector<std::string> rules{"-BF+AFA+FB-","+AF-BFB-FA+"};
    std::string vars = "AB";
    Lsystem Lsys = Lsystem(axiom, rules, vars);
    std::cout << "lsystem" << std::endl;
    
    std::cout << Lsys.get() << std::endl;
    
    Lsys.next();
    std::cout << Lsys.get() << std::endl;
    
    Lsys.next();
    std::cout << Lsys.get() << std::endl;
    
    /* ========== testing map and char reading loop ============= */
    
    float angle = 90;
    bool loop = false;
    std::vector<std::string> systems;
    std::vector<Turtle> turt_sys;
    std::vector<glm::vec3> current_turtles;
    std::vector<glm::vec3> vertices;
    char c;
    int i = 0;
    int j = 0;
    int op_br = 0;
    
    systems.push_back(Lsys.get());
    turt_sys.push_back(new Turtle());

    do {
        current_turtles.clear();
        while(i < turt_sys.size()) {
            
            c = systems[i][0];
            systems[i].erase(0,1);
            
            switch(c) {
                case DRAW:
                case FORWARD:
                    turt_sys[i].forward();
                    vertices.push_back(turt_sys[i].get_pos());
                    current_turtles.push_back(turt_sys[i].get_pos());
                    i++;                                //next turtle
                    break;
            
                case YAW_UP:
                    turt_sys[i].yaw(angle);
                    break;
                    
                case YAW_DOWN:
                    turt_sys[i].yaw(-angle);
                    break;
                    
                case PITCH_UP:
                    turt_sys[i].pitch(angle);
                    break;
                    
                case PITCH_DOWN:
                    turt_sys[i].pitch(-angle);
                    break;
                    
                case ROLL_UP:
                    turt_sys[i].roll(angle);
                    break;
                    
                case ROLL_DOWN:
                    turt_sys[i].roll(-angle);
                    break;
                    
                case BRANCH:                            //add a branching path to tree, new turtle
                    op_br = 1;
                    systems.push_back(std::string());
                    turt_sys.push_back(new Turtle(turt_sys[i]));
                    while(op_br != 0) {                 //count '[' / ']' until 0 
                        c = systems[i][0];
                        systems[i].erase(0,1);          //chars from original string removed
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
                    
                    break;
                    
                case END:                               //turtle is at end of string
                    
                    break;
                    
                default:                                //all other chars are ignored by turtle
                    
                    break;
                
                
            }
            
            
        }
        
    } while(loop);
    
    for(int t = 0; t < vertices.size(); t++) {
        print(vertices[i]);
        
    }
    
    
    
    

    return 0;
}
