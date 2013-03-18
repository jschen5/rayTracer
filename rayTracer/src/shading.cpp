#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>

#include "Item.h"
#include "RayRecord.h"
#include "RayTracer.h"
#include "shading.h"

#define INF std::numeric_limits<float>::infinity()
#define nINF -1 * std::numeric_limits<float>::infinity()

glm::vec3 calculateLighting(Light * light, RayRecord &rr, RayTracer &tracer) {

    glm::vec3 finalcolor = vec3(0, 0, 0) ; 
    glm::vec3 normal = rr.getNorm() ;
    
    glm::vec3 intersection = rr.getP0() + rr.getP1() * rr.getT() ;
    glm::vec3 eyeDir = rr.getP0() - intersection ;

    glm::vec3 lightDir ;
    glm::vec3 lightLoc (light->_position.x, light->_position.y, light->_position.z) ;
    // determine course of action based on point/directional light source
    GLfloat attenuationFactor = 1.0f ;
    if (!light->isPointLight()) {
        lightDir = glm::normalize(vec3(light->_position.x, light->_position.y, light->_position.z)) ;
    } else {
        if (tracer.useAttenuation) {
            GLfloat dist = glm::sqrt((intersection.x - lightLoc.x) * (intersection.x - lightLoc.x)
                                    + (intersection.y - lightLoc.y) * (intersection.y - lightLoc.y)
                                    + (intersection.x - lightLoc.z) * (intersection.x - lightLoc.z)) ;
            attenuationFactor = 1 / (GLfloat) (tracer.constAtten + tracer.linearAtten * dist + tracer.quadrAtten * dist * dist) ;
        }
        lightDir = glm::normalize(vec3(light->_position.x, light->_position.y, light->_position.z) - intersection);
    }

    glm::vec3 halfVect = glm::normalize(lightDir + glm::normalize(eyeDir)) ;  

    //compute diffuse term
    glm::vec3 cdiff_lcol = rr.getMaterial()->_diffuse * light->_color ;
	GLfloat maxFactor  = glm::max((GLfloat) glm::dot(normal, lightDir), 0.0f) ;
    glm::vec3 diffuseTerm = cdiff_lcol * maxFactor;

    //compute specular term
    glm::vec3 cspec_lcol = rr.getMaterial()->_specular * light->_color;
    maxFactor = pow(glm::max((GLfloat) glm::dot(normal, halfVect), 0.0f), rr.getMaterial()->_shininess);
    glm::vec3 specularTerm = cspec_lcol * maxFactor;
    
    //finalcolor = rr.getMaterial()->_ambient + rr.getMaterial()->_emission + attenuationFactor * (diffuseTerm + specularTerm) ;
    finalcolor = attenuationFactor * (diffuseTerm + specularTerm) ;
    return finalcolor ;
}


bool isBlocked(RayRecord &originalIntersect, std::vector<Item *> &objects, Light * light, glm::vec2 offset) {

    glm::vec3 lightP0 = originalIntersect.getP0() + originalIntersect.getP1() * originalIntersect.getT() ;
    glm::vec3 lightP1 ;

    //glm::vec3 lightDir ;
    GLfloat boundT = INF ;

    if (!light->isPointLight()) {   //if directional light
        lightP1 = light->_position ;
        //lightDir = glm::normalize(glm::vec3(lightposn.x, lightposn.y, lightposn.z)) ;
    } else {    //if point light
        lightP1 = light->_position - lightP0 ;
        //lightDir = glm::normalize(glm::vec3(lightposn.x, lightposn.y, lightposn.z) - lightP0) ;
        boundT = 1.0f ;
    }
    //Material mt ;
    for (int objCount = 0; objCount < objects.size(); objCount++) {
        //RayRecord shadowRay(INF, mt, lightP0 + 0.0005f * lightP1, lightP1) ;
        RayRecord * shadowRay = light->getRayToLight(lightP0, offset) ;
        if (objects[objCount]->intersectionTest(*shadowRay, boundT)) {   // if any objects block, return true
            return true ;
        }
        delete shadowRay ;
    }
    //no objects between intersection and light, return false
    return false ;
}

bool bvhIsBlocked(RayRecord &originalIntersect, BVH bvh, Light * light, glm::vec2 offset) {
    glm::vec3 lightP0 = originalIntersect.getP0() + originalIntersect.getP1() * originalIntersect.getT() ;
    glm::vec3 lightP1 ;

    //glm::vec3 lightDir ;
    GLfloat boundT = INF ;

    if (!light->isPointLight()) {   //if directional light
        lightP1 = light->_position ;
    } else {    //if point light
        lightP1 = light->_position - lightP0 ;
        boundT = 1.0f ;
    }
    //Material mt ;
    //RayRecord shadowRay(INF, mt, lightP0 + 0.0005f * lightP1, lightP1) ;
    RayRecord * shadowRay = light->getRayToLight(lightP0, offset) ;
    //return bvh.intersectionTest(*shadowRay, boundT) ;
    bool retval = bvh.intersectionTest(*shadowRay, boundT) ;
    delete shadowRay ;
    return retval ;
}


bool refract(RayRecord &originalIntersect, glm::vec3 viewDir, glm::vec3 normal, GLfloat ref_index, RayRecord &t) {
    GLfloat ratio = 1 / (float) ref_index ;
    /*
    GLfloat ratio ;
    if (originalIntersect.getIntersectedObj() == originalIntersect.getPrevObj()) {
        originalIntersect.getRefractStack()->pop() ;
        ratio = originalIntersect.getRefractStack()->top() / (float) ref_index ;
    }
    */
    GLfloat discriminant = 1 - ratio * ratio * (1 - glm::dot(viewDir, normal) * glm::dot(viewDir, normal)) ;
    if (discriminant < 0) {
        return false ;  //total internal reflection, all energy reflected
    }
    glm::vec3 t_p0 = originalIntersect.getP0() + originalIntersect.getP1() * originalIntersect.getT() ;
    glm::vec3 t_p1 = ratio * (viewDir - normal * glm::dot(viewDir, normal)) - (normal * glm::sqrt(discriminant)) ;
    t = RayRecord(INF, *originalIntersect.getMaterial(), t_p0 + 0.0001f * t_p1, t_p1) ;
    t._isRefraction = true ;
    return true ;
}