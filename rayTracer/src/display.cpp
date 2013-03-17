/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.  
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

using namespace std ; 
#include "variables.h"
#include "readfile.h"

// New helper transformation function to transform vector by modelview 
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.  
// Some of you may want to use the more modern routines in readfile.cpp 
// that can also be used.  

void transformvec (const GLfloat input[4], GLfloat output[4]) {
  GLfloat modelview[16] ; // in column major order
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview) ; 
  
  for (int i = 0 ; i < 4 ; i++) {
    output[i] = 0 ; 
    for (int j = 0 ; j < 4 ; j++) 
      output[i] += modelview[4*j+i] * input[j] ; 
  }
}

void printMat (const mat4 puttr) {
	cout << puttr[0][0] << " " << puttr[0][1] << " " << puttr[0][2] << " " << puttr[0][3] << endl ;
                  cout << puttr[1][0] << " " << puttr[1][1] << " " << puttr[1][2] << " " << puttr[1][3] << endl ;
                  cout << puttr[2][0] << " " << puttr[2][1] << " " << puttr[2][2] << " " << puttr[2][3] << endl ;
                  cout << puttr[3][0] << " " << puttr[3][1] << " " << puttr[3][2] << " " << puttr[3][3] << endl ;
}

void display() {
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        // I'm including the basic matrix setup for model view to 
        // give some sense of how this works.  

	glMatrixMode(GL_MODELVIEW);
	mat4 mv ; 

        if (useGlu) mv = glm::lookAt(eye,center,up) ; 
	else {
          mv = Transform::lookAt(eye,center,up) ; 
          mv = glm::transpose(mv) ; // accounting for row major
        }
		//printMat(mv);
        glLoadMatrixf(&mv[0][0]) ; 

        // Set Light and Material properties for the teapot
        // Lights are transformed by current modelview matrix. 
        // The shader can't do this globally. 
        // So we need to do so manually.  
        if (numused) {
          glUniform1i(enablelighting,true) ;
          
          // YOUR CODE FOR HW 2 HERE.  
          // You need to pass the lights to the shader. 
          // Remember that lights are transformed by modelview first.  
          int lightIndex ;
          for (lightIndex = 0; lightIndex < numused; lightIndex++) {
              GLfloat temp1[4] = { lightposn[lightIndex*4], lightposn[1+lightIndex*4], lightposn[2+lightIndex*4], lightposn[3+lightIndex*4] } ;
              GLfloat temp2[4] ;
              transformvec(temp1, temp2) ;
              lightransf[lightIndex*4] = temp2[0] ;
              lightransf[1 + lightIndex*4] = temp2[1] ;
              lightransf[2 + lightIndex*4] = temp2[2] ;
              lightransf[3 + lightIndex*4] = temp2[3] ;
          }
          /*
          cout << "lightscolor: " << endl ;
          for (int i = 0; i < numused*4; i++) { cout << lightcolor[i] << endl; }
          cout << "lightposn: " << endl ;
          for (int i = 0; i < numused*4; i++) { cout << lightposn[i] << endl; }
          cout << "numused : " << numused << endl ; 
          */
          glUniform4fv(lightpos, numused, lightransf) ;
          glUniform4fv(lightcol, numused, lightcolor) ;
          glUniform1i(numusedcol, numused) ;
        }
        else glUniform1i(enablelighting,false) ; 
     
    
    

        // Transformations for objects, involving translation and scaling 
        mat4 sc(1.0) , tr(1.0), transf(1.0) ; 
        sc = Transform::scale(sx,sy,1.0) ; 
		//std::cout << "sc" << std::endl;
		//printMat(sc);
        //tr = Transform::translate(0.0,0.0,0.0) ;
		tr = Transform::translate(tx,ty,0.0) ; 
		//std::cout << "tr" << std::endl;
		//printMat(tr);
        // YOUR CODE FOR HW 2 HERE.  
        // You need to use scale, translate and modelview to 
        // set up the net transformation matrix for the objects.  
        // Account for GLM issues etc.
        transf = mv * glm::transpose(tr) * glm::transpose(sc);


        for (int i = 0 ; i < numobjects ; i++) {
          object * obj = &(objects[i]) ; 

          {
          // YOUR CODE FOR HW 2 HERE. 
          // Set up the object transformations 
          // And pass in the appropriate material properties
              //for (int i = 0; i < 4; i++) { cout << "ambient" << i << " = " << ambient[i] << endl; }
              glUniform4fv(ambientcol, 1, obj->ambient) ;
              glUniform4fv(diffusecol, 1, obj->diffuse) ;
              glUniform4fv(specularcol, 1, obj->specular) ;
              glUniform4fv(emissioncol, 1, obj->emission) ;
              glUniform1f(shininesscol, obj->shininess) ;
              //std::cout << "obj" << std::endl;
		      //printMat(obj->transform);
              mat4 temp = transf*glm::transpose(obj->transform) ;
              glLoadMatrixf(&temp[0][0]) ;
          }

          // Actually draw the object
          // We provide the actual glut drawing functions for you.  
          if (obj -> type == cube) {
            glutSolidCube(obj->size) ; 
          }
          else if (obj -> type == sphere) {
            const int tessel = 20 ; 
            glutSolidSphere(obj->size, tessel, tessel) ; 
          }
          if (obj -> type == teapot) {
            glutSolidTeapot(obj->size) ; 
          }

        }
    
        glutSwapBuffers();
}
