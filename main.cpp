/******************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi  */
/* Extends HW 1 to deal with shading, more transforms and multiple objects    */
/******************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h"
#include <FreeImage.h>
#include <glm/gtx/norm.hpp>
#include <cmath>

using namespace std ; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  
#include "main.h"
void display(void) ;  // prototype for display function.  

bool allowGrader = false;
vec3 tempVec = vec3();

void swapRedBlueChannels24bit(BYTE* freeImagePixels, int totalBytes) {
    int totalPixels = totalBytes / 3;
    for (int i = 0; i < totalPixels; i++) {
        unsigned char temp = freeImagePixels[i * 3];
        freeImagePixels[i * 3] = freeImagePixels[i * 3 + 2];
        freeImagePixels[i * 3 + 2] = temp;
    }
}

// pixels must be of size 3*width*height with 8 bits for R, G and B
void saveScreenshot(string fname, unsigned char* pixels, int width, int height) {
  BYTE *freeImagePixels = new BYTE[3 * width * height];
  int totalPixels = width * height;
  int totalValues = totalPixels * 3;
  for (int i = 0; i < totalValues; i++) {
      freeImagePixels[i] = pixels[i];
  }

  if (FreeImage_IsLittleEndian()) {
      swapRedBlueChannels24bit(freeImagePixels, 3 * width * height);
  }

  FIBITMAP *img = FreeImage_ConvertFromRawBits(freeImagePixels, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, /* startFromTopLeft = */ true);

  std::cout << "Saving screenshot: " << fname << "\n";

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
  delete []freeImagePixels;
}


void printHelp() {
  std::cout << "\npress 'h' to print this message again.\n" 
    << "press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n" 
    << "press 'i' to run image grader test cases\n"
    << "press 'g' to switch between using glm::lookAt and glm::Perspective or your own LookAt.\n"       
    << "press 'r' to reset the transformations.\n"
    << "press 'v' 't' 's' to do view [default], translate, scale.\n"
    << "press ESC to quit.\n" ;      
}


void keyboard(unsigned char key, int x, int y) {
  switch(key) {
    case '+':
      amount++;
      std::cout << "amount set to " << amount << "\n" ;
      break;
    case '-':
      amount--;
      std::cout << "amount set to " << amount << "\n" ; 
      break;
    case 'h':
      printHelp();
      break;
    case 27:  // Escape to quit
      exit(0) ;
      break ;
    case 'r': // reset eye and up vectors, scale and translate. 
      eye = eyeinit ; 
      up = upinit ; 
      amount = amountinit ;
      transop = view ;
      sx = sy = 1.0 ; 
      tx = ty = 0.0 ; 
      break ;   
    case 'v': 
      transop = view ;
      std::cout << "Operation is set to View\n" ; 
      break ; 
    case 't':
      transop = translate ; 
      std::cout << "Operation is set to Translate\n" ; 
      break ; 
    case 's':
      transop = scale ; 
      std::cout << "Operation is set to Scale\n" ; 
      break ; 
  }
}

//  You will need to enter code for the arrow keys 
//  When an arrow key is pressed, it will call your transform functions

void specialKey(int key, int x, int y) {
  switch(key) {
    case 100: //left
      if (transop == view) Transform::left(amount, eye,  up);
      else if (transop == scale) sx -= amount * 0.01 ; 
      else if (transop == translate) tx -= amount * 0.01 ; 
      break;
    case 101: //up
      if (transop == view) Transform::up(amount,  eye,  up);
      else if (transop == scale) sy += amount * 0.01 ; 
      else if (transop == translate) ty += amount * 0.01 ; 
      break;
    case 102: //right
      if (transop == view) Transform::left(-amount, eye,  up);
      else if (transop == scale) sx += amount * 0.01 ; 
      else if (transop == translate) tx += amount * 0.01 ; 
      break;
    case 103: //down
      if (transop == view) Transform::up(-amount,  eye,  up);
      else if (transop == scale) sy -= amount * 0.01 ; 
      else if (transop == translate) ty -= amount * 0.01 ; 
      break;
  }
}

