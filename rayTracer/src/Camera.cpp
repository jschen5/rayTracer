#include "Camera.h"
#include <stdio.h>
#include <iostream>

#define INF std::numeric_limits<float>::infinity()
#define nINF -1 * std::numeric_limits<float>::infinity()


Camera::Camera(void)
{
}


Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up, GLfloat fovy, GLfloat d, GLfloat lensSize, GLfloat width, GLfloat height):
_eye(eye),
_center(center),
_up(up),
_fovy(fovy),
_width(width),
_height(height),
_lens_size(lensSize)
{
    _w = glm::normalize(_eye - _center) ;
	_u = glm::normalize(glm::cross(_up, _w)) ;
	_v = glm::normalize(glm::cross(_w, _u)) ;
    _d = d ;
    _aspect = width / (float) height ;

    _screenY = _d * glm::tan(glm::radians(_fovy / (float) 2.0f)) ;
    _screenX = _aspect * _screenY ;

}


RayRecord Camera::generateRay(GLuint i, GLuint j, glm::vec2 eyeJitter, glm::vec2 viewJitter) {
    GLfloat pixX = _screenX * (i + viewJitter[0] - (_width / (float) 2.0f)) / (_width / (float) 2.0f) ;
    GLfloat pixY = _screenY * (j + viewJitter[1] - (_height / (float) 2.0f)) / (_height / (float) 2.0f) ;
            
    glm::vec3 p0 = _eye ;
    glm::vec3 p1 = pixX * _u + pixY * _v + -_d * _w ;

    glm::vec3 focPos = p0 + p1 ;
    
    glm::vec3 newEye = p0 + ((eyeJitter[0] - 0.5f) * _u + (eyeJitter[1] - 0.5f) * _v) * _lens_size ;
    
    glm::vec3 newP1 = focPos - newEye ;
    
    Material mt ;

    return RayRecord(INF, mt, newEye, newP1) ;
}


Camera::~Camera(void)
{
}
