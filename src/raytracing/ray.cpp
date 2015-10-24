#include <raytracing/ray.h>

Ray::Ray(const glm::vec3 &o, const glm::vec3 &d):
    origin(o),
    direction(glm::normalize(d)),
    transmitted_color(1,1,1)
{}

Ray::Ray(const glm::vec4 &o, const glm::vec4 &d):
    Ray(glm::vec3(o), glm::vec3(d))
{}

Ray::Ray(const Ray &r):
    Ray(r.origin, r.direction)
{
    transmitted_color = r.transmitted_color;
}

Ray Ray::GetTransformedCopy(const glm::mat4 &T) const
{
    Ray newRay = Ray(glm::vec3(0), glm::vec3(0));
    newRay.origin = glm::vec3(T * glm::vec4(origin[0], origin[1], origin[2], 1));
    newRay.direction = glm::vec3(T * glm::vec4(direction[0], direction[1], direction[2], 0));

    return newRay;
}
