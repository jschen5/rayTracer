#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <vector>
//#include <GL/glew.h>
//#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class BoundBox
{
public:

    glm::vec3 boxMin ;
    glm::vec3 dVector ;

    BoundBox(void);
    BoundBox(glm::vec3 min, glm::vec3 dv) ;
    ~BoundBox(void);
};

