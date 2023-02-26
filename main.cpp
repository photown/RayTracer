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
#include "kdtree/KdTree.h"
#include <chrono>


using namespace std;

#include "Const.h"
#include "Camera.h"
#include "Ray.h"
#include "Intersection.h"
#include "Raytracer.h"
#include "Light.h"


// Main variables in the program.  
#define MAINPROGRAM 
#include "readfile.h" // prototypes for readfile.cpp  
#include "main.h"
void display(void) ;  // prototype for display function.  

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

}

void transformvec4(Camera* camera, const vec4& input, vec4& output)
{
    glm::vec4 outputvec = camera->modelview * input;
    output.x = outputvec.x;
    output.y = outputvec.y;
    output.z = outputvec.z;
    output.w = outputvec.w;
}

unsigned long long timeSinceEpochMillisec() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "Usage: transforms scenefile [grader input (optional)]\n"; 
    exit(-1); 
  }

  FreeImage_Initialise();

  init();
  cout << "Reading input file..." << endl;
  Config config = readfile(argv[1]);
  World& world = config.world;

  for (int i = 0; i < world.lights->size(); i++) {
      transformvec4(world.camera, world.lights->at(i)->position, world.lights->at(i)->lighttransf);
  }

  unsigned char* pixels;
  unsigned long long totalMs = 0;
  int numRenders = 1;
  for (int i = 0; i < numRenders; i++) {
      auto start = timeSinceEpochMillisec();
      cout << "Constructing KD tree..." << endl;

      KdTreeNode* kdTreeRoot = KdTree::constructTree(*world.objects, /* depth= */ 0);

      cout << "Raytracing..." << endl;

      Raytracer raytracer = Raytracer();
      pixels = raytracer.Raytrace(world, *kdTreeRoot, config.width, config.height);

      unsigned long long renderTimeMs = (timeSinceEpochMillisec() - start);
      cout << "Raytracing done in " << renderTimeMs << " milliseconds." << endl;
      totalMs += renderTimeMs;
  }
  cout << "Average time to render is " << (totalMs / numRenders) << " milliseconds." << endl;
  cout << "Saving screenshot..." << endl;
  
  saveScreenshot(config.outputLocation.empty() ? "raytrace.png" : config.outputLocation, pixels, config.width, config.height);

  cout << "Screenshot saved." << endl;

  delete[] pixels;
  FreeImage_DeInitialise();
  return 0;
}
