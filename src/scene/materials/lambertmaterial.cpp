#include <scene/materials/lambertmaterial.h>

LambertMaterial::LambertMaterial():Material()
{}

LambertMaterial::LambertMaterial(const glm::vec3 &color):Material(color)
{}

glm::vec3 LambertMaterial::EvaluateReflectedEnergy(const Intersection &isx, const Ray &outgoing_ray, const Ray &incoming_ray)
{
    glm::vec3 norm = glm::normalize(isx.normal);
    // incoming_ray = light ray
    glm::vec3 light_ray = glm::normalize(incoming_ray.direction);
    // outgoing_ray = ray to camera

    float brightness_val = glm::clamp(glm::dot(norm, light_ray), 0.0f, 1.0f);

    glm::vec3 diffuse_color = this->base_color * brightness_val * incoming_ray.transmitted_color;

    return diffuse_color;
}
