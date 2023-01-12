#include "Raytracer.h"

unsigned char* Raytracer::Raytrace(Camera* camera, std::vector<Object*> objects, int width, int height) {
    unsigned char* pixels = new unsigned char[3 * width * height];
    Intersection intersection = Intersection();
    int totalPixels = width * height;
    int count = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            //int x = 321;
            //int y = 280;

            count++;
            int currentPercent = (int)round((double)(100 * count) / totalPixels);
            int lastPercent = (int)round((double)(100 * (count - 1)) / totalPixels);
            if (currentPercent != lastPercent) {
                std::cout << "Progress: " << currentPercent << "%" << std::endl;
            }
            pixels[x * 3 + y * width * 3] = (unsigned char)0;
            pixels[x * 3 + y * width * 3 + 1] = (unsigned char)0;
            pixels[x * 3 + y * width * 3 + 2] = (unsigned char)0;
            Ray* ray = RayThruPixel(camera, x, y);
            Intersect(ray, objects, &intersection);
            if (intersection.isHit) {
                getColor(camera, objects, lights, lighttemp, &intersection, maxdepth);
                pixels[x * 3 + y * width * 3] = (unsigned char)(lighttemp->x * 255);
                pixels[x * 3 + y * width * 3 + 1] = (unsigned char)(lighttemp->y * 255);
                pixels[x * 3 + y * width * 3 + 2] = (unsigned char)(lighttemp->z * 255);
            }
        }
    }
    return pixels;
}

// returns result in world coordinates
Ray* Raytracer::RayThruPixel(Camera* camera, int x, int y) {
    const vec3 a = camera->eye - camera->center;
    const vec3 b = camera->up;
    const vec3 w = glm::normalize(a);
    const vec3 u = glm::normalize(glm::cross(b, w));
    const vec3 v = glm::cross(w, u);
    const float aspect = ((float)width) / height;
    float fovyrad = glm::radians(camera->fovy);
    const float fovx = 2 * atan(tan(fovyrad * 0.5) * aspect);
    const float alpha = tan(fovx * 0.5) * (x + 0.5 - (width * 0.5)) / (width * 0.5);
    const float beta = tan(fovyrad * 0.5) * ((height * 0.5) - y - 0.5) / (height * 0.5);

    return new Ray(/* origin= */ camera->eye, /* direction= */ glm::normalize(alpha * u + beta * v - w));
}

float Raytracer::getAttenuation(float intensity, float distance, vec3 attenuation) {
    return intensity / (attenuation.x + distance * attenuation.y + distance * distance * attenuation.z);
}

void Raytracer::Intersect(Ray* ray, std::vector<Object*> objects, Intersection* result) {
    result->isHit = false;

    Ray rayInObjectSpace = Ray(vec3(), vec3());
    Intersection resultIntersection = Intersection();

    for (Object* object : objects) {
        vec3 transformedRayOrigin = vec3(object->transform_inverse * vec4(ray->origin, 1.0));
        vec3 transformedRayDirection = vec3(object->transform_inverse * vec4(ray->direction, 0.0));

        rayInObjectSpace.origin = transformedRayOrigin;
        rayInObjectSpace.direction = transformedRayDirection;
        resultIntersection.isHit = false;

        object->Intersect(&rayInObjectSpace, object, &resultIntersection);

        if (resultIntersection.isHit) {
            vec3 inverseTransformedHitVec = vec3(object->transform * vec4(resultIntersection.hitVector, 1.0));
            vec3 inverseTransformedNormal = glm::normalize(object->normal_transform * resultIntersection.hitNormal);
            vec3 oldHitVector = result->hitVector;
            if (!result->isHit || (glm::distance2(oldHitVector, ray->origin) > glm::distance2(inverseTransformedHitVec, ray->origin))) {
                result->hitVector = inverseTransformedHitVec; // camera coords
                result->hitNormal = inverseTransformedNormal; //camera coords
                result->isHit = true;
                result->object = object;
                result->rayDir = ray->direction;
            }
        }
    }
}

void Raytracer::getColor(Camera* camera, std::vector<Object*> objects, std::vector<Light*> lights, vec4* result, Intersection* intersection, int depth) {
    vec4 color = glm::vec4();
    color.x = intersection->object->ambient[0] + intersection->object->emission[0];
    color.y = intersection->object->ambient[1] + intersection->object->emission[1];
    color.z = intersection->object->ambient[2] + intersection->object->emission[2];

    vec3 diffuse = vec3(
        intersection->object->diffuse[0],
        intersection->object->diffuse[1],
        intersection->object->diffuse[2]);
    vec3 specular = vec3(
        intersection->object->specular[0],
        intersection->object->specular[1],
        intersection->object->specular[2]);

    vec3 hitVectorCameraView = vec3(camera->modelview * vec4(intersection->hitVector, 1));
    vec3 hitNormalCameraView = glm::normalize(camera->normal_modelview * intersection->hitNormal);

    for (int i = 0; i < lights.size(); i++) {
        Light* light = lights[i];
        vec3 lightDir;
        if (light->position.w != 0) {
            // point light
            lightDir = glm::normalize(vec3(light->position) - intersection->hitVector);
        }
        else {
            // directional light
            lightDir = glm::normalize(vec3(light->position));
        }
        vec3 lightPos;
        Ray* ray = new Ray(intersection->hitVector + intersection->hitNormal * 0.0001f, lightDir); // TODO add the hit vec offset
        Intersection* lightIntersection = new Intersection();
        Intersect(ray, objects, lightIntersection);
        if (lightIntersection->isHit
            && (light->position.w == 0 || glm::distance2(intersection->hitVector, vec3(light->position)) > glm::distance2(intersection->hitVector, lightIntersection->hitVector))) {
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
        vec3 halfAngle = normalize(normalize(vec3(camera->modelview * vec4(-intersection->rayDir, 0.0))) + lightDirCameraView);
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
                    * glm::pow(glm::max(glm::dot(hitNormalCameraView, halfAngle), 0.0f), intersection->object->shininess)

                    ), 1.0f);
    }

    if (depth > 0) {
        vec3 mirrorDirection = intersection->rayDir - 2 * glm::dot(intersection->rayDir, intersection->hitNormal) * intersection->hitNormal;
        Ray* ray = new Ray(intersection->hitVector + intersection->hitNormal * 0.0001f, mirrorDirection);
        Intersection* reflectionIntersection = new Intersection();
        Intersect(ray, objects, reflectionIntersection);
        if (reflectionIntersection->isHit) {
            vec4* result = new vec4();
            getColor(camera, objects, lights, result, reflectionIntersection, depth - 1);
            color += vec4(specular, 1.0) * (*result);
        }
    }

    color.x = glm::min(color.x, 1.0f);
    color.y = glm::min(color.y, 1.0f);
    color.z = glm::min(color.z, 1.0f);
    color.w = 1;

    result->x = color.x;
    result->y = color.y;
    result->z = color.z;
    result->w = color.w;
}