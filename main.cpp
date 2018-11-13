/* Name: main.cpp
 * Author: Kevin Koos
 * Description: main file for testing parts of l-system viever applcation
 */
#include "main.hpp"
#include <cmath>

void print(glm::vec3 vec) {
    std::cout << "x: " << round(vec.x) << "  " << "y: " << round(vec.y) << "  " << "z: ";
    std::cout << round(vec.z) << std::endl;
}

int main(int argc, char** argv) {
    Turtle turtle = Turtle();
    print(turtle.get_pos());

    turtle.forward();
    print(turtle.get_pos());

    turtle.yaw(90);
    turtle.forward();
    print(turtle.get_pos());

    turtle.yaw(180);
    turtle.forward();
    print(turtle.get_pos());

    turtle.roll(90);
    turtle.yaw(90);
    turtle.forward();
    print(turtle.get_pos());
    
    
    std::string axiom = "A";
    std::vector<std::string> rules{"AB","B"};
    std::string vars = "AB";
    Lsystem Lsys = Lsystem(axiom, rules, vars);
    std::cout << "lsystem" << std::endl;
    
    std::cout << Lsys.get() << std::endl;
    
    Lsys.next();
    std::cout << Lsys.get() << std::endl;
    
    Lsys.next();
    std::cout << Lsys.get() << std::endl;
    
    Lsys.next();
    std::cout << Lsys.get() << std::endl;
    
    
    
    return 0;
}
