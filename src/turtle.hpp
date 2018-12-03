#ifndef TURTLE_H
#define TURTLE_H

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

//TURTLE COMMANDS AND THEIR CHARACTERS
enum CHARS {
        DRAW =          'D',
        FORWARD =       'F',
        SDRAW =         'd',
        SFORWARD =      'f',
        REVERSE =       '!',
        ROLL_UP =       '>',
        ROLL_DOWN =     '<',
        PITCH_UP =      '^',
        PITCH_DOWN =    'v',
        YAW_UP =        '+',
        YAW_DOWN =      '-',
        BRANCH =        '[',
        END =           '\0',
};

class Turtle {
    private:
    glm::vec3 position;  //position of the turtle x,y,z
    glm::mat3 coord_sys; //coordinate system of turtle, vectors are forward, left, up
    
    
    public:
    
    Turtle();
    ~Turtle();
    Turtle(Turtle *turt);
    void update_sys(int ax, float angle);
    void forward();
    glm::vec3 get_pos();
    void pitch(float angle);
    void yaw(float angle);
    void roll(float angle);
};

#endif
