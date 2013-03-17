#include <stdlib.h>
#include "Item.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "bvh.h"

glm::vec3 calculateLighting(Light * light, RayRecord &rr, RayTracer &tracer) ;
bool isBlocked(RayRecord &originalIntersect, std::vector<Item *> &objects, Light * light, glm::vec2 offset) ;
bool bvhIsBlocked(RayRecord &originalIntersect, BVH bvh, Light * light, glm::vec2 offset) ;
bool refract(RayRecord &originalIntersect, glm::vec3 viewDir, glm::vec3 normal, GLfloat ref_index, RayRecord &t) ;