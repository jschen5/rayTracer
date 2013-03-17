#include "RayTracer.h"
#include "shading.h"
#include <stdio.h>
#include <iostream>

#define INF std::numeric_limits<float>::infinity()
#define nINF -1 * std::numeric_limits<float>::infinity()


RayTracer::RayTracer(void)
{
    useAttenuation = false ;
    maxdepth = 5 ;
}


bool RayTracer::intersections(RayRecord &rec) {
    bool existIntersect = false ;
    for (int objCount = 0; objCount < objects.size(); objCount++) {
        Item * obj = objects[objCount] ;
        if (obj->intersectionTest(rec, INF)) {
            rec.updateIntersectedObj(obj) ;
            if (!existIntersect) { existIntersect = true ; }
        }
    }
    return existIntersect ;
}

bool RayTracer::bvhIntersections(RayRecord &rec) {
    return this->bvh.intersectionTest(rec, INF) ;
}


glm::vec3 RayTracer::getColor(RayRecord &rec, glm::vec2 offset, GLuint numSamples, GLuint depth) {

    bool existIntersect ;
    if (this->useBVH) {
        existIntersect = bvhIntersections(rec) ;
    } else {
        existIntersect = intersections(rec) ;
    }

    if (depth == maxdepth || !existIntersect) {
        return vec3(0, 0, 0) ;
    } else {    // depth != maxdepth && existIntersect
        Light * light ;
        glm::vec3 ultimateColor = rec.getIntersectedObj()->_material->_ambient + rec.getIntersectedObj()->_material->_emission ;
        //glm::vec3 ultimateColor = vec3(0, 0, 0) ;
        for (int lightIdx = 0; lightIdx < lights.size(); lightIdx++) {
            light = lights[lightIdx] ;
            bool blocked ;
            if (this->useBVH) {     //are we using BVH?
                blocked = bvhIsBlocked(rec, this->bvh, light, offset) ;
            } else {
                blocked = isBlocked(rec, objects, light, offset) ;
            }
            if (!blocked) {
                ultimateColor += calculateLighting(light, rec, *this) ;
            }
        }

        glm::vec3 reflectionP0 = rec.getP0() + rec.getP1() * rec.getT() ;
        glm::vec3 reflectionP1 = rec.getP1() - 2 * (glm::dot(rec.getP1(), rec.getNorm())) * rec.getNorm() ;
        Material mt ;
        RayRecord reflectionRay(INF, mt, reflectionP0 + 0.0005f * reflectionP1, reflectionP1) ;
        
        // compute refractions
        GLfloat refract_index = rec.getIntersectedObj()->_material->_refract_index ;
        if (refract_index > 0) {
            RayRecord t ;
            GLfloat c ;
            glm::vec3 k ;
            glm::vec3 d = rec.getP1() ;
            glm::vec3 n = rec.getNorm() ;
            if (glm::dot(d, n) < 0) {
                refract(rec, d, n, refract_index, t) ;
                c = -glm::dot(glm::normalize(d), n) ;
                k.r = k.g = k.b = 1.0f ;
            } else {
                glm::vec3 rgbAtten = rec.getMaterial()->_rgbAttenuation ;
                k.r = exp(-rgbAtten.r * rec.getT()) ;
                k.g = exp(-rgbAtten.g * rec.getT()) ;
                k.b = exp(-rgbAtten.b * rec.getT()) ;
                if (refract(rec, d, -n, 1/refract_index, t)) {
                    c = glm::dot(glm::normalize(t.getP1()), n) ;
                } else {
                    return k * getColor(reflectionRay, offset, numSamples, depth + 1) ;
                    //return ultimateColor + k * getColor(reflectionRay, offset, numSamples, depth + 1) ;
                    //return rec.getMaterial()->_specular * getColor(reflectionRay, offset, numSamples, depth + 1) ;
                }
            }
            
            GLfloat R0 = ((refract_index - 1) * (refract_index - 1)) / ((refract_index + 1) * (refract_index + 1)) ;
            GLfloat R1 = R0 + (1 - R0) * pow((1 - c), 5) ;
            //glm::vec3 refl_comp = rec.getMaterial()->_specular * R1 * getColor(reflectionRay, offset, numSamples, depth + 1) ;
            glm::vec3 refl_comp = R1 * getColor(reflectionRay, offset, numSamples, depth + 1) ;
            glm::vec3 refr_comp = (1 - R1) * getColor(t, offset, numSamples, depth + 1) ;
            return ultimateColor + k * (refl_comp + refr_comp) ;
        //compute reflections
        } else if (!(rec.getMaterial()->_specular.r == 0 && rec.getMaterial()->_specular.g == 0 && rec.getMaterial()->_specular.b == 0)) {
            glm::vec3 reflectedColor = getColor(reflectionRay, offset, numSamples, depth + 1) ;
            return ultimateColor + rec.getMaterial()->_specular * reflectedColor ;
        } else {
            return ultimateColor ;
        }
    }
}

void RayTracer::createBVH() {
    this->bvh = BVH(this->objects, 0) ;
}

RayTracer::~RayTracer(void)
{
}
