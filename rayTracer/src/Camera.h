#pragma once
#include "RayRecord.h"
#include <GL/glew.h>
#include <GL/glut.h>

class Camera
{
public:

    glm::vec3 _eye ;
    glm::vec3 _center ;
    glm::vec3 _up ;
    GLfloat _fovy ;

    glm::vec3 _u, _v, _w ;
    GLfloat _d ;    //focal length
    GLfloat _width, _height ;
    GLfloat _aspect ;
    GLfloat _lens_size ;

    GLfloat _screenX, _screenY ;

    Camera(void);
    Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up, GLfloat fovy, GLfloat d, GLfloat lensSize, GLfloat width, GLfloat height) ;
    RayRecord generateRay(GLuint i, GLuint j, glm::vec2 eyeJitter, glm::vec2 viewJitter) ;
    ~Camera(void);
};

