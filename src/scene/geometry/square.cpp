#include <scene/geometry/square.h>



Intersection SquarePlane::GetIntersection(Ray r)
{
    Intersection pt;

    //make local version copy of ray
    Ray r1 = r.GetTransformedCopy(this->transform.invT());

    glm::vec3 norm = glm::vec3(0.0f,0.0f,1.0f); //normal to default square plane
    // S is a point (any) on the plane
    glm::vec3 S = glm::vec3(0.3f,0.3f,0.0f); //centered at origin, along XY plane, normal facing towards pos z

    float t = glm::dot(norm, ((S-r1.origin)/glm::dot(norm, r1.direction)));
    if (t > 0) { //negative t means intersection happened behind cam (ignore)
        //calculate point in local space
        glm::vec3 P = r1.origin + t * r1.direction;

        //check if the point is within bounds of square
        if (P[0]>=-0.5f && P[0]<=0.5f && P[1]>=-0.5f && P[1]<=0.5f) {
            //change all attributes to world-coordinates & store in "pt"
            pt.point = glm::vec3(this->transform.T() * glm::vec4(P,1));
            pt.normal = glm::normalize(glm::vec3(this->transform.invTransT() * glm::vec4(norm,0.0f)));
            pt.object_hit = this;
            if (pt.object_hit->material->texture == NULL) {
                pt.hit_color = this->material->base_color;
            } else {
                pt.hit_color = this->material->base_color*this->material->GetImageColor(GetUVCoordinates(P), this->material->texture);
            }
            pt.t = t;

        }
    }
    return pt;
}






// this function takes as input a local-space vec3 point
// on the surface of the object and returns a vec2 representing
// the texture coordinates at that point.

glm::vec2 SquarePlane::GetUVCoordinates(const glm::vec3 &point) {

    glm::vec2 uv;
    uv[0] = point[0]+0.5;
    uv[1] = point[1]+0.5;
    return uv;
}







void SquarePlane::create()
{
    GLuint cub_idx[6];
    glm::vec3 cub_vert_pos[4];
    glm::vec3 cub_vert_nor[4];
    glm::vec3 cub_vert_col[4];

    cub_vert_pos[0] = glm::vec3(-0.5f, 0.5f, 0);  cub_vert_nor[0] = glm::vec3(0, 0, 1); cub_vert_col[0] = material->base_color;
    cub_vert_pos[1] = glm::vec3(-0.5f, -0.5f, 0); cub_vert_nor[1] = glm::vec3(0, 0, 1); cub_vert_col[1] = material->base_color;
    cub_vert_pos[2] = glm::vec3(0.5f, -0.5f, 0);  cub_vert_nor[2] = glm::vec3(0, 0, 1); cub_vert_col[2] = material->base_color;
    cub_vert_pos[3] = glm::vec3(0.5f, 0.5f, 0);   cub_vert_nor[3] = glm::vec3(0, 0, 1); cub_vert_col[3] = material->base_color;

    cub_idx[0] = 0; cub_idx[1] = 1; cub_idx[2] = 2;
    cub_idx[3] = 0; cub_idx[4] = 2; cub_idx[5] = 3;

    count = 6;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(cub_idx, 6 * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(cub_vert_pos, 4 * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(cub_vert_nor, 4 * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(cub_vert_col, 4 * sizeof(glm::vec3));

    this->boundingBox = new BoundingBox();
    setBoundingBox();
}


/**
 * @brief SquarePlane::setBoundingBox
 * @brief called from create() function for squarePlane and filled with TRANSFORMED (min x, min y, and min z)
 * @brief and TRANSFORMED (max x, max y, max z)
 *
 * @param cub_vert_pos (vertex positions of cubes)
 */
void SquarePlane::setBoundingBox() {

    this->boundingBox->setTransformedBox(this->transform.T());
    this->boundingBox->create();
}


bool SquarePlane::isMesh() {
    return false;
}

