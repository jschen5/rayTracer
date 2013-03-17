#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"

class Item;

class RayRecord
{
private:

    GLfloat _t ;
    glm::vec3 _color ;
    glm::vec3 _norm ;
    glm::vec3 _p0, _p1 ;
    Material * _material ;
    Item * _intersectedObj ;
    GLfloat _prevN ;

public:

    bool _isRefraction ;

    RayRecord(void);
    RayRecord(GLfloat t, Material &m, glm::vec3 p0, glm::vec3 p1) ;
    void updateT(GLfloat t) ;
    void updateColor(glm::vec3 col) ;
    void updateNorm(glm::vec3 n) ;
    void updateMaterial(Material &m) ;
    void updateIntersectedObj(Item * m) ;
    void updateN(GLfloat N) ;

    GLfloat getT() ;
    glm::vec3 getColor() ;
    glm::vec3 getNorm() ;
    Material * getMaterial() ;
    Item * getIntersectedObj() ;
    glm::vec3 getP0() ;
    glm::vec3 getP1() ;
    GLfloat getPrevN() ;

    ~RayRecord(void);
};

