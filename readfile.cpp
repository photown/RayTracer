/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  


// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 

/*****************************************************************************/

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h" 
//#include "Camera.h"
#include "objects/Triangle.h"
#include "objects/Sphere.h"
#include "objects/Object.h"

using namespace std;
#include "variables.h" 
#include "readfile.h"

struct Camera;

// You may not need to use the following two functions, but it is provided
// here for convenience

// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4>& transfstack, float* values)
{
    mat4 transform = transfstack.top();
    vec4 valvec = vec4(values[0], values[1], values[2], values[3]);
    vec4 newval = transform * valvec;
    for (int i = 0; i < 4; i++) values[i] = newval[i];
}

void rightmultiply(const mat4& M, stack<mat4>& transfstack)
{
    mat4& T = transfstack.top();
    T = T * M;
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream& s, const int numvals, float* values)
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

std::string vecToString(vec3 &vec) {
    return "vec(x=" + std::to_string(vec.x) + ", y=" + std::to_string(vec.y) + ", z=" + std::to_string(vec.z) + ")";
}

void setupCommonObjectProperties(Object* obj, mat4& transform, Shape shape) {
    // Set the object's light properties
    for (int i = 0; i < 3; i++) {
        (obj->ambient)[i] = ambient[i];
        (obj->diffuse)[i] = diffuse[i];
        (obj->specular)[i] = specular[i];
        (obj->emission)[i] = emission[i];
    }
    obj->shininess = shininess;
    obj->transform = transform;
    obj->normal_transform = mat3(glm::transpose(glm::inverse(transform)));
    obj->transform_inverse = glm::inverse(transform);
    obj->type = shape;
}

