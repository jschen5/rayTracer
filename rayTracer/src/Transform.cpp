// Transform.cpp: implementation of the Transform class.


#include "Transform.h"
#include <iostream>

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW2 HERE
  // Please implement this.  Likely the same as in HW 1.  
	const float cos_const = cos(glm::radians(degrees));
	const float sin_const = sin(glm::radians(degrees));
	mat3 outer_product(axis[0]*axis[0], axis[0]*axis[1], axis[0]*axis[2], 
						axis[0]*axis[1], axis[1]*axis[1], axis[1]*axis[2], 
						axis[0]*axis[2], axis[1]*axis[2], axis[2]*axis[2]);
	mat3 dual_mat(0, axis[2], -axis[1], -axis[2], 0, axis[0], axis[1], -axis[0], 0);
	mat3 rot_mat = mat3(1.0)*cos_const + outer_product*(1-cos_const) + dual_mat*sin_const;
	return rot_mat;
}

mat4 Transform::rotateHomog(const float degrees, const vec3& axis) {
    vec3 normAxis = glm::normalize(axis) ;
    mat3 r = glm::transpose(Transform::rotate(degrees, normAxis)) ;
    mat4 rotMat ( vec4(r[0][0], r[0][1], r[0][2], 0), vec4(r[1][0], r[1][1], r[1][2], 0),
                  vec4(r[2][0], r[2][1], r[2][2], 0), vec4(0, 0, 0, 1) ) ;
    return rotMat ;
}

void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.  
	eye = rotate(degrees, up)*eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW2 HERE 
  // Likely the same as in HW 1.  
	vec3 axis = glm::normalize(glm::cross(eye, up));
	eye = rotate(degrees, axis)*eye;
	up = rotate(degrees, axis)*up;
}


mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.  
	vec3 w = glm::normalize(eye);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);
	mat4 r(vec4(u[0], u[1], u[2], 0),
			vec4(v[0], v[1], v[2], 0),
			vec4(w[0], w[1], w[2], 0),
			vec4(0, 0, 0, 1));
	mat4 t(vec4(1, 0, 0, -eye[0]),
			vec4(0, 1, 0, -eye[1]),
			vec4(0, 0, 1, -eye[2]),
			vec4(0, 0, 0, 1));
	return t*r;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.  
	float a = - (zFar + zNear) / (zFar - zNear);
	float b = -2 * zNear * zFar / (zFar-zNear);
	float d = 1 / glm::tan(glm::radians(fovy)/2);
	mat4 p(vec4(d/aspect, 0, 0, 0),
			vec4(0, d, 0, 0),
			vec4(0, 0, a, b),
			vec4(0, 0, -1, 0));
	return p;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) {
  // YOUR CODE FOR HW2 HERE
  // Implement scaling 
	mat4 s(vec4(sx, 0, 0, 0),
			vec4(0, sy, 0, 0),
			vec4(0, 0, sz, 0),
			vec4(0, 0, 0, 1));
	return s;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) {
  // YOUR CODE FOR HW2 HERE
  // Implement translation
	mat4 t(vec4(1, 0, 0, tx),
			vec4(0, 1, 0, ty),
			vec4(0, 0, 1, tz),
			vec4(0, 0, 0, 1));
	return t;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) {
  vec3 x = glm::cross(up,zvec) ; 
  vec3 y = glm::cross(zvec,x) ; 
  vec3 ret = glm::normalize(y) ; 
  return ret ; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
