#pragma once
#include "Item.h"
class Triangle :
    public Item
{
public:

    std::vector<glm::vec3> _vertices ;
    std::vector<glm::vec3> _Tvertices ;
    glm::vec3 _normal ;

    //glm::vec3 smoothNorm ;

    Triangle(void);
    Triangle(glm::vec3 norm,  glm::mat4 MV, Material &m, std::vector<glm::vec3> verts, std::vector<glm::vec3> Tverts) ;
    virtual bool intersectionTest (RayRecord &rr, GLfloat boundT) ;
    static glm::vec3 cartesianToBarycentric (glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p, glm::vec3 n) ;
    virtual BoundBox makeBox () ;
    ~Triangle(void);
};

