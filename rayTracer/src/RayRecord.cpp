#include "RayRecord.h"

RayRecord::RayRecord(void)
{
}

RayRecord::RayRecord(GLfloat t, Material &m, glm::vec3 p0, glm::vec3 p1):
_t(t),
_material(&m),
_p0(p0), _p1(p1),
_isRefraction(false)
{
}

void RayRecord::updateNorm(glm::vec3 n) {
    _norm = n ;
}

void RayRecord::updateT(GLfloat t) {
    _t = t ;
}

void RayRecord::updateColor(glm::vec3 col) {
    _color = col ;
}

void RayRecord::updateMaterial(Material &m) {
    _material = &m ;
}

void RayRecord::updateIntersectedObj(Item * m) {
    _intersectedObj = m ;
}

void RayRecord::updateN(GLfloat N) {
    _prevN = N ;
}


glm::vec3 RayRecord::getNorm() {
    return _norm ;
}

GLfloat RayRecord::getT() {
    return _t ;
}

glm::vec3 RayRecord::getColor() {
    return _color ;
}

Material * RayRecord::getMaterial() {
    return _material ;
}

Item * RayRecord::getIntersectedObj() {
    return _intersectedObj ;
}

glm::vec3 RayRecord::getP0() {
    return _p0 ;
}

glm::vec3 RayRecord::getP1() {
    return _p1 ;
}

GLfloat RayRecord::getPrevN() {
    return _prevN ;
}



RayRecord::~RayRecord(void)
{
}
