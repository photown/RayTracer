#include "Raytracer.h"
#include <limits>
#include <glm/gtx/matrix_decompose.hpp>
#include<../AMath.h>
#include "kdtree/KdTree.h"

glm::vec3 scale;
glm::quat rotation;
glm::vec3 translation;
glm::vec3 skew;
glm::vec4 perspective;

unsigned char* Raytracer::Raytrace(World& world, KdTreeNode& kdTreeRoot, int width, int height) {
    unsigned char* pixels = new unsigned char[3 * width * height];
    Intersection intersection = Intersection();
    int totalPixels = width * height;
    int count = 0;
    vec4 lighttemp = vec4();
    int currentPercent;
    int lastPercent = -1;

    Box worldBoundingBox = Box(
        vec3(
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max()),
        vec3(
            std::numeric_limits<float>::min(),
            std::numeric_limits<float>::min(),
            std::numeric_limits<float>::min()));
    for (Object* object : *world.objects) {
        vec3 bottomLeftBackWorld = object->axisAlignedBoundingBox->bottomLeftBack;
        if (IsLessThan(bottomLeftBackWorld.x, worldBoundingBox.bottomLeftBack.x)) {
            worldBoundingBox.bottomLeftBack.x = bottomLeftBackWorld.x;
        }
        if (IsLessThan(bottomLeftBackWorld.y, worldBoundingBox.bottomLeftBack.y)) {
            worldBoundingBox.bottomLeftBack.y = bottomLeftBackWorld.y;
        }
        if (IsLessThan(bottomLeftBackWorld.z, worldBoundingBox.bottomLeftBack.z)) {
            worldBoundingBox.bottomLeftBack.z = bottomLeftBackWorld.z;
        }

        vec3 topRightFrontWorld = object->axisAlignedBoundingBox->topRightFront;
        if (IsGreaterThan(topRightFrontWorld.x, worldBoundingBox.topRightFront.x)) {
            worldBoundingBox.topRightFront.x = topRightFrontWorld.x;
        }
        if (IsGreaterThan(topRightFrontWorld.y, worldBoundingBox.topRightFront.y)) {
            worldBoundingBox.topRightFront.y = topRightFrontWorld.y;
        }
        if (IsGreaterThan(topRightFrontWorld.z, worldBoundingBox.topRightFront.z)) {
            worldBoundingBox.topRightFront.z = topRightFrontWorld.z;
        }
    }


    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
    //       int x = 10;
    //        int y = 10;

            count++;
            currentPercent = (int)round((double)(100 * count) / totalPixels);
            if (currentPercent != lastPercent) {
                std::cout << "Progress: " << currentPercent << "%" << std::endl;
            }
            lastPercent = currentPercent;
            pixels[x * 3 + y * width * 3] = (unsigned char)0;
            pixels[x * 3 + y * width * 3 + 1] = (unsigned char)0;
            pixels[x * 3 + y * width * 3 + 2] = (unsigned char)0;
            Ray ray = RayThruPixel(*world.camera, x, y, width, height);

            BoxIntersection boxIntersection = BoxIntersection();
            worldBoundingBox.Intersect(ray, boxIntersection);
            if (!boxIntersection.isHit) {
      //          continue;
            }
            else {
                float start = boxIntersection.tStart;
                float end = boxIntersection.tEnd;
                Intersect(ray, kdTreeRoot, start, end, *world.objects, intersection);
                if (intersection.isHit) {
                    getColor(world, lighttemp, intersection, world.maxdepth, kdTreeRoot, worldBoundingBox);
                    pixels[x * 3 + y * width * 3] = (unsigned char)(lighttemp.x * 255);
                    pixels[x * 3 + y * width * 3 + 1] = (unsigned char)(lighttemp.y * 255);
                    pixels[x * 3 + y * width * 3 + 2] = (unsigned char)(lighttemp.z * 255);
                }
            }
        }
    }
    return pixels;
}

// returns result in world coordinates
Ray Raytracer::RayThruPixel(Camera& camera, int x, int y, int width, int height) {
    const vec3 a = camera.eye - camera.center;
    const vec3 b = camera.up;
    const vec3 w = glm::normalize(a);
    const vec3 u = glm::normalize(glm::cross(b, w));
    const vec3 v = glm::cross(w, u);
    float fovyrad = glm::radians(camera.fovy);
    const float fovx = 2 * atan(tan(fovyrad * 0.5) * camera.aspectRatio);
    const float alpha = tan(fovx * 0.5) * (x + 0.5 - (width * 0.5)) / (width * 0.5);
    const float beta = tan(fovyrad * 0.5) * ((height * 0.5) - y - 0.5) / (height * 0.5);

    return Ray(/* origin= */ camera.eye, /* direction= */ glm::normalize(alpha * u + beta * v - w));
}

float Raytracer::getAttenuation(float intensity, float distance, vec3& attenuationComponents) {
    return intensity / (attenuationComponents.x + distance * attenuationComponents.y + distance * distance * attenuationComponents.z);
}

