/* Name: lsystem.cpp
 * Author: Kevin Koos
 * Description: class definition of an lindenmeyer system
 */
#include <string>
#include <vector>

/* alphabet
 * F - draw forward 1
 * f - move forward 1
 * [ - push current state
 * ] - pop current state
 * 
 *
 */


class Lsystem {
  private:
    std::vector<std::string> system;    //list of strings, first is axiom
    std::vector<std::string> rules;     //defines way chars are replaced
    std::vector<std::string> variables; //chars that can be replaced in a rule, 1-1 with rules
    std::string axiom;                  //inital string of characters
    int generation;                     //which generation is it
    int n_rules;                        //number of rules total

    //basic constructor and destructor
    Lsystem(){}
    ~Lsystem(){}
  public:
    
    //Name: generate
    //Decriptions: generates the next iteration of the lysystem.
    void generate(){
    
    }
    
    //Name: strip
    //Description: strips the production string for gen n and a pointer to the striped string on the heap
    std::string* strip(int n) {
        
    }

};
