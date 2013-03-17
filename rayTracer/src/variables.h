/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  
#include "Camera.h"
#include "Triangle.h"
#include "Sphere.h"

#ifdef MAINPROGRAM 
#define EXTERN 
/*
EXTERN vec3 ambient = vec3(0, 0, 0) ;
EXTERN vec3 diffuse = vec3(0, 0, 0) ; 
EXTERN vec3 specular = vec3(0, 0, 0) ; 
EXTERN vec3 emission = vec3(0, 0, 0) ; 
EXTERN GLfloat shininess = 1 ; 
*/
#else 
#define EXTERN extern 
//Material properties set before each object is specified
/*
EXTERN vec3 ambient ;
EXTERN vec3 diffuse ; 
EXTERN vec3 specular ; 
EXTERN vec3 emission ; 
EXTERN GLfloat shininess ; 
*/
#endif 

EXTERN vec3 ambient ;
EXTERN vec3 diffuse ; 
EXTERN vec3 specular ; 
EXTERN vec3 emission ; 
EXTERN GLfloat shininess ; 

EXTERN vec3 eye;
EXTERN vec3 center;
EXTERN vec3 up;
EXTERN GLfloat fovy;
EXTERN GLint width;
EXTERN GLint height;

EXTERN GLuint maxdepth ;
EXTERN string outputFile ;


// Lighting parameter array, similar to that in the fragment shader

EXTERN vector<vec4> lightposn ; //Light Positions
EXTERN vector<vec3> lightcolor ; //Light colors
EXTERN vector<vec4> lighttransf ; //Lights transformed by modelview
EXTERN GLfloat constAtten ;
EXTERN GLfloat linearAtten ;
EXTERN GLfloat quadrAtten ;
EXTERN GLuint numused ; // How many lights are used 
 

//geometry
EXTERN GLuint maxverts ;
EXTERN vector<vec3> vertices ; //vertices
EXTERN vector<vec3> triangles ; //indices
//EXTERN vector<vec3> normals ; //normals to be computed per triangle face

EXTERN GLuint maxvertnorms ;
EXTERN vector<vec3> vertices2 ; //vertices specified with normals
EXTERN vector<vec3> triangles2 ; //indices for vertices with normals
EXTERN vector<vec3> normals2 ; //normals per vertex read in from input file

EXTERN vector<Item *> objects ;
EXTERN vector<Triangle> triObjects ;
EXTERN vector<Sphere> sphObjects ;

EXTERN bool pushSeen ;  //used in readfile
EXTERN bool useAttenuation ;

EXTERN Camera cam ;

//EXTERN vec3 smoothNorm ;