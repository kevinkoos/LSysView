/* Name: turtle.cp
 * Date: 11-18
 * Description: turtle class definition for cs 450 final project l-system viewer
 */
#include "turtle.hpp"

// updates the coordinate system matrix on some axis and angle
void Turtle::update_sys(int ax, float angle){
        glm::vec3 axis;  
        if(ax == 1) { axis = glm::vec3(0.f,1.f,0.f); }        //pitch
        else if(ax == 2) { axis = glm::vec3(0.f,0.f,1.f); }   //yaw
        else if(ax == 3){ axis = glm::vec3(1.f,0.f,0.f); }    //roll
        coord_sys *= glm::mat3(glm::rotate(glm::mat4(), glm::radians(angle), axis));
}

//basic constructor
Turtle::Turtle() {
        position = glm::vec3(0.f);
        coord_sys = glm::mat3(1.f);    //default coord sys is i,j,k
}
    
Turtle::~Turtle(){}
    
//copy constructor
Turtle::Turtle(const Turtle &turt) {
    position = turt.position;
    coord_sys = turt.coord_sys;
}

//sends the turtle forward in its heading
void Turtle::forward(){ position += glm::normalize(coord_sys[0]); }
    
//get the position of the turtle
glm::vec3 Turtle::get_pos() { return position; }

void Turtle::pitch(float angle){ update_sys(1, angle); }
void Turtle::yaw(float angle){ update_sys(2, angle); }
void Turtle::roll(float angle){ update_sys(3, angle); }

