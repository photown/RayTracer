/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  

#include <vector>

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up 

EXTERN struct Camera {
    vec3 eyeinit;
    vec3 upinit;
    vec3 center;
    float fovy;

    Camera(vec3 eyeinit, vec3 upinit, vec3 center, float fovy) {
        this->eyeinit = eyeinit;
        this->upinit = upinit;
        this->center = center;
        this->fovy = fovy;
    }
};

EXTERN struct Ray {
    vec3 origin;
    vec3 direction;

    Ray(vec3 origin, vec3 direction) {
        this->origin = origin;
        this->direction = direction;
    }
};

#ifdef MAINPROGRAM 
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
vec3 center(0.0,0.0,0.0) ; // Center look at point 
int amountinit = 5;
int width = 500, height = 500 ; // width and height 
Camera* camera = new Camera(eyeinit, upinit, center, /* fov= */ 90.0);
#else  
EXTERN int amountinit;
EXTERN int width, height ; 
EXTERN Camera* camera ; 
#endif 

EXTERN bool useGlu; // Toggle use of "official" opengl/glm transform vs user 
EXTERN mat4 projection, modelview; // The mvp matrices
static enum {view, translate, scale} transop ; // which operation to transform 
enum shape {ShapeCube, ShapeTriangle, ShapeSphere, ShapeTeapot} ;
EXTERN float sx, sy ; // the scale in x and y 
EXTERN float tx, ty ; // the translation in x and y

// Materials (read from file) 
// With multiple objects, these are colors for each.
EXTERN float ambient[3] ; 
EXTERN float diffuse[3] ; 
EXTERN float specular[3] ; 
EXTERN float emission[3] ; 
EXTERN float shininess ; 
EXTERN vec3 attenuation;

// For multiple objects, read from a file.  
const int maxobjects = 10 ; 
EXTERN int numobjects ; 
EXTERN struct object {
  shape type ; 
  float ambient[3] ; 
  float diffuse[3] ; 
  float specular[3] ;
  float emission[3] ; 
  float shininess ;
  mat4 transform ; 

  virtual ~object() {}
};

EXTERN struct Sphere : object {
    float radius;
    vec3 center;
};

EXTERN struct Triangle : object {
    int index1;
    int index2;
    int index3;
    bool hasNormals;
    Triangle(int index1, int index2, int index3, bool hasNormals) {
        this->index1 = index1;
        this->index2 = index2;
        this->index3 = index3;
        this->hasNormals = hasNormals;
    }
};

EXTERN vector<object*> objects;

EXTERN struct Light {
    float intensity = 1; // between 0 and 1
    vec4 position; //xyzw
    vec4 color; //rgba
    vec4 lighttransf;  // Light transformed by modelview
    vec3 attenuation;
};

EXTERN struct Intersection {
    bool isHit;
    object* object;
    vec3 hitVector;
    vec3 hitNormal;
};

EXTERN vector<Light*> lights;
EXTERN int maxdepth;
EXTERN string outputLocation;

EXTERN vector<vec3*> vertices;
EXTERN vector<pair<vec3*, vec3*>*> vertexnormals;

// Variables to set uniform params for lighting fragment shader 
EXTERN unsigned int lightcol ; 
EXTERN unsigned int lightpos ;
EXTERN unsigned int numusedcol ;
EXTERN unsigned int enablelighting ;
EXTERN unsigned int amientcol ;
EXTERN unsigned int diffusecol ;
EXTERN unsigned int specularcol ;
EXTERN unsigned int emissioncol ;
EXTERN unsigned int shininesscol ;

