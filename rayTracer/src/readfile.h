#include <sstream>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RayTracer.h"

void matransform (std::stack<glm::mat4> &transfstack, GLfloat * values) ;
void rightmultiply (const glm::mat4 &M, std::stack<glm::mat4> &transfstack) ;
bool readvals (std::stringstream &s, const int numvals, GLfloat * values) ;
bool readchars(std::stringstream &s, const int numvals, char * str) ;
void readfile (const char * filename, RayTracer &tracer) ;
