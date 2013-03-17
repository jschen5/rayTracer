#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RayRecord.h"

class Light
{
public:

    glm::vec3 _position ;
    glm::vec3 _color ;

    virtual bool isPointLight() = 0 ;
    virtual RayRecord * getRayToLight(glm::vec3 p, glm::vec2 offset) = 0 ;
    virtual ~Light(void) ;
};

