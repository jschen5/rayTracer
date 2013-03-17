#include <iostream>
#include "Sphere.h"

void printMat2 (const glm::mat4 puttr) {
	std::cout << puttr[0][0] << " " << puttr[0][1] << " " << puttr[0][2] << " " << puttr[0][3] << std::endl ;
                  std::cout << puttr[1][0] << " " << puttr[1][1] << " " << puttr[1][2] << " " << puttr[1][3] << std::endl ;
                  std::cout << puttr[2][0] << " " << puttr[2][1] << " " << puttr[2][2] << " " << puttr[2][3] << std::endl ;
                  std::cout << puttr[3][0] << " " << puttr[3][1] << " " << puttr[3][2] << " " << puttr[3][3] << std::endl ;
}

void printVec3sph (const glm::vec3 puttr) {
    std::cout << puttr[0] << ", " << puttr[1] << ", " << puttr[2]  << std::endl ;
}


Sphere::Sphere(void)
{
}


Sphere::Sphere(glm::vec3 c, GLfloat r, Material &mt, glm::mat4 MV):
_center(c),
_radius(r)
{
    _material = &mt ;
    _modelTransform = MV ;
    box = this->makeBox() ;
}


bool Sphere::intersectionTest(RayRecord &rr, GLfloat boundT) {
    
    glm::mat4 invTr = glm::inverse(_modelTransform) ;

    glm::vec4 tempP0 (rr.getP0().x, rr.getP0().y, rr.getP0().z, 1.0) ;
    tempP0 = glm::transpose(invTr) * tempP0 ;
    glm::vec3 p0 = glm::vec3(tempP0.x, tempP0.y, tempP0.z) / (GLfloat) tempP0.w ;

    glm::vec4 tempP1 (rr.getP1().x, rr.getP1().y, rr.getP1().z, 0.0) ;
    tempP1 = glm::transpose(invTr) * tempP1 ;
    glm::vec3 p1 = glm::vec3(tempP1.x, tempP1.y, tempP1.z) ;
    

    GLfloat a = glm::dot(p1, p1) ;
    GLfloat b = 2 * glm::dot(p1, (p0 - _center)) ;
    GLfloat c = glm::dot((p0 - _center), (p0 - _center)) - _radius * _radius ;

    GLfloat discriminant = b*b - 4*a*c ;

    if (discriminant < 0) {
        return false ;
    } else {

        GLfloat sqrtTerm = glm::sqrt(discriminant) ;
        GLfloat t1 = (-b + sqrtTerm) / (2 * a) ;
        GLfloat t2 = (-b - sqrtTerm) / (2 * a) ;

        GLfloat oldT = rr.getT() ;
        GLfloat newT ;
        if (t1 > 0 && t2 > 0) { // both positive roots
            newT = glm::min(t1, t2) ;
            //rr.updateT(glm::min(oldT, newT)) ;
            if (newT < oldT && newT < boundT) {
                rr.updateT(newT) ;
                glm::vec4 normTemp (p0 + p1*newT - _center, 0.0f) ;
                normTemp = invTr * normTemp ;
                rr.updateNorm(glm::normalize(glm::vec3(normTemp.x, normTemp.y, normTemp.z))) ;
                rr.updateMaterial(*_material) ;
                rr.updateIntersectedObj(this) ;
                return true ;
            } else {
                return false ;
            }
        } else if (t1 * t2 < 0) {   // 1 negative and 1 positive root
            newT = glm::max(t1, t2) ;   //pick the positive root
            //rr.updateT(glm::min(oldT, newT)) ;
            if (newT < oldT && newT < boundT) {
                rr.updateT(newT) ;
                rr.updateColor(_material->_ambient) ;
                glm::vec4 normTemp (p0 + p1*newT - _center, 0.0f) ;
                normTemp = invTr * normTemp ;
                rr.updateNorm(glm::normalize(glm::vec3(normTemp.x, normTemp.y, normTemp.z))) ;
                rr.updateMaterial(*_material) ;
                rr.updateIntersectedObj(this) ;
                return true ;
            } else {
                return false ;
            }
        } else {    // both roots same, tangent to sphere, no intersect
            return false ;
        }
    }
}

BoundBox Sphere::makeBox () {
    glm::vec3 oldMax = glm::vec3(this->_center.x + this->_radius, this->_center.y + this->_radius, this->_center.z + this->_radius) ;
    glm::vec3 oldMin = glm::vec3(this->_center.x - this->_radius, this->_center.y - this->_radius, this->_center.z - this->_radius) ;
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
    
    glm::vec3 p0 = glm::vec3(_p0.x, _p0.y, _p0.z) ;
    glm::vec3 p1 = glm::vec3(_p1.x, _p1.y, _p1.z) ;
    glm::vec3 p2 = glm::vec3(_p2.x, _p2.y, _p2.z) ;
    glm::vec3 p3 = glm::vec3(_p3.x, _p3.y, _p3.z) ;
    glm::vec3 p4 = glm::vec3(_p4.x, _p4.y, _p4.z) ;
    glm::vec3 p5 = glm::vec3(_p5.x, _p5.y, _p5.z) ;
    glm::vec3 p6 = glm::vec3(_p6.x, _p6.y, _p6.z) ;
    glm::vec3 p7 = glm::vec3(_p7.x, _p7.y, _p7.z) ;

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
    //make new boundbox
    BoundBox b (newMin, newVect) ;
	return b;
}

Sphere::~Sphere(void)
{
}
