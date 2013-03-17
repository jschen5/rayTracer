#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"
#include "RayRecord.h"
#include "BoundBox.h"


class Item
{
public:

    glm::mat4 _modelTransform ;
    Material * _material ;
    BoundBox box ;


    virtual bool intersectionTest (RayRecord &rr, GLfloat boundT) = 0 ; 
    virtual BoundBox makeBox () = 0 ;
    void setModelTransform (glm::mat4 MT) ;

    virtual ~Item(void);
};

