#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h" 
#include "readfile.h"
#include "RayTracer.h"

void printMat (const mat4 puttr) {
	std::cout << puttr[0][0] << " " << puttr[0][1] << " " << puttr[0][2] << " " << puttr[0][3] << std::endl ;
                  std::cout << puttr[1][0] << " " << puttr[1][1] << " " << puttr[1][2] << " " << puttr[1][3] << std::endl ;
                  std::cout << puttr[2][0] << " " << puttr[2][1] << " " << puttr[2][2] << " " << puttr[2][3] << std::endl ;
                  std::cout << puttr[3][0] << " " << puttr[3][1] << " " << puttr[3][2] << " " << puttr[3][3] << std::endl ;
}

void printVecNo (const glm::vec3 puttr) {
    std::cout << puttr[0] << ", " << puttr[1] << ", " << puttr[2]  << std::endl ;
}

// The function below applies the appropriate transform to a 4-vector
void matransform(std::stack<mat4> &transfstack, GLfloat * values) {
  mat4 transform = transfstack.top() ; 
  vec4 valvec = vec4(values[0],values[1],values[2],values[3]) ; 
  vec4 newval = valvec * transform ; 
  for (int i = 0 ; i < 4 ; i++) values[i] = newval[i] ; 
}

void rightmultiply(const mat4 & M, std::stack<mat4> &transfstack) {
  mat4 &T = transfstack.top() ; 
  // Right multiply M, but do this left to account for row/column major 
  T = M * T ; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(std::stringstream &s, const int numvals, GLfloat * values) {
  for (int i = 0 ; i < numvals ; i++) {
    s >> values[i] ; 
    if (s.fail()) {
      std::cout << "Failed reading value " << i << " will skip\n" ; 
      return false ;
    }
  }
  return true ; 
}

bool readchars(std::stringstream &s, const int numvals, char * str) {
  for (int i = 0 ; i < numvals ; i++) {
    s >> str ; 
    if (s.fail()) {
      std::cout << "Failed reading value " << i << " will skip\n" ; 
      return false ;
    }
  }
  return true ; 
}

void readfile (const char * filename, RayTracer &tracer) {
  std::string str, cmd ; 
  std::ifstream in ;
  std::cout << "filename:" << filename ;
  in.open(filename) ; 
  if (in.is_open()) {

    GLfloat width, height ;

    std::stack <mat4> transfstack ; 
    transfstack.push(mat4(1.0)) ;  // identity

    vec3 ambient = vec3(0, 0, 0) ;
    vec3 diffuse = vec3(0, 0, 0) ; 
    vec3 specular = vec3(0, 0, 0) ; 
    vec3 emission = vec3(0, 0, 0) ;
    vec3 rgb_atten = vec3(1, 1, 1) ;
    GLfloat refr_index = -1.0f ;
    GLfloat shininess = 1.0f ;

    getline (in, str) ; 
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#')) {
        // Ruled out comment and blank lines 

        std::stringstream s(str) ;
        s >> cmd ; 
        int i ; 
        GLfloat values[12] ; // position and color for light, colors for others
                             // Up to 12 params for cameras.  
        std::string out ; //name of output file
        bool validinput ; // validity of input 

        /*
        if (cmd == "directional") {
            validinput = readvals(s, 6, values) ;
            if (validinput) {
                tracer.lpositions.push_back(vec4(values[0], values[1], values[2], 0.0)) ;
                tracer.lcolors.push_back(vec3(values[3], values[4], values[5])) ;
            }
        }
        */
        if (cmd == "point") {
            validinput = readvals(s, 7, values) ;
            if (validinput) {
                glm::vec3 lpos(values[0], values[1], values[2]) ;
                glm::vec3 lcol(values[4], values[5], values[6]) ;
                Light * light = new PointLight(lpos, lcol, values[3]) ; //4th arg is area
                tracer.lights.push_back(light) ;
            }
        }

        else if (cmd == "attenuation") {
            validinput = readvals(s, 3, values) ;
            if (validinput) {
                tracer.useAttenuation = true ;
                tracer.constAtten = values[0] ;
                tracer.linearAtten = values[1] ;
                tracer.quadrAtten = values[2] ;
            }
        }

        // Material Commands 
        // Ambient, diffuse, specular, shininess

        else if (cmd == "ambient") {
          validinput = readvals(s, 3, values) ; // colors 
          if (validinput) 
            ambient = vec3(values[0], values[1], values[2]) ;
        }
        else if (cmd == "diffuse") {
          validinput = readvals(s, 3, values) ; 
          if (validinput) 
            diffuse = vec3(values[0], values[1], values[2]) ; 
        }
        else if (cmd == "specular") {
          validinput = readvals(s, 3, values) ; 
          if (validinput) 
            specular = vec3(values[0], values[1], values[2]) ; 
        }
        else if (cmd == "emission") {
          validinput = readvals(s, 3, values) ; 
          if (validinput) 
            emission = vec3(values[0], values[1], values[2]) ; 
        }
        else if (cmd == "shininess") {
          validinput = readvals(s, 1, values) ; 
          if (validinput) shininess = values[0] ; 
        }

        else if (cmd == "a") {
            validinput = readvals(s, 3, values) ;
            if (validinput)
                rgb_atten = vec3(values[0], values[1], values[2]) ;
        }

        else if (cmd == "nt") {
            validinput = readvals(s, 1, values) ;
            if (validinput)
                refr_index = values[0] ;
        }

        else if (cmd == "size") {
            validinput = readvals(s, 2, values) ;
            if (validinput) {
                width = values[0] ;
                height = values[1] ;
            }
        }

        else if (cmd == "camera") {
          validinput = readvals(s,12,values) ; // 12 values eye cen up fov lens foc
          if (validinput) {

			  vec3 eye (values[0], values[1], values[2]) ;
			  vec3 center (values[3], values[4], values[5]) ;
			  vec3 up (values[6], values[7], values[8]) ;
			  GLfloat fovy = values[9] ;
              GLfloat lens = values[10] ;
              GLfloat foc = values[11] ;

              tracer.cam = Camera(eye, center, up, fovy, foc, lens, width, height) ;

          } 
        }
        
        else if (cmd == "antialias") {
            validinput = readvals(s, 1, values) ;
            if (validinput)
                tracer.numSamples = values[0] ;
        }
        
		else if (cmd == "maxdepth") {
		  validinput = readvals(s, 1, values) ;
		  if (validinput) {
			  tracer.maxdepth = values[0] ;
		  }
		}
        /*
	    else if (cmd == "output") {
		  validinput = readchars(s, 1, out) ;
		  if (validinput) {
	          outputFile = out ;
		  }
        }
        */
        
        else if (cmd == "sphere") {
            validinput = readvals(s, 4, values) ;
            if (validinput) {
                vec3 sphCent (values[0], values[1], values[2]) ;

                Material * mt = new Material(ambient, diffuse, specular, shininess, emission, rgb_atten, refr_index) ;
                Sphere * sphere = new Sphere(sphCent, values[3], *mt, transfstack.top()) ;

                tracer.objects.push_back(sphere) ;
            }
        }
        
        else if (cmd == "maxverts") {
            validinput = readvals(s, 1, values) ;
            if (validinput) {
            }
        }
        else if (cmd == "maxvertnorms") {
            validinput = readvals(s, 1, values) ;
            if (validinput) {
            }
        }
        else if (cmd == "vertex") {
            validinput = readvals(s, 3, values) ;
            if (validinput) {
                vec3 newVert (values[0], values[1], values[2]) ;
                tracer.vertices.push_back(newVert) ;
            }
        }
        else if (cmd == "vertexnormal") {
            validinput = readvals(s, 6, values) ;
            if (validinput) {
                vec3 newVert (values[0], values[1], values[2]) ;
                vec3 newNorm (values[3], values[4], values[5]) ;
                //vertices2.push_back(newVert) ;
                //normals2.push_back(newNorm) ;
            }
        }
        else if (cmd == "tri") {
            validinput = readvals(s, 3, values) ;
            if (validinput) {
                vec3 newTri (values[0], values[1], values[2]) ;
                mat4 mv = transfstack.top() ;

                std::vector<vec3> newVerts ;

                vec3 a = tracer.vertices[newTri.x] ;
                //printVecNo(a) ;
                newVerts.push_back(a) ;
                vec4 atemp = glm::transpose(mv) * vec4(a.x, a.y, a.z, 1.0) ;
                a = vec3(atemp.x, atemp.y, atemp.z) / atemp.w ;
                //printVecNo(a) ;

                vec3 b = tracer.vertices[newTri.y] ;
                newVerts.push_back(b) ;
                vec4 btemp = glm::transpose(mv) * vec4(b.x, b.y, b.z, 1.0) ;
                b = vec3(btemp.x, btemp.y, btemp.z) / btemp.w ; 

                vec3 c = tracer.vertices[newTri.z] ;
                newVerts.push_back(c) ;
                vec4 ctemp = glm::transpose(mv) * vec4(c.x, c.y, c.z, 1.0) ;
                c = vec3(ctemp.x, ctemp.y, ctemp.z) / ctemp.w ;

                std::vector<vec3> newTVerts ;
                newTVerts.push_back(a) ;
                newTVerts.push_back(b) ;
                newTVerts.push_back(c) ;

                vec3 v1 = b - a ;
                vec3 v2 = c - a ;
    
                vec3 newNorm = glm::normalize(glm::cross(v1, v2)) ;

                Material * mtr = new Material(ambient, diffuse, specular, shininess, emission, rgb_atten, refr_index) ;
                Triangle * tri = new Triangle(newNorm, mv, *mtr, newVerts, newTVerts) ;

                tracer.objects.push_back(tri) ;
            }
        }
        else if (cmd == "trinormal") {
            validinput = readvals(s, 3, values) ;
            if (validinput) {
                vec3 newTri (values[0], values[1], values[2]) ;
                
                //Triangle delta ;
            }
        }

        //TRANSFORMATIONS
        else if (cmd == "translate") {
          validinput = readvals(s,3,values) ; 
          if (validinput) {
              rightmultiply(Transform::translate(values[0], values[1], values[2]), transfstack) ;
          }
        }
        else if (cmd == "scale") {
          validinput = readvals(s,3,values) ; 
          if (validinput) {
              rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack) ;
          }
        }
        else if (cmd == "rotate") {
          validinput = readvals(s,4,values) ; 
          if (validinput) {
            // values[0..2] are the axis, values[3] is the angle.  
            // You may want to normalize the axis (or in Transform::rotate)
              vec3 normAxis = glm::normalize(vec3(values[0], values[1], values[2])) ;
              mat3 r = glm::transpose(Transform::rotate(values[3], normAxis)) ;
              mat4 rotMat ( vec4(r[0][0], r[0][1], r[0][2], 0), vec4(r[1][0], r[1][1], r[1][2], 0),
                            vec4(r[2][0], r[2][1], r[2][2], 0), vec4(0, 0, 0, 1) ) ;
              rightmultiply(rotMat, transfstack) ;
          }
        }
        
        // the basic push/pop code for matrix stacks
        else if (cmd == "pushTransform") { transfstack.push(transfstack.top()) ; }

        else if (cmd == "popTransform") {
          if (transfstack.size() <= 1) 
            std::cerr << "Stack has no elements.  Cannot Pop\n" ; 
          else { transfstack.pop() ; }
        }
        
        else {
          std::cerr << "Unknown Command: " << cmd << " Skipping \n" ; 
        }
      }
      getline (in, str) ; 
    }
    

  }
  else {
    std::cerr << "Unable to Open Input Data File " << filename << "\n" ; 
    throw 2 ; 
  }
  
}
