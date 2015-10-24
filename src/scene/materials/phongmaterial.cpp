#include <scene/materials/phongmaterial.h>

PhongMaterial::PhongMaterial():
    PhongMaterial(glm::vec3(0.5f, 0.5f, 0.5f))
{}

PhongMaterial::PhongMaterial(const glm::vec3 &color):
    Material(color),
    specular_power(10)
{}

glm::vec3 PhongMaterial::EvaluateReflectedEnergy(const Intersection &isx, const Ray &outgoing_ray, const Ray &incoming_ray)
{
    glm::vec3 ambient_color = glm::vec3(0.5f,.5f,.5f);
    glm::vec3 norm = glm::normalize(isx.normal);
    // incoming_ray = light ray
    glm::vec3 light_color = incoming_ray.transmitted_color;
    glm::vec3 light_ray = glm::normalize(incoming_ray.direction);
    // outgoing_ray = ray to camera

    float brightness_val = glm::clamp(glm::dot(norm, light_ray), 0.0f, 1.0f);
    glm::vec3 diffuse_color = this->base_color * (brightness_val + ambient_color) * light_color;

    //calculate the specular highlights
    glm::vec3 H;
    H[0]= (incoming_ray.direction[0]-outgoing_ray.direction[0])/2;
    H[1]= (incoming_ray.direction[1]-outgoing_ray.direction[1])/2;
    H[2]= (incoming_ray.direction[2]-outgoing_ray.direction[2])/2;

    glm::vec3 specular_color = glm::max(glm::pow(glm::dot(H, norm), specular_power), 0.0f) * glm::vec3(1.0f,1.0f,1.0f);

    return .1f*ambient_color + .2f*diffuse_color + .7f*specular_color;
}
