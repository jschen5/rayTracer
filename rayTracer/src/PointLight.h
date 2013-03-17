#pragma once
#include "Light.h"
class PointLight :
    public Light
{
public:

    GLfloat _area ;
    GLfloat _width , _height ;

    PointLight(void);
    PointLight(glm::vec3 lpos, glm::vec3 lcol, GLfloat area) ;

    virtual bool isPointLight() ;
    virtual RayRecord * getRayToLight(glm::vec3 p, glm::vec2 offset)  ;

    ~PointLight(void);
};

