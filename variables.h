#pragma once
/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  

#include <vector>
#include <utility>
#include <string>
#include "Camera.h"
#include "Object.h"

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

#ifdef MAINPROGRAM 
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
int amountinit = 5;
int width = 500, height = 500 ; // width and height 
Camera* camera = new Camera(eyeinit, upinit, vec3(0.0, 0.0, 0.0), /* fov= */ 90.0);
#else  
EXTERN int amountinit;
EXTERN int width, height ; 
EXTERN Camera* camera ; 
#endif 

// Materials (read from file) 
// With multiple objects, these are colors for each.
EXTERN float ambient[3] ; 
EXTERN float diffuse[3] ; 
EXTERN float specular[3] ; 
EXTERN float emission[3] ; 
EXTERN float shininess ; 
EXTERN vec3 attenuation;


EXTERN std::vector<Object*> objects;

struct Light {
    float intensity = 1; // between 0 and 1
    vec4 position; //xyzw
    vec4 color; //rgba
    vec4 lighttransf;  // Light transformed by modelview
    vec3 attenuation;
};

EXTERN std::vector<Light*> lights;
EXTERN int maxdepth;
EXTERN std::string outputLocation;

EXTERN std::vector<vec3*> vertices;
EXTERN std::vector<std::pair<vec3*, vec3*>*> vertexnormals;