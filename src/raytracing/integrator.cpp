#include <raytracing/integrator.h>


Integrator::Integrator():
    max_depth(5)
{
    scene = NULL;
    intersection_engine = NULL;
}



// Helper function that helps determine if a light has been intersection
// returns NULL if no light intersected, and the Geometry* light if a light has been intersected
// (ignoring other objects in scene)
//void ComputeShading(Ray r, )


//Basic ray trace
glm::vec3 Integrator::TraceRay(Ray r, unsigned int depth)
{
    glm::vec3 ambient_light = glm::vec3(0.05f);
    glm::vec3 pxl_color = glm::vec3(1.0f,1.0f,1.0f);

    if (depth >= max_depth) {
        return glm::vec3(0.0f);
    }

    //cast ray from cam to pixel (ray given, r)
    //compute *nearest* pt of intersection in world space

    Intersection pt = intersection_engine->GetIntersection(r); //traverses bvh tree to determine pt hit here

    //if no object is hit by this ray, just set color to ambient light color
    if (pt.object_hit == NULL) {
        pxl_color =  ambient_light;
    } else {
        //otherwise, if it did hit something, offset the found intersection by 1E-6 along normal to prevent SHADOW ACNE
        pt.point = pt.point + pt.normal*0.0001f;

        //get object material
        Material *obj_mat = pt.object_hit->material;

        glm::vec3 general_color;
        glm::vec3 mirror_reflected_color;

        //go through each light and compound their effect on the intersected point:
        for (int i = 0; i < this->scene->lights.count(); i++) {
            glm::vec3 light_pos = this->scene->lights[i]->transform.position();
            glm::vec3 LF_dir = glm::normalize(light_pos - pt.point);
            Ray light_feeler = Ray(pt.point, LF_dir);

            //calculate light color
            glm::vec3 light_color = this->scene->lights[i]->material->base_color;
            light_feeler.transmitted_color = light_color;

            //add light color to the pxl color
            pxl_color = light_color*pxl_color;



            //CHECK if obj in view is EMISSIVE
            if (obj_mat->emissive && depth < max_depth) { //if the object is emissive, return the base_color
                return obj_mat->base_color;
            }

            // Now check if obj_mat is REFLECTIVE : (can be both transparent and reflective)
            if (obj_mat->reflectivity > 0 && depth < max_depth) {
                //trace ray (recursively) that bounces off reflective surface along the normal
                // compound existing pxl color from other lights x
                // the weighted combo of diffuse color + reflected color
                mirror_reflected_color = obj_mat->base_color*TraceRay(Ray(pt.point, pt.normal), depth+1);
                if (obj_mat->texture != NULL) {
                    mirror_reflected_color = mirror_reflected_color * pt.hit_color;
                }
            }

            //if obj_mat is TRANSPARENT: set GENERAL_COLOR
            if (obj_mat->refract_idx_in >0 || obj_mat->refract_idx_out>0) {
                //if either are not 0, then material is transparent!
                //calculate refracted ray in/out of transparent obj
                glm::vec3 N = pt.normal;
                glm::vec3 new_origin;
                float ref_ratio;

                if (glm::dot(r.direction,pt.normal) > 0) {
                    //ray is exiting the object
                    ref_ratio = obj_mat->refract_idx_in/obj_mat->refract_idx_out;
                    N = -N; //reverse normal direction for when ray is exiting object (hits inside of sphere!)
                    //move point along normal so resulting ray does not self intersect
                    new_origin = pt.point - N *0.00012f;
                } else {
                    //ray is entering object
                    ref_ratio = obj_mat->refract_idx_out/obj_mat->refract_idx_in;
                    //move pt of intersection inside sphere
                    new_origin = pt.point - pt.normal*0.00012f;
                }
                glm::vec3 refract_dir = glm::refract(r.direction, N, ref_ratio);
                Ray refractedRay = Ray(new_origin, glm::normalize(refract_dir));
                general_color = obj_mat->base_color*TraceRay(refractedRay, depth+1);
                if (obj_mat->texture != NULL) {
                    general_color = general_color * pt.hit_color;
                }

            } else {
                general_color = pt.hit_color*obj_mat->EvaluateReflectedEnergy(pt, r, light_feeler);

                //object is OPAQUE, check for shadow
                if (ShadowTest(pt.point, light_feeler, this->scene->lights[i])) { //if this pt is in shadow:

                    Material* hit_obj_mat = this->intersection_engine->GetIntersection(light_feeler).object_hit->material;
                    if (hit_obj_mat->refract_idx_in>0 || hit_obj_mat->refract_idx_out>0) {
                        general_color = hit_obj_mat->base_color*general_color;
                    }else {
                        return glm::vec3(0.0f);
                    }
                }

            } //end obj_materials calculations

            //set total color of this pixel due to this light before moving to next light
            pxl_color = pxl_color* ((1-obj_mat->reflectivity)*general_color + obj_mat->reflectivity*mirror_reflected_color);

        } //end for loop going through each light
        //pxl_color = pxl_color/(float)this->scene->lights.count();

    } //end else statement
    return pxl_color;
}



// takes as input a pt in world space
// returns true if there is something between point and a light
// returns false if object DOES see light
bool Integrator::ShadowTest(glm::vec3 &point, Ray light_feeler, Geometry *light) { // point in world space

    Intersection test = this->intersection_engine->GetIntersection(light_feeler);
    if (test.object_hit == light) { //first object encountered was light source itself
        return false; // no obstructing lights present! not in shadow
    }
    return true;
}







void Integrator::SetDepth(unsigned int depth)
{
    max_depth = depth;
}
