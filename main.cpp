/* Name: main.cpp
 * Author: Kevin Koos
 * Description: main file for testing parts of l-system viever applcation
 */
#include <vector>
#include <list>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <cstdio>
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
    //std::cout << "x: " << vec.x << "  " << "y: " << vec.y << "  " << "z: "  << vec.z << std::endl;
    printf("x: %5.2f  y: %5.2f  z: %5.2f\n", vec.x, vec.y, vec.z);
    
}

int main(int argc, char** argv) {
    
//     float angle = 90;
//     std::string axiom = "A";
//     std::vector<std::string> rules{"-BF+AFA+FB-","+AF-BFB-FA+"};
//     std::string vars = "AB";
    float angle = 45;
    std::string axiom = "D";
    std::vector<std::string> rules{"F[+D]-D","FF"};
    std::string vars = "DF";
    
    Lsystem Lsys = Lsystem(axiom, rules, vars);
        
    Lsys.next();    //1st iter
    Lsys.next();    //2nd
    Lsys.next();    //3rd
    
    /* ========== testing map and char reading loop ============= */
    
    std::cout << "Starting string: " << axiom << std::endl;
    std::cout << "Variables: " << vars << std::endl;
    std::cout << "Rules: "; 
    for(int i=0;i<rules.size();i++){ std::cout << std::setw(4) << rules[i]; }
    std::cout << std::endl;
    

    
    bool loop = true;
    std::list<std::string> systems;
    std::list<Turtle>      turt_sys;
    std::vector<glm::vec3>   current_turtles;
    std::vector<glm::vec3>   vertices;
    std::vector<glm::vec3>   prev_vertices;
    char c;
    int op_br;
    
    systems.push_back(Lsys.get());
    turt_sys.push_back(Turtle());
    vertices.push_back(glm::vec3(0., 0., 0.));
    prev_vertices.push_back(glm::vec3(0., 0., 0.));

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
    } while(loop && !turt_sys.empty());                 //if we want to loop, go until we're done
    
    //print out the list of vertices
    for(int i = 0; i < vertices.size(); i++) {
        print(vertices[i]);
        //printf("\t");
        //print(prev_vertices[i]);
    }
    
    
    
    

    return 0;
}
