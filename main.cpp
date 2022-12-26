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

using namespace std ; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  
#include "main.h"
void display(void) ;  // prototype for display function.  

bool allowGrader = false;
vec3 tempVec = vec3();
vec3 hitVec = vec3();

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

}

void transformvec(const float input[4], float output[4])
{
    glm::vec4 inputvec(input[0], input[1], input[2], input[3]);

    glm::vec4 outputvec = modelview * inputvec;

    output[0] = outputvec[0];

    output[1] = outputvec[1];

    output[2] = outputvec[2];

    output[3] = outputvec[3];
}

bool Intersect(Ray* ray, vector<object*> objects, vec3* result) {
    for (object* object : objects) {
        if (object->type == ShapeSphere) {
            float t;
            bool found = false;
            vec3 p0 = ray->origin;
            vec3 p1 = ray->direction;
            float a = glm::dot(p1, p1);
            tempVec.x = object->transform[3][0];
            tempVec.y = object->transform[3][1];
            tempVec.z = object->transform[3][2];
            vec3 center = tempVec;
            float b = 2 * glm::dot(p1, (p0 - center));
            float c = glm::dot((p0 - center), (p0 - center)) - object->size * object->size;
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
                vec3 res = (p0 + p1 * t);
                result->x = res.x;
                result->y = res.y;
                result->z = res.z;
                return true;
            }
        }
        else {
            // TODO: add triangles
        }
    }
    return false;
}

Ray* RayThruPixel(Camera* camera, int x, int y) {
    const vec3 a = eye - center;
    const vec3 b = up;
    const vec3 w = glm::normalize(a);
    const vec3 u = glm::normalize(glm::cross(b, w));
    const vec3 v = glm::cross(w, u);
    const float aspect = ((float)width) / height;
    const float fovx = 2 * atan(tan(camera->fovy * 0.5) * aspect);
    const float alpha = tan(fovx * 0.5) * (x - (width * 0.5)) / (width * 0.5);
    const float beta = tan(fovy * 0.5) * ((height * 0.5) - y) / (height * 0.5);

    return new Ray(/* origin= */ eye, /* direction= */ glm::normalize(alpha * u + beta * v - w));


    //const mat4 resultRotation(u.x, v.x, w.x, 0, u.y, v.y, w.y, 0, u.z, v.z, w.z, 0, 0, 0, 0, 1);
    //mat4 resultTranslation(1.0f);
    //resultTranslation[3] = vec4(-eye, 1);

    //// You will change this return call
    //return resultRotation * resultTranslation;
}

unsigned char* Raytrace(Camera* camera, vector<object*> objects, int width, int height) {
    unsigned char* pixels = new unsigned char[3 * width * height];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixels[x * 3 + y * width * 3] = (unsigned char) 0;
            pixels[x * 3 + y * width * 3 + 1] = (unsigned char) 0;
            pixels[x * 3 + y * width * 3 + 2] = (unsigned char) 0;
            Ray* ray = RayThruPixel(camera, x, y);
            bool isHit = Intersect(ray, objects, &hitVec);
            if (isHit) {
               // cout << "IS HIT!! " << isHit << endl;
            }
            if (isHit) {
                pixels[x * 3 + y * width * 3] = (unsigned char) 255;
                pixels[x * 3 + y * width * 3 + 1] = (unsigned char) 255;
                pixels[x * 3 + y * width * 3 + 2] = (unsigned char) 255;
            }
        }
    }
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

  modelview = Transform::lookAt(eye, center, up);
  for (int i = 0; i < numused; i++) {
      transformvec(&lightposn[i * 4], &lightransf[i * 4]);
  }
  cout << "WTF -------------------" << endl;
  unsigned char* pixels = Raytrace(camera, objects, width, height);

  saveScreenshot("antoan_test1.png", pixels, width, height);

  delete[] pixels;
  FreeImage_DeInitialise();
  return 0;
}
