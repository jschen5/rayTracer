#include <stdio.h>
#include <iostream>
#include "PointLight.h"

#define INF std::numeric_limits<float>::infinity()
#define nINF -1 * std::numeric_limits<float>::infinity()


PointLight::PointLight(void)
{
}

PointLight::PointLight(glm::vec3 lpos, glm::vec3 lcol, GLfloat area):
_area(area)
{
    _position = lpos ;
    _width = _height = sqrt(area) ;
    _color = lcol ;
}

bool PointLight::isPointLight() {
    return true ;
}

RayRecord * PointLight::getRayToLight(glm::vec3 p, glm::vec2 offset) {

    Material mt ;

    if (_area != 0) {
        
        glm::vec3 w = glm::normalize(p - _position) ;
        glm::vec3 tempUp ;

        if (w.x == 0 && w.z == 0) { //light is directly above p
            tempUp = glm::vec3(0, 0, 1) ;
        } else {
            tempUp = glm::vec3(0, 1, 0) ;
        }

        glm::vec3 u = glm::normalize(glm::cross(tempUp, w)) ;
        glm::vec3 v = glm::normalize(glm::cross(w, u)) ;

        //GLfloat lightX = _width * (offset[0] - (numSamples / (float) 2.0f)) / (numSamples / (float) 2.0f) ;
        //GLfloat lightY = _height * (offset[1] - (numSamples / (float) 2.0f)) / (numSamples / (float) 2.0f) ;
        GLfloat lightX = _width * (offset[0] - 0.5) ; 
        GLfloat lightY = _height * (offset[1] - 0.5) ;
        
        glm::vec3 lightP1 = lightX * u + lightY * v - glm::length(_position - p) * w ;

        return new RayRecord(INF, mt, p + 0.0005f * lightP1, lightP1) ;

    } else {

        return new RayRecord(INF, mt, p + 0.0005f * (_position - p), _position - p) ;

    }


}

PointLight::~PointLight(void)
{
}
