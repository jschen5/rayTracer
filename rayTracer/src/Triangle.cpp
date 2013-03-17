#include <iostream>
#include "Triangle.h"

void printMatrix (const glm::mat4 puttr) {
	std::cout << puttr[0][0] << " " << puttr[0][1] << " " << puttr[0][2] << " " << puttr[0][3] << std::endl ;
                  std::cout << puttr[1][0] << " " << puttr[1][1] << " " << puttr[1][2] << " " << puttr[1][3] << std::endl ;
                  std::cout << puttr[2][0] << " " << puttr[2][1] << " " << puttr[2][2] << " " << puttr[2][3] << std::endl ;
                  std::cout << puttr[3][0] << " " << puttr[3][1] << " " << puttr[3][2] << " " << puttr[3][3] << std::endl ;
}

void printVec3 (const glm::vec3 puttr) {
    std::cout << puttr[0] << ", " << puttr[1] << ", " << puttr[2]  << std::endl ;
}


Triangle::Triangle(void)
{
}

Triangle::Triangle(glm::vec3 norm, glm::mat4 MV, Material &m, std::vector<glm::vec3> verts, std::vector<glm::vec3> Tverts):
//_indices(inds),
_normal(norm),
_vertices(verts),
_Tvertices(Tverts)
{
    _material = &m ;
    _modelTransform = MV ;
	box = this->makeBox();
}


glm::vec3 Triangle::cartesianToBarycentric (glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p, glm::vec3 n) {

    GLfloat lambda1 = glm::dot(glm::cross(p3 - p2, p - p2), n) / (glm::dot(glm::cross(p2 - p1, p3 - p1), n)) ;
    GLfloat lambda2 = glm::dot(glm::cross(p1 - p3, p - p3), n) / (glm::dot(glm::cross(p2 - p1, p3 - p1), n)) ;
    GLfloat lambda3 = glm::dot(glm::cross(p2 - p1, p - p1), n) / (glm::dot(glm::cross(p2 - p1, p3 - p1), n)) ;
    return glm::vec3(lambda1, lambda2, lambda3) ;

}


bool Triangle::intersectionTest(RayRecord &rr, GLfloat boundT) {
    
    bool retBool = false ;

    //glm::vec3 tri = _indices ;
    glm::vec3 a = _Tvertices[0] ;
    glm::vec3 b = _Tvertices[1] ;
    glm::vec3 c = _Tvertices[2] ;

    //printVec3(a) ;
    //printVec3(b) ;
    //printVec3(c) ;
    
    GLfloat denom = glm::dot(rr.getP1(), _normal) ;
    if (denom < 0 || rr._isRefraction) {
        GLfloat newT = glm::dot((a - rr.getP0()), _normal) / denom ;
        if (newT < rr.getT() && newT > 0 && newT < boundT) {

            glm::vec3 intersect = rr.getP0() + rr.getP1()*newT ;

            glm::vec3 bary = Triangle::cartesianToBarycentric(a, b, c, intersect, _normal) ;
            if ((bary.x >= 0 && bary.x <= 1) && (bary.y >= 0 && bary.y <= 1) && (bary.z >= 0 && bary.z <= 1)) {
                rr.updateT(newT) ;
                rr.updateMaterial(*_material) ;
                rr.updateNorm(_normal) ;
                rr.updateIntersectedObj(this) ;
                retBool = true ;
            }
        }
    }
    return retBool ;
}

