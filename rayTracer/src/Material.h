#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>

class Material
{
public:

    glm::vec3 _ambient ;
    glm::vec3 _diffuse ;
    glm::vec3 _specular ;
    GLfloat _shininess ;
    glm::vec3 _emission ;
    glm::vec3 _rgbAttenuation ;
    GLfloat _refract_index ;

    Material(void);
    Material(glm::vec3 ambi, glm::vec3 diff, glm::vec3 spec, GLfloat shin, glm::vec3 emis, glm::vec3 rgbAtten, GLfloat refr_index) ;
    ~Material(void);
};

