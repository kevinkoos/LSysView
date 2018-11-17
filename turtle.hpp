#ifndef TURTLE_H
#define TURTLE_H

#include "./glm/vec3.hpp"
#include "./glm/mat3x3.hpp"
#include "./glm/gtc/matrix_transform.hpp"
#include "./glm/gtc/matrix_inverse.hpp"

class Turtle {
    private:
    glm::vec3 position;  //position of the turtle x,y,z
    glm::mat3 coord_sys; //coordinate system of turtle, vectors are forward, left, up
    
    void update_sys(int ax, float angle);
    
    public:
    
    Turtle();
    ~Turtle();
    Turtle(Turtle *turt);
    void forward();
    glm::vec3 get_pos();
    void pitch(float angle);
    void yaw(float angle);
    void roll(float angle);
};

#endif