void readfile(const char* filename)
{
    string str, cmd, strval;
    ifstream in;
    in.open(filename);
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        stack <mat4> transfstack;
        transfstack.push(mat4(1.0));  // identity

        getline(in, str);
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd;
                int i;
                float values[10]; // Position and color for light, colors for others
                // Up to 10 params for cameras.  
                bool validinput; // Validity of input 

                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "light") {
                    validinput = readvals(s, 8, values); // Position/color for lts.
                    if (validinput) {
                        Light* light = new Light();
                        light->position = vec4(values[0], values[1], values[2], values[3]);
                        light->color = vec4(values[4], values[5], values[6], values[7]);
                        light->attenuation = vec3(attenuation);
                        lights.push_back(light);
                    }
                }

                // Material Commands 
                // Ambient, diffuse, specular, shininess properties for each object.
                // Filling this in is pretty straightforward, so I've left it in 
                // the skeleton, also as a hint of how to do the more complex ones.
                // Note that no transforms/stacks are applied to the colors. 

                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            ambient[i] = values[i];
                        }
                    }
                }
                else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            diffuse[i] = values[i];
                        }
                    }
                }
                else if (cmd == "specular") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            specular[i] = values[i];
                        }
                    }
                }
                else if (cmd == "emission") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            emission[i] = values[i];
                        }
                    }
                }
                else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        shininess = values[0];
                    }
                }
                else if (cmd == "size") {
                    validinput = readvals(s, 2, values);
                    if (validinput) {
                        width = (int)values[0]; height = (int)values[1];
                    }
                }
                else if (cmd == "camera") {
                    validinput = readvals(s, 10, values); // 10 values eye cen up fov
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE
                        // Use all of values[0...9]
                        // You may need to use the upvector fn in Transform.cpp
                        // to set up correctly. 
                        // Set eyeinit upinit center fovy in variables.h 
                        vec3 eye(values[0], values[1], values[2]);
                        vec3 cen(values[3], values[4], values[5]);
                        vec3 up(values[6], values[7], values[8]);
                        float fov = values[9];
                        up = Transform::upvector(up, eye - cen);
                        camera = new Camera(eye, up, cen, fov);
                    }
                }

                // I've left the code for loading objects in the skeleton, so 
                // you can get a sense of how this works.  
                // Also look at demo.txt to get a sense of why things are done this way.
                else if (cmd == "sphere") {
                    validinput = readvals(s, 4, values);
                    if (validinput) {
                        Sphere* sphere = new Sphere();
                        sphere->center = vec3(values[0], values[1], values[2]);
                        sphere->radius = values[3];                            
                        setupCommonObjectProperties(sphere, transfstack.top(), ShapeSphere);
                        objects.push_back(sphere);
                    }
                }

                else if (cmd == "translate") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE.  
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file. 
                        // Also keep in mind what order your matrix is!
                        float vec[4] = { values[0], values[1], values[2], 0 };
                        //  matransform(transfstack, vec);
                        rightmultiply(Transform::translate(vec[0], vec[1], vec[2]), transfstack);
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE.  
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.  
                        // Also keep in mind what order your matrix is!
                        //vec3 scale(values[0], values[1], values[2]);
                        float vec[4] = { values[0], values[1], values[2], 0 };
                        //  matransform(transfstack, vec);
                        rightmultiply(Transform::scale(vec[0], vec[1], vec[2]), transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s, 4, values);
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE. 
                        // values[0..2] are the axis, values[3] is the angle.  
                        // You may want to normalize the axis (or in Transform::rotate)
                        // See how the stack is affected, as above.  
                        // Note that rotate returns a mat3. 
                        // Also keep in mind what order your matrix is!
                        float vec[4] = { values[0], values[1], values[2], 0 };
                        // matransform(transfstack, vec);
                        vec3 axis = normalize(vec3(vec[0], vec[1], vec[2]));
                        float angle = values[3];
                        mat3 rot = Transform::rotate(angle, axis);
                        mat4 m4(1.0f);
                        m4[0] = vec4(rot[0], 0.0f);
                        m4[1] = vec4(rot[1], 0.0f);
                        m4[2] = vec4(rot[2], 0.0f);
                        rightmultiply(m4, transfstack);
                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                }
                else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n";
                    }
                    else {
                        transfstack.pop();
                    }
                }
                else if (cmd == "maxdepth") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        maxdepth = (int) values[0];
                    }
                }
                else if (cmd == "output") {
                    s >> strval;
                    cout << "antoan output =" << strval << endl;
                    outputLocation = strval;
                }
                else if (cmd == "vertex") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        vertices.push_back(new vec3(values[0], values[1], values[2]));
                    }
                }
                else if (cmd == "vertexnormal") {
                    validinput = readvals(s, 6, values);
                    if (validinput) {
                        vertexnormals.push_back(new pair<vec3*, vec3*>(
                            new vec3(values[0], values[1], values[2]), new vec3(values[3], values[4], values[5])));
                    }
                }
                else if (cmd == "tri") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        Triangle* triangle = new Triangle(values[0], values[1], values[2], /* hasNormals= */ false);
                        setupCommonObjectProperties(triangle, transfstack.top(), ShapeTriangle);
                        objects.push_back(triangle);
                    }
                }
                else if (cmd == "trinormal") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        Triangle* triangle = new Triangle(values[0], values[1], values[2], /* hasNormals= */ true);
                        setupCommonObjectProperties(triangle, transfstack.top(), ShapeTriangle);
                        objects.push_back(triangle);
                    }
                }
                else if (cmd == "attenuation") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        attenuation = vec3(values[0], values[1], values[2]);
                    }
                }
                else if (cmd == "directional") {
                /*
                It should be treated as directional (distant) if w = 0 and as a point light in homogeneous coordinates otherwise. */
                    validinput = readvals(s, 6, values);
                    if (validinput) {
                        Light* light = new Light();
                        light->position = vec4(values[0], values[1], values[2], 0.0f);
                        light->attenuation = vec3(1, 0, 0);
                        light->intensity = 1;
                        light->color = vec4(values[3], values[4], values[5], 1.0f);
                        lights.push_back(light);
                    }
                }
                else if (cmd == "point") {
                    validinput = readvals(s, 6, values);
                    if (validinput) {
                        Light* light = new Light();
                        light->position = vec4(values[0], values[1], values[2], 1.0f);
                        light->attenuation = vec3(attenuation);
                        light->intensity = 1;
                        light->color = vec4(values[3], values[4], values[5], 1.0f);
                        lights.push_back(light);
                    }
                }
                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }
            getline(in, str);
        }

        // Set up initial position for eye, up and amount
        // As well as booleans 
    }
    else {
        cerr << "Unable to Open Input Data File " << filename << "\n";
        throw 2;
    }
}
