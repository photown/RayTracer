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
#include <cmath>

using namespace std;

#include "Const.h"
#include "Camera.h"
#include "Ray.h"
#include "Intersection.h"
#include "Raytracer.h"



// Main variables in the program.  
#define MAINPROGRAM 
#include "readfile.h" // prototypes for readfile.cpp  
#include "variables.h" 
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

void init() {
    maxdepth = 5;
    attenuation = vec3(1, 0, 0);
    ambient[0] = 0.2;
    ambient[1] = 0.2;
    ambient[2] = 0.2;
}

void transformvec4(const vec4& input, vec4& output)
{
    glm::vec4 outputvec = camera->modelview * input;
    output.x = outputvec.x;
    output.y = outputvec.y;
    output.z = outputvec.z;
    output.w = outputvec.w;
}

void transformvec3(const vec3& input, vec3& output)
{
    glm::vec4 outputvec = camera->modelview * glm::vec4(input, 1.0f);
    output.x = outputvec.x;
    output.y = outputvec.y;
    output.z = outputvec.z;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "Usage: transforms scenefile [grader input (optional)]\n"; 
    exit(-1); 
  }

  FreeImage_Initialise();

  init();
  readfile(argv[1]);   

  for (int i = 0; i < lights.size(); i++) {
      transformvec4(lights[i]->position, lights[i]->lighttransf);
  }

  cout << "Raytracing..." << endl;

  Raytracer raytracer = Raytracer();
  unsigned char* pixels = raytracer.Raytrace(camera, objects, width, height);
  
  cout << "Saving screenshot..." << endl;
  
  saveScreenshot(outputLocation.empty() ? "antoan_test1.png" : outputLocation, pixels, width, height);

  cout << "Screenshot saved." << endl;

  delete[] pixels;
  FreeImage_DeInitialise();
  return 0;
}