void init() {
    maxdepth = 5;
    attenuation = vec3(1, 0, 0);
    ambient[0] = 0.2;
    ambient[1] = 0.2;
    ambient[2] = 0.2;
}

void transformvec4(const vec4& input, vec4& output)
{
    glm::vec4 outputvec = modelview * input;
    output.x = outputvec.x;
    output.y = outputvec.y;
    output.z = outputvec.z;
    output.w = outputvec.w;
}

void transformvec3(const vec3& input, vec3& output)
{
    glm::vec4 outputvec = modelview * glm::vec4(input, 1.0f);
    output.x = outputvec.x;
    output.y = outputvec.y;
    output.z = outputvec.z;
}

void Intersect(Ray* ray, vector<object*> objects, Intersection* result) {
    result->isHit = false;

    for (object* object : objects) {
        vec3 hitVector;
        vec3 hitNormal;
        bool found = false;
        vec3 transformedRayOrigin = vec3(object->transform_inverse * vec4(ray->origin, 1.0));
        vec3 transformedRayDirection = vec3(object->transform_inverse * vec4(ray->direction, 0.0));

        if (object->type == ShapeSphere) {
            Sphere* sphere = dynamic_cast<Sphere*>(object);

            float t;
            vec3 p0 = transformedRayOrigin;
            vec3 p1 = transformedRayDirection;
            float a = glm::dot(p1, p1);
            //todo: add rotation transformation
           // vec3 temp;
           // transformvec3(tempVec, temp);
          //  vec3 center = glm::vec3(object->transform * glm::vec4(temp, 1.0f));
            vec3 center2 = sphere->center; // world coords
           // vec3 center2 = sphere->center;
            float b = 2 * glm::dot(p1, (p0 - center2));
            float radius = sphere->radius;
            float c = glm::dot((p0 - center2), (p0 - center2)) - radius * radius;
            float D = b * b - 4 * a * c;
            //cout << "D = " << D << endl;
            if (D > 0) {
                // two roots
                float sqrtD = glm::sqrt(D);
                float root1 = (-b + sqrtD) / (2 * a);
                float root2 = (-b - sqrtD) / (2 * a);
                if (root1 > 0 && root2 > 0) {
                    t = glm::min(root1, root2);
                    found = true;
                }
                else if (root2 < 0 && root1 >= 0) {
                    t = root1;
                    found = true;
                }
                else {
                    // should not happen, implies sthat both roots are negative
                }
            }
            else if (D == 0) {
                // one root
                float root = -b / (2 * a);
                t = root;
                found = true;
            }
            else if (D < 0) {
                // no roots
               // continue;
            }
            if (found) {
                //result->isHit = true;
                //result->object = object;
                //vec4 cent;
                //transformvec4(object->transform * glm::vec4(0, 0, 0, 1), cent);
                //cent.x += object->size;
                //result->hitVector = glm::vec3(cent); // hit vector in camera coords

               // vec3 normal;
               ////vec3 cent2;
               // //transformvec3(glm::vec3(object->transform * glm::vec4(center, 1.0f)), center); // center is sphere center in camera coords
               // normal = glm::normalize(result->hitVector - center); // normal vector in camera coords
               // result->hitNormal.x = normal.x;
               // result->hitNormal.y = normal.y;
               // result->hitNormal.z = normal.z;
               // result->isHit = true;
                hitVector = p0 + p1 * t;
                hitNormal = glm::normalize(hitVector - center2);




                /*vec3 res;
                transformvec3(p0 + p1 * t, res);
                result->hitVector.x = res.x;
                result->hitVector.y = res.y;
                result->hitVector.z = res.z;
                result->isHit = true;
                result->object = object;
                vec3 normal;
                transformvec3(glm::normalize(result->hitVector - center), normal);
                result->hitNormal.x = normal.x;
                result->hitNormal.y = normal.y;
                result->hitNormal.z = normal.z;*/
            }
        }
        else if (object->type == ShapeTriangle) {
            Triangle* triangle = dynamic_cast<Triangle*>(object);
            if (!triangle->hasNormals) {
                // vertices in camera coords
                vec3 vertex1 = *vertices[triangle->index1];//vec3(object->transform * vec4(*vertices[triangle->index1], 1.0f)); 
                vec3 vertex2 = *vertices[triangle->index2];//vec3(object->transform * vec4(*vertices[triangle->index2], 1.0f));
                vec3 vertex3 = *vertices[triangle->index3];//vec3(object->transform * vec4(*vertices[triangle->index3], 1.0f));

                vec3 N = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
                float D = -glm::dot(N, vertex1);
                float m = glm::dot(N, transformedRayDirection);
                if (m == 0) {
                    // no intersection because ray parallel to plane
                }
                else {
                    float t = -(glm::dot(N, transformedRayOrigin) + D) / m;
                    if (t < 0) {
                        // no intersection because ray goes away from triange plane
                    }
                    else {
                        vec3 Phit = transformedRayOrigin + t * transformedRayDirection;
                        vec3 edge1 = vertex2 - vertex1;
                        vec3 edge2 = vertex3 - vertex2;
                        vec3 edge3 = vertex1 - vertex3;
                        vec3 c1 = Phit - vertex1;
                        vec3 c2 = Phit - vertex2;
                        vec3 c3 = Phit - vertex3;
                        //cout << "dot 1 = " << glm::dot(N, glm::cross(edge1, c1)) << endl;
                        //cout << "dot 2 = " << glm::dot(N, glm::cross(edge2, c2)) << endl;
                        //cout << "dot 3 = " << glm::dot(N, glm::cross(edge3, c3)) << endl;
                        if (glm::dot(N, glm::cross(edge1, c1)) >= 0
                                && glm::dot(N, glm::cross(edge2, c2)) >= 0
                                && glm::dot(N, glm::cross(edge3, c3)) >= 0) {
                            found = true;
                            hitVector = Phit;
                            hitNormal = N;
                        }
                    }
                }

            } // TODO: handle case with normals as well
        }

        if (found) {
            vec3 inverseTransformedHitVec = vec3(object->transform * vec4(hitVector, 1.0));
            vec3 inverseTransformedNormal = glm::normalize(object->normal_transform * hitNormal);// vec3(object->normal_transform* vec4(hitNormal, 0.0));
            vec3 oldHitVector = result->hitVector;
            if (!result->isHit || (glm::distance2(oldHitVector, ray->origin) > glm::distance2(inverseTransformedHitVec, ray->origin))) {
                result->hitVector = inverseTransformedHitVec; // camera coords
                result->isHit = true;
                result->object = object;
                result->rayDir = ray->direction;
                result->hitNormal = inverseTransformedNormal; //camera coords
            }
        }
    }
}

