#pragma once
#include "Item.h"

class Sphere :
    public Item
{
public:

    glm::vec3 _center ;
    GLfloat _radius ;

    Sphere(void);
    Sphere(glm::vec3 c, GLfloat r, Material &mt, glm::mat4 MV) ;
    virtual bool intersectionTest (RayRecord &rr, GLfloat boundT) ;
    virtual BoundBox makeBox () ;
    ~Sphere(void);
};

