#pragma once

#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h"

#include "Camera.h"
#include "RayRecord.h"
#include "Material.h"
#include "Triangle.h"
#include "Sphere.h"
#include "bvh.h"
#include "PointLight.h"

class RayTracer
{
public:

    GLuint maxdepth ;

    std::vector<Light *> lights ; //Light Positions
    std::vector<glm::vec3> lcolors ; //Light colors
    std::vector<Item *> objects ;
    std::vector<glm::vec3> vertices ;
    BVH bvh ;
    Camera cam ;

    bool useBVH ;
    GLuint numSamples ;

    // attenuations
    bool useAttenuation ;
    GLfloat constAtten ;
    GLfloat linearAtten ;
    GLfloat quadrAtten ;
    //GLuint numused ; // How many lights are used 



    RayTracer(void);
    bool intersections(RayRecord &rec) ;
    bool bvhIntersections(RayRecord &rec) ;
    glm::vec3 getColor(RayRecord &rec, glm::vec2 offset, GLuint numSamples, GLuint depth) ;
    void createBVH() ;
    ~RayTracer(void);
};

