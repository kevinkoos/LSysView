#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <string>
#include <vector>

class Lsystem {
private:
    std::vector<std::string> system;    //list of strings, first is axiom
    std::vector<std::string> rules;     //defines way chars are replaced
    std::string variables; //chars that can be replaced in a rule, 1-1 with rules
    std::string axiom;                  //inital string of characters
    int generation;                     //which generation is it
    int n_rules;  
    
public:
    Lsystem();
    Lsystem(std::string axiom, std::vector<std::string> rules, std::string variables);
    ~Lsystem();
    std::string get();
    std::string get(int n);
    void next();
};

#endif
