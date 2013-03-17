#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Item.h"
#include "BoundBox.h"
#include "RayRecord.h"

class BVH : public Item
{
public:
    
    BoundBox bb ;

	Item* left;
	Item* right;

    BVH(void);
    BVH(std::vector<Item*> objs, GLuint axis) ;
    virtual bool intersectionTest (RayRecord &rr, GLfloat boundT) ;
    virtual BoundBox makeBox () ;

    BoundBox combine(BoundBox b1, BoundBox b2) ;
    bool hitBox(RayRecord &rr) ;
    ~BVH(void);
};