// returns result in world coordinates
Ray* RayThruPixel(Camera* camera, int x, int y) {
    const vec3 a = eye - camera->center;
    const vec3 b = up;
    const vec3 w = glm::normalize(a);
    const vec3 u = glm::normalize(glm::cross(b, w));
    const vec3 v = glm::cross(w, u);
    const float aspect = ((float)width) / height;
    float fovyrad = glm::radians(camera->fovy);
    const float fovx = 2 * atan(tan(fovyrad * 0.5) * aspect);
    const float alpha = tan(fovx * 0.5) * (x + 0.5 - (width * 0.5)) / (width * 0.5);
    const float beta = tan(fovyrad * 0.5) * ((height * 0.5) - y - 0.5) / (height * 0.5);

    // world coordinates
    return new Ray(/* origin= */ eye, /* direction= */ glm::normalize(alpha * u + beta * v - w));


    //const mat4 resultRotation(u.x, v.x, w.x, 0, u.y, v.y, w.y, 0, u.z, v.z, w.z, 0, 0, 0, 0, 1);
    //mat4 resultTranslation(1.0f);
    //resultTranslation[3] = vec4(-eye, 1);

    //// You will change this return call
    //return resultRotation * resultTranslation;
}

vec4* lighttemp = new vec4();

inline float getAttenuation(float intensity, float distance, vec3 attenuation) {
    return intensity / (attenuation.x + distance * attenuation.y + distance * distance * attenuation.z);
}

