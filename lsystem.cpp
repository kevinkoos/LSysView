/* Name: lsystem.cpp
 * Author: Kevin Koos
 * Description: class definition of an lindenmeyer system
 */
#include "lsystem.hpp"

/* alphabet
 * == core ==
 * there are two forward chars for allowing edge rewriting systems
 * F,D - forward 1
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
 * anything else not listed is considered a placholder variable which are ignored by the turtle
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

//shortcut functions
std::string Lsystem::get(){ return system[generation]; }
std::string Lsystem::get_next(){ this->next(); return this->get(); }


//gets the iteration specified by int n, will generate iterations to match
std::string Lsystem::get(int n){
    if(n > generation) {
        for(int i = generation; i < n; i++) {
            this->next();
        }
    }
    return system[n];
}

//advance the l-system by one iteration
void Lsystem::next() {
    char c;
    std::size_t pos;
    system.push_back(system.back());
    generation++;
    //loop backwards replacing as we go
    for(int i = system.back().length()-1; i >= 0; i--) {
        c = system.back()[i];
        pos = variables.find(c);
        if(pos != std::string::npos) {
            system.back().replace(i, 1, rules[pos]);
        }
    }
}

//get the latest generation number
int Lsystem::get_gen() {
    return generation;
}

    












