//#include <vld.h>

#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
//#include <GL/glew.h>
//#include <GL/glut.h>
#include <FreeImage.h>

#include "readfile.h" // prototypes for readfile.cpp  
#include "shading.h"
#include "RayTracer.h"
#include "bvh.h"

#include "Camera.h"
#include "RayRecord.h"
#include "Material.h"
#include "Triangle.h"
#include "Sphere.h"

#include <omp.h>

#define INF std::numeric_limits<float>::infinity()
#define nINF -1 * std::numeric_limits<float>::infinity()

void printMt (const mat4 puttr) {
	std::cout << puttr[0][0] << " " << puttr[0][1] << " " << puttr[0][2] << " " << puttr[0][3] << std::endl ;
                  std::cout << puttr[1][0] << " " << puttr[1][1] << " " << puttr[1][2] << " " << puttr[1][3] << std::endl ;
                  std::cout << puttr[2][0] << " " << puttr[2][1] << " " << puttr[2][2] << " " << puttr[2][3] << std::endl ;
                  std::cout << puttr[3][0] << " " << puttr[3][1] << " " << puttr[3][2] << " " << puttr[3][3] << std::endl ;
}

void printV3 (const vec3 puttr) {
    std::cout << puttr[0] << ", " << puttr[1] << ", " << puttr[2]  << std::endl ;
}



void saveScreenshot(std::string fname, GLfloat * pixels, GLuint width, GLuint height) {
	FreeImage_Initialise();
    int pix = width * height;
    RGBQUAD color ;
    FIBITMAP * img = FreeImage_Allocate(width, height, 24) ;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            color.rgbRed = pixels[j*width*3 + i*3] * 255 ;
            color.rgbGreen = pixels[j*width*3 + i*3 + 1] * 255 ;
            color.rgbBlue = pixels[j*width*3 + i*3 + 2] * 255 ;
            FreeImage_SetPixelColor(img, i, j, &color) ;
        }
    }
	std::cout << "Saving screenshot: " << fname << std::endl ;

	//FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
    FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
	//delete pixels;
    FreeImage_DeInitialise();
}


int main(int argc, char* argv[]) {

    RayTracer tracer ;

    readfile(argv[1], tracer) ;

    tracer.useBVH = true ;

    tracer.createBVH() ;
    
    GLuint width = (GLuint) tracer.cam._width ;
    GLuint height = (GLuint) tracer.cam._height ;

    GLfloat * pxls = new GLfloat[width * height * 3] ;

    int iCPU = omp_get_num_procs();
    omp_set_num_threads(iCPU);

    int i = 0;
    #pragma omp parallel for private(i)
    for (int j = 0; j < height; j++) {
        std::cout << j << std::endl ;
        for (i = 0; i < width; i++) {

            glm::vec3 finalColor = vec3(0, 0, 0) ;
            RayRecord rec ;
            
            if (tracer.numSamples > 1) {
                
                GLuint n_sq = tracer.numSamples * tracer.numSamples ;
                glm::vec2 * rArr = new glm::vec2[n_sq] ;
                glm::vec2 * sArr = new glm::vec2[n_sq] ;
                glm::vec2 * eArr = new glm::vec2[n_sq] ;
                for (int p = 0; p < tracer.numSamples; p++) {
                    for (int q = 0; q < tracer.numSamples; q ++) {
                        GLfloat r1 = (std::rand() % 1000) / 1000.0f ;
                        GLfloat r2 = (std::rand() % 1000) / 1000.0f ;
                        GLfloat s1 = (std::rand() % 1000) / 1000.0f ;
                        GLfloat s2 = (std::rand() % 1000) / 1000.0f ;
                        GLfloat e1 = (std::rand() % 1000) / 1000.0f ;
                        GLfloat e2 = (std::rand() % 1000) / 1000.0f ;

                        rArr[p + q * tracer.numSamples] = glm::vec2((p + r1) / (GLfloat) tracer.numSamples, (q + r2) / (GLfloat) tracer.numSamples) ;
                        sArr[p + q * tracer.numSamples] = glm::vec2((p + s1) / (GLfloat) tracer.numSamples, (q + s2) / (GLfloat) tracer.numSamples) ;
                        eArr[p + q * tracer.numSamples] = glm::vec2((p + e1) / (GLfloat) tracer.numSamples, (q + e2) / (GLfloat) tracer.numSamples) ;
                    }
                }
                for (int p = n_sq - 1; p > 0; p--) {   //shuffle sArr
                    GLuint randInt = std::rand() % p ;
                    glm::vec2 temp = sArr[randInt] ;
                    sArr[randInt] = sArr[p] ;
                    sArr[p] = temp ;
                }
                for (int p = n_sq - 1; p > 0; p--) {   //shuffle eArr
                    GLuint randInt = std::rand() % p ;
                    glm::vec2 temp = eArr[randInt] ;
                    eArr[randInt] = eArr[p] ;
                    eArr[p] = temp ;
                }
                for (int p = 0; p < tracer.numSamples; p++) {
                    for (int q = 0; q < tracer.numSamples; q++) {
                        rec = tracer.cam.generateRay(i, j, eArr[p + q * tracer.numSamples], rArr[p + q * tracer.numSamples]) ;
                        finalColor += glm::clamp(tracer.getColor(rec, sArr[p + q * tracer.numSamples], tracer.numSamples, 0), 0.0f, 1.0f) ;
                    }
                }
                finalColor = finalColor / (GLfloat) n_sq ;
                delete[] rArr ;
                delete[] sArr ;
                delete[] eArr ;
            } else {
                rec = tracer.cam.generateRay(i, j, glm::vec2(0.5, 0.5), glm::vec2(0.5, 0.5)) ;
                finalColor = glm::clamp(tracer.getColor(rec, glm::vec2(0.5, 0.5), 1, 0), 0.0f, 1.0f) ;
            }
            
            pxls[j*width*3 + i*3] = finalColor.r ;
            pxls[j*width*3 + i*3 + 1] = finalColor.g ;
            pxls[j*width*3 + i*3 + 2] = finalColor.b ;
        }
    }

    //cleanup (delete objects which are all allocated on heap)
    for (int objCount = 0; objCount < tracer.objects.size(); objCount++) {
        delete tracer.objects[objCount] ;
    }

    std::cout << "saving image" << std::endl ;
    std::string s = "scene1.png" ;
    saveScreenshot(s, pxls, width, height) ;
    delete[] pxls ;

}
