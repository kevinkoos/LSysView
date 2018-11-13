/* Name: lsystem.cpp
 * Author: Kevin Koos
 * Description: class definition of an lindenmeyer system
 */
#include "lsystem.hpp"

/* alphabet
 * == core ==
 * there are two forward chars for edge rewriting
 * F,D - draw forward 1
 * f,d - move forward 1
 * [ - push current state
 * ] - pop current state
 * + - add yaw
 * - - sub yaw
 * ^ - add pitch
 * v - sub pitch
 * > - add roll
 * < - sub roll
 *
 * == additional ==
 * {...} - draw a polygon
 * 
 * anything else not listed is considered a placholder variable which are ignored
 */

//basic constructor and destructor
Lsystem::Lsystem(){}
Lsystem::Lsystem(std::string axiom, std::vector<std::string> rules, std::string variables) {
    this->axiom = axiom;
    this->variables = variables;
    this->rules = rules;
    n_rules = rules.size();
    generation = 0;
    system.push_back(axiom);
}
Lsystem::~Lsystem(){}

//gets the iteration specified by int n, will generate iterations to match
std::string Lsystem::get(){ return system[generation]; }

std::string Lsystem::get(int n){
    if(n > generation) {
        //generate iteration till generation == n
    }
    return system[n];
}

//advance the l-system by one iteration
void Lsystem::next() {
    std::size_t pos_prev = 0;
    std::size_t pos = system[generation].find_first_of(variables);
    system.push_back(std::string());
    while(pos != std::string::npos) {
        system.back().append( system[generation].substr(pos_prev,pos-1) );
        system.back().append( rules[variables.find(system[generation][pos])] );
        pos_prev = pos;
        pos = system[generation].find_first_of(variables, pos+1);
    }
    system.back().append( system[generation].substr(pos_prev) );    
    generation++;
}

    