void getColor(vector<Light*> lights, vec4* result, Intersection* intersection, int depth) {
    vec4 color = glm::vec4();
 //   cout << "log 1: color default (0) value " << color.x << " " << color.y << " " << color.z << " " << color.w << endl;
    color.x  = intersection->object->ambient[0] + intersection->object->emission[0];
    color.y = intersection->object->ambient[1] + intersection->object->emission[1];
    color.z = intersection->object->ambient[2] + intersection->object->emission[2];

  //  cout << "log 2: color updated initial value " << color.x << " " << color.y << " " << color.z << " " << color.w << endl;
    vec3 diffuse = vec3(
        intersection->object->diffuse[0],
        intersection->object->diffuse[1],
        intersection->object->diffuse[2]);
    vec3 specular = vec3(
        intersection->object->specular[0],
        intersection->object->specular[1],
        intersection->object->specular[2]);

   /* vec3 hitVectorInCameraCoords = vec3(modelview * vec4(intersection->hitVector, 1.0f));
    vec3 hitNormalInCameraCoords = vec3(modelview * vec4(intersection->hitNormal, 1.0f));*/

    for (int i = 0; i < lights.size(); i++)  {
        Light* light = lights[i];
        // run a ray from contact point to light
        // if there are no intersections, add the light stuff

        vec3 hitVectorCameraView = vec3(modelview * vec4(intersection->hitVector, 1));
        vec3 hitNormalCameraView = glm::normalize(normal_modelview * intersection->hitNormal);
        
        //if (light->position.w != 0) {
            // point light
            vec3 lightDir;
            if (light->position.w != 0) {
               lightDir = glm::normalize(vec3(light->position) - intersection->hitVector);
            }
            else {
                lightDir = glm::normalize(vec3(light->position));
            }
            vec3 lightPos;
            Ray* ray = new Ray(intersection->hitVector + intersection->hitNormal * 0.001f, lightDir); // TODO add the hit vec offset
            Intersection* lightIntersection = new Intersection();
            Intersect(ray, objects, lightIntersection);
            if (lightIntersection->isHit
                    && (light->position.w == 0 || glm::distance2(intersection->hitVector, vec3(light->position)) > glm::distance2(intersection->hitVector, lightIntersection->hitVector))) {
                continue;
            }
        //}


        //vec3 lightDir = glm::vec3(1, 0, 0);
       // vec3 lightDir = glm::normalize(vec3(light->position) - intersection->hitVector);
        
      //  vec3 test2 = normalize(vec3(light->lighttransf));
       // vec3 test = normalize(vec3(modelview * vec4(intersection->hitNormal, 1.0f)));
        vec3 lightDirCameraView;
        if (light->position.w != 0) {
            // point light
            lightDirCameraView = glm::normalize(vec3(light->lighttransf) - hitVectorCameraView);
        }
        else {
            // directional light
            lightDirCameraView = glm::normalize(vec3(light->lighttransf));
        }
        //vec3 halfAngle = normalize(glm::normalize(-hitVectorCameraView) + lightDirCameraView);
        vec3 halfAngle = normalize(normalize(vec3(modelview * vec4(-intersection->rayDir, 0.0))) + lightDirCameraView);
        float attenuation = getAttenuation(light->intensity, glm::distance(vec3(light->lighttransf), hitVectorCameraView), light->attenuation);
        float dot = glm::dot(hitNormalCameraView, lightDirCameraView);
        color +=
            vec4(vec3(light->color) *
                attenuation *
                (
                    diffuse
                    * glm::max(
                        dot, 0.0f)
//           ), 1.0);
                + 
                specular
                * glm::pow(glm::max(glm::dot(hitNormalCameraView, halfAngle), 0.0f), intersection->object->shininess)                  
                    
                    ), 1.0f);

//        color = vec4(1, 1, 1, 1);
      //  cout << "log 1.5: printing everything I possibly can: diffuse=[" << diffuse.x << " " << diffuse.y << " " << diffuse.z << " " << diffuse.w << "] " << "lightDir = [" << lightDir.x << " " << lightDir.y << " " << lightDir.z << "] hitNormal = [" << intersection->hitNormal.x << " " << intersection->hitNormal.y << " " << intersection->hitNormal.z << endl;
      //  cout << "log 2: color inside for loop value " << color.x << " " << color.y << " " << color.z << " " << color.w << endl;

    }

    if (depth > 0) {
        vec3 mirrorDirection = intersection->rayDir - 2 * glm::dot(intersection->rayDir, intersection->hitNormal) * intersection->hitNormal;
        Ray* ray = new Ray(intersection->hitVector + intersection->hitNormal * 0.001f, mirrorDirection);
        Intersection* reflectionIntersection = new Intersection();
        Intersect(ray, objects, reflectionIntersection);
        if (reflectionIntersection->isHit) {
            vec4* result = new vec4();
            getColor(lights, result, reflectionIntersection, depth - 1);
            color += vec4(specular, 1.0) * (*result);
        }
    }

   // cout << "log 3: color ending value " << color.x << " " << color.y << " " << color.z << " " << color.w << endl;

    color.x = glm::min(color.x, 1.0f);
    color.y = glm::min(color.y, 1.0f);
    color.z = glm::min(color.z, 1.0f);
    color.w = 1;

    result->x = color.x;
    result->y = color.y;
    result->z = color.z;
    result->w = color.w;
}