BoundBox Triangle::makeBox () {
    glm::vec3 v1 = this->_vertices[0] ;
    glm::vec3 v2 = this->_vertices[1] ;
    glm::vec3 v3 = this->_vertices[2] ;

    //old max, min, and vector
    glm::vec3 oldMax = glm::vec3(glm::max(glm::max(v1.x, v2.x), v3.x), glm::max(glm::max(v1.y, v2.y), v3.y), glm::max(glm::max(v1.z, v2.z), v3.z)) ;
    glm::vec3 oldMin =  glm::vec3(glm::min(glm::min(v1.x, v2.x), v3.x), glm::min(glm::min(v1.y, v2.y), v3.y), glm::min(glm::min(v1.z, v2.z), v3.z)) ;
    glm::vec3 oldVect = oldMax - oldMin ;

    glm::vec4 _oldMin = glm::vec4(oldMin, 1) ;
    glm::vec4 _oldVectX = glm::vec4(oldVect.x, 0, 0, 0) ;
    glm::vec4 _oldVectY = glm::vec4(0, oldVect.y, 0, 0) ;
    glm::vec4 _oldVectZ = glm::vec4(0, 0, oldVect.z, 0) ;
    
    glm::vec4 _trMin = glm::transpose(this->_modelTransform) * _oldMin ;
    glm::vec4 _trVectX = glm::transpose(this->_modelTransform) * _oldVectX ;
    glm::vec4 _trVectY = glm::transpose(this->_modelTransform) * _oldVectY ;
    glm::vec4 _trVectZ = glm::transpose(this->_modelTransform) * _oldVectZ ;

    //now get all 8 new points of the box
    glm::vec4 _p0 = _trMin ;
    glm::vec4 _p1 = _trMin + _trVectX ;
    glm::vec4 _p2 = _trMin + _trVectX + _trVectY ;
    glm::vec4 _p3 = _trMin + _trVectX + _trVectZ ;
    glm::vec4 _p4 = _trMin + _trVectX + _trVectY + _trVectZ ;
    glm::vec4 _p5 = _trMin + _trVectY ;
    glm::vec4 _p6 = _trMin + _trVectY + _trVectZ ;
    glm::vec4 _p7 = _trMin + _trVectZ ;
    
    //std::cout << "TriBBoxPts: " << std::endl ;
    glm::vec3 p0 = glm::vec3(_p0.x, _p0.y, _p0.z) ;
    //printVec3(p0) ;
    glm::vec3 p1 = glm::vec3(_p1.x, _p1.y, _p1.z) ;
    //printVec3(p1) ;
    glm::vec3 p2 = glm::vec3(_p2.x, _p2.y, _p2.z) ;
    //printVec3(p2) ;
    glm::vec3 p3 = glm::vec3(_p3.x, _p3.y, _p3.z) ;
    //printVec3(p3) ;
    glm::vec3 p4 = glm::vec3(_p4.x, _p4.y, _p4.z) ;
    //printVec3(p4) ;
    glm::vec3 p5 = glm::vec3(_p5.x, _p5.y, _p5.z) ;
    //printVec3(p5) ;
    glm::vec3 p6 = glm::vec3(_p6.x, _p6.y, _p6.z) ;
    //printVec3(p6) ;
    glm::vec3 p7 = glm::vec3(_p7.x, _p7.y, _p7.z) ;
    //printVec3(p7) ;

    //get new max and min
    GLfloat newMaxX, newMaxY, newMaxZ, newMinX, newMinY, newMinZ ;
    std::vector<glm::vec3> plist ;
    plist.push_back(p0) ; plist.push_back(p1) ; plist.push_back(p2) ; plist.push_back(p3) ;
    plist.push_back(p4) ; plist.push_back(p5) ; plist.push_back(p6) ; plist.push_back(p7) ;
    for (int i = 0; i < plist.size(); i++) {
        GLfloat nextX = plist[i].x ;
        GLfloat nextY = plist[i].y ;
        GLfloat nextZ = plist[i].z ;
        if (i == 0) {
            newMaxX = newMinX = nextX ;
            newMaxY = newMinY = nextY ;
            newMaxZ = newMinZ = nextZ ;
        } else {
            //new max
            newMaxX = glm::max(nextX, newMaxX) ;
            newMaxY = glm::max(nextY, newMaxY) ;
            newMaxZ = glm::max(nextZ, newMaxZ) ;
            //new min
            newMinX = glm::min(nextX, newMinX) ;
            newMinY = glm::min(nextY, newMinY) ;
            newMinZ = glm::min(nextZ, newMinZ) ;
        }
    }
    //set new max and min
    glm::vec3 newMax (newMaxX, newMaxY, newMaxZ) ;
    glm::vec3 newMin (newMinX, newMinY, newMinZ) ;
    //get new vector
    glm::vec3 newVect = newMax - newMin ;
    //glm::vec3 newVect = glm::vec3(newMax.x - newMin.x, newMax.y - newMin.y, newMax.z - newMin.z) ;
    //make new boundbox
    BoundBox b (newMin, newVect) ;
	return b;
}

Triangle::~Triangle(void)
{
}
