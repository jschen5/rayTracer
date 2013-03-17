#include "Material.h"


Material::Material(void)
{
}

Material::Material(glm::vec3 ambi, glm::vec3 diff, glm::vec3 spec, GLfloat shin, glm::vec3 emis, glm::vec3 rgbAtten, GLfloat refr_index):
_ambient(ambi), 
_diffuse(diff), 
_specular(spec), 
_shininess(shin), 
_emission(emis),
_rgbAttenuation(rgbAtten),
_refract_index(refr_index)
{
}



Material::~Material(void)
{
}