unsigned char* Raytrace(Camera* camera, vector<object*> objects, int width, int height) {
    unsigned char* pixels = new unsigned char[3 * width * height];
    Intersection* intersection = new Intersection();
    int totalPixels = width * height;
    int count = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
    //int x = 321;
    //int y = 280;

        count++;
        int currentPercent = (int)round((double)(100 * count) / totalPixels);
        int lastPercent = (int)round((double)(100 * (count-1)) / totalPixels);
        if (currentPercent != lastPercent) {
            cout << "Progress: " << currentPercent << "%" << endl;
        }
        pixels[x * 3 + y * width * 3] = (unsigned char)0;
        pixels[x * 3 + y * width * 3 + 1] = (unsigned char)0;
        pixels[x * 3 + y * width * 3 + 2] = (unsigned char)0;
        Ray* ray = RayThruPixel(camera, x, y);
        Intersect(ray, objects, intersection);
        if (intersection->isHit) {
            // cout << "IS HIT!! " << isHit << endl;
        }
        if (intersection->isHit) {
            getColor(lights, lighttemp, intersection, maxdepth);
            pixels[x * 3 + y * width * 3] = (unsigned char)(lighttemp->x * 255);
            pixels[x * 3 + y * width * 3 + 1] = (unsigned char)(lighttemp->y * 255);
            pixels[x * 3 + y * width * 3 + 2] = (unsigned char)(lighttemp->z * 255);
        }
      }
    }
    delete intersection;
    return pixels;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "Usage: transforms scenefile [grader input (optional)]\n"; 
    exit(-1); 
  }

  FreeImage_Initialise();

  init();
  readfile(argv[1]) ; 

  printHelp();

  modelview = Transform::lookAt(eye, camera->center, up);
  normal_modelview = mat3(glm::transpose(glm::inverse(modelview)));

  for (int i = 0; i < lights.size(); i++) {
      transformvec4(lights[i]->position, lights[i]->lighttransf);
  }

  cout << "Raytracing..." << endl;

  unsigned char* pixels = Raytrace(camera, objects, width, height);
  
  cout << "Saving screenshot..." << endl;
  
  saveScreenshot(outputLocation.empty() ? "antoan_test1.png" : outputLocation, pixels, width, height);

  cout << "Screenshot saved." << endl;

  delete[] pixels;
  FreeImage_DeInitialise();
  return 0;
}
