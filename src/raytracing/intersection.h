#pragma once
#include <la.h>
#include <scene/geometry/geometry.h>
#include <raytracing/ray.h>
#include <scene/scene.h>
#include <raytracing/BVHnode.h>

class Material;
class Geometry;
class Scene;

class Intersection
{
public:
    Intersection();

    glm::vec3 point;      //The place at which the intersection occurred
    glm::vec3 normal;     //The surface normal at the point of intersection
    float t;              //The parameterization for the ray (in world space) that generated this intersection.
                          //t is equal to the distance from the point of intersection to the ray's origin if the ray's direction is normalized.
    Geometry* object_hit; //The object that the ray intersected, or NULL if the ray hit nothing.
    glm::vec3 hit_color; //color of object at point of intersection on the surface
};

class IntersectionEngine
{
public:
    IntersectionEngine();
    Scene *scene;

    BVHnode* BVHrootNode; //pointer to root of bvh tree

    Intersection GetIntersection(Ray r);
};
