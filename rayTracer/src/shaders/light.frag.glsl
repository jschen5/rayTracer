# version 120 

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color ;  
// in vec4 mynormal ; 
// in vec4 myvertex ;
// That is certainly more modern

varying vec4 color ;
varying vec3 mynormal ; 
varying vec4 myvertex ; 

const int numLights = 10 ; 
uniform bool enablelighting ; // are we lighting at all (global).
uniform vec4 lightposn[numLights] ; // positions of lights 
uniform vec4 lightcolor[numLights] ; // colors of lights
uniform int numused ;               // number of lights used

// Now, set the material parameters.  These could be varying and/or bound to 
// a buffer.  But for now, I'll just make them uniform.  
// I use ambient, diffuse, specular, shininess as in OpenGL.  
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient ; 
uniform vec4 diffuse ; 
uniform vec4 specular ; 
uniform vec4 emission ; 
uniform float shininess ; 

vec3 lightDir ;
vec3 surfaceNorm ;

void main (void) 
{       
    if (enablelighting) {       
        
        vec4 finalcolor ; 

        // YOUR CODE FOR HW 2 HERE
        // A key part is implementation of the fragment shader

        const vec3 eyepos = vec3(0,0,0) ; 
        vec4 _mypos = gl_ModelViewMatrix * myvertex ; 
        vec3 mypos = _mypos.xyz / _mypos.w ; // Dehomogenize current location 
        vec3 eyedirn = normalize(eyepos - mypos) ; 

        // Compute normal, needed for shading. 
        // Simpler is vec3 normal = normalize(gl_NormalMatrix * mynormal) ; 
        vec3 _normal = (gl_ModelViewMatrixInverseTranspose*vec4(mynormal,0.0)).xyz ; 
        vec3 normal = normalize(_normal) ; 

		vec4 sigmaLight = vec4(0, 0, 0, 0) ;
		int lightIndex ;

        for (lightIndex = 0; lightIndex < numused; lightIndex++) {

            vec4 lposn = lightposn[lightIndex] ;
            vec3 lightPos = lposn.xyz / lposn.w ;
            
            //determine course of action based on
            //whether point or directional light source
            if (lposn.w == 0) {
                lightDir = normalize(lposn.xyz) ;
            } else {
                lightDir = normalize(lightPos - mypos); // no attenuation 
            } 
            vec3 halfAngle = normalize(lightDir + eyedirn);  

            surfaceNorm = normal;
            vec4 lightCol = lightcolor[lightIndex] ;

            //compute diffuse term
            vec4 cdiff_lcol = diffuse*lightCol;
			float maxFactor  = max(dot(surfaceNorm, lightDir), 0);   //lightDirection is negative of position vector
            vec4 diffuseTerm = cdiff_lcol*maxFactor;

            //compute specular term
            vec4 cspec_lcol = specular*lightCol;
            //vec3 reflection = -lightDir + 2*(dot(lightDir, surfaceNorm))*surfaceNorm;
            //vec3 halfAngle = normalize(lightDir + reflection) ;
            maxFactor = pow(max(dot(surfaceNorm, halfAngle), 0), shininess);
            vec4 specularTerm = cspec_lcol*maxFactor;

            //add to running total (for sum over all the lights)
            sigmaLight += (diffuseTerm + specularTerm);
		}
        finalcolor = ambient + emission + sigmaLight;

        
            float intensity = dot(lightDir, surfaceNorm) ;
            if (intensity > .95) {
                finalcolor = finalcolor*vec4(1.0, 1.0, 1.0, 1.0) ;
                //keep as final color
            } else if (intensity > .5) {
                //finalcolor = finalcolor*3/5 ;
                finalcolor = finalcolor*vec4(0.8, 0.8, 0.8, 1.0) ;
            } else if (intensity > .25) {
                //finalcolor = finalcolor*2/5 ;
                finalcolor = finalcolor*vec4(0.6, 0.6, 0.6, 1.0) ;
            } else {
                //finalcolor = finalcolor*1/5 ;
                finalcolor = finalcolor*vec4(0.4, 0.4, 0.4, 1.0) ;
            }
        
        //finalcolor = vec4(numused/10, numused/10, numused/10, 1) ;
        //finalcolor = vec4(1, 1, 1, 1) ;
        gl_FragColor = finalcolor ;
        }
    else gl_FragColor = color ; 
}