void Raytracer::Intersect(Ray& ray, KdTreeNode& kdTreeRoot, float tStart, float tEnd, std::vector<Object*>& objects, Intersection& result) {
    result.isHit = false;

    Ray rayInObjectSpace = Ray(vec3(), vec3());
    Intersection resultIntersection = Intersection();

    std::vector<Object*> intersectionCandidates;
    KdTree::traverse(kdTreeRoot, ray, intersectionCandidates);
    
    for (Object* object : intersectionCandidates) {
    //for (Object* object : objects) {
        vec3 transformedRayOrigin = vec3(object->transform_inverse * vec4(ray.origin, 1.0));
        vec3 transformedRayDirection = vec3(object->transform_inverse * vec4(ray.direction, 0.0));

        rayInObjectSpace.origin = transformedRayOrigin;
        rayInObjectSpace.direction = transformedRayDirection;
        resultIntersection.isHit = false;

        object->Intersect(rayInObjectSpace, (*object), resultIntersection);
        
        if (resultIntersection.isHit) {
            vec3 inverseTransformedHitVec = vec3(object->transform * vec4(resultIntersection.hitVector, 1.0));
            vec3 inverseTransformedNormal = glm::normalize(object->normal_transform * resultIntersection.hitNormal);
            vec3 oldHitVector = result.hitVector;
            if (!result.isHit || (glm::distance2(oldHitVector, ray.origin) > glm::distance2(inverseTransformedHitVec, ray.origin))) {
                result.hitVector = inverseTransformedHitVec; // camera coords
                result.hitNormal = inverseTransformedNormal; //camera coords
                result.isHit = true;
                result.object = object;
                result.rayDir = ray.direction;
            }
        }
    }
}

void Raytracer::getColor(World& world, vec4& result, Intersection& intersection, int depth, KdTreeNode& kdTreeRoot, Box& worldBoundingBox) {
    vec4 color = glm::vec4();
    color.x = intersection.object->ambient[0] + intersection.object->emission[0];
    color.y = intersection.object->ambient[1] + intersection.object->emission[1];
    color.z = intersection.object->ambient[2] + intersection.object->emission[2];

    vec3 diffuse = vec3(
        intersection.object->diffuse[0],
        intersection.object->diffuse[1],
        intersection.object->diffuse[2]);
    vec3 specular = vec3(
        intersection.object->specular[0],
        intersection.object->specular[1],
        intersection.object->specular[2]);

    vec3 hitVectorCameraView = vec3(world.camera->modelview * vec4(intersection.hitVector, 1));
    vec3 hitNormalCameraView = glm::normalize(world.camera->normal_modelview * intersection.hitNormal);
    Intersection lightIntersection = Intersection();
    Ray lightRay = Ray(vec3(), vec3());

    for (int i = 0; i < world.lights->size(); i++) {
        Light* light = world.lights->at(i);
        vec3 lightDir;
        if (light->position.w != 0) {
            // point light
            lightDir = glm::normalize(vec3(light->position) - intersection.hitVector);
        }
        else {
            // directional light
            lightDir = glm::normalize(vec3(light->position));
        }
        vec3 lightPos;
        lightRay.origin = intersection.hitVector + intersection.hitNormal * 0.0001f;
        lightRay.direction = lightDir;
        lightIntersection.isHit = false;


        BoxIntersection boxIntersection;
        worldBoundingBox.Intersect(lightRay, boxIntersection);
        if (!boxIntersection.isHit) {
            continue;
        }

        Intersect(lightRay, kdTreeRoot, boxIntersection.tStart, boxIntersection.tEnd, *world.objects, lightIntersection);
        float dist1 = glm::distance2(intersection.hitVector, vec3(light->position));
        float dist2 = glm::distance2(intersection.hitVector, lightIntersection.hitVector);
        if (lightIntersection.isHit
            && (light->position.w == 0 || IsGreaterThan(dist1, dist2))) {
            continue;
        }

        vec3 lightDirCameraView;
        if (light->position.w != 0) {
            // point light
            lightDirCameraView = glm::normalize(vec3(light->lighttransf) - hitVectorCameraView);
        }
        else {
            // directional light
            lightDirCameraView = glm::normalize(vec3(light->lighttransf));
        }
        vec3 halfAngle = normalize(normalize(vec3(world.camera->modelview * vec4(-intersection.rayDir, 0.0))) + lightDirCameraView);
        float attenuation = getAttenuation(light->intensity, glm::distance(vec3(light->lighttransf), hitVectorCameraView), light->attenuation);
        float dot = glm::dot(hitNormalCameraView, lightDirCameraView);
        color +=
            vec4(vec3(light->color) *
                attenuation *
                (
                    diffuse
                    * glm::max(
                        dot, 0.0f)
                    +
                    specular
                    * glm::pow(glm::max(glm::dot(hitNormalCameraView, halfAngle), 0.0f), intersection.object->shininess)

                    ), 1.0f);
    }

    if (depth > 0) {
        vec3 mirrorDirection = intersection.rayDir - 2 * glm::dot(intersection.rayDir, intersection.hitNormal) * intersection.hitNormal;
        Ray ray = Ray(intersection.hitVector + intersection.hitNormal * 0.0001f, mirrorDirection);
        Intersection reflectionIntersection = Intersection();

        BoxIntersection boxIntersection;
        worldBoundingBox.Intersect(ray, boxIntersection);
        if (boxIntersection.isHit) {
            Intersect(ray, kdTreeRoot, boxIntersection.tStart, boxIntersection.tEnd, *world.objects, reflectionIntersection);
            if (reflectionIntersection.isHit) {
                vec4 result = vec4();
                getColor(world, result, reflectionIntersection, depth - 1, kdTreeRoot, worldBoundingBox);
                color += vec4(specular, 1.0) * result;
            }
        }
    }

    color.x = glm::min(color.x, 1.0f);
    color.y = glm::min(color.y, 1.0f);
    color.z = glm::min(color.z, 1.0f);
    color.w = 1;

    result.x = color.x;
    result.y = color.y;
    result.z = color.z;
    result.w = color.w;
}