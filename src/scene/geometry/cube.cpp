#include "cube.h"
#include "../../raytracing/intersection.h"
#include <la.h>
#include <iostream>
#include <math.h>

static const int CUB_IDX_COUNT = 36;
static const int CUB_VERT_COUNT = 24;


/**
 * @brief Cube::GetIntersection
 *
 * @param r
 * @return Intersection pt
 */
Intersection Cube::GetIntersection(Ray r)
{
    Ray r1 = r.GetTransformedCopy(this->transform.invT());
//returns the closest point of intersection from the ray's origin

    float t_near = -INFINITY;
    float t_far = INFINITY;
    //default Intersection pt
    Intersection pt;
    glm::vec4 normNear;
    glm::vec4 normFar;

    float t0;
    float t1;

    for (int i = 0; i < 3; i++) {
        if (r1.direction[i] == 0 && (r1.origin[i] < -0.5 || r1.direction[i] > 0.5)) {//parallel to the slab in question
            return pt; //misses the cube
        } else {
            t0 = (-0.5 - r1.origin[i])/ r1.direction[i];
            t1 = (0.5 - r1.origin[i]) / r1.direction[i];
            normNear = glm::vec4(0);
            normNear[i] = -1; //bottom plane (ray coming from bottom)
            normFar = glm::vec4(0);
            normFar[i] = 1;
            if (t0 > t1) {
                //makes t0 represent the intersection w/ slab closer to origin
                float temp = t0;
                t0 = t1;
                t1 = temp;
                normNear[i] = 1;
                normFar[i] = -1;
            }
            if (t0 > t_near) {
                t_near = t0;
                pt.normal = glm::vec3(normNear);
            }
            if (t1 < t_far) {
                t_far = t1;
            }
        }
    }
    if (t_near > t_far) {
        pt.normal = glm::vec3(0); //reset normal
        return pt; // box is missed
    } else {
        // NOW get the actual pt of intersection by plugging in t val:
        if (t_near < 0 && t_far >= 0) { // t_near not valid intersection, use t_far
            glm::vec3 temppt = r1.origin + t_far * r1.direction;
            pt.point = glm::vec3(this->transform.invT() * glm::vec4(temppt[0], temppt[1], temppt[2], 1));
            pt.normal = glm::normalize(glm::vec3(this->transform.invTransT()*normFar));
            pt.t = glm::distance(pt.point,r.origin);
            pt.object_hit = this;
            if (pt.object_hit->material->texture == NULL) {
                pt.hit_color = this->material->base_color;
            } else {
                //if there is an image associated with the material then use the color of the image
                pt.hit_color = this->material->GetImageColor(GetUVCoordinates(pt.point), this->material->texture);
            }
        } else {
            //make sure the saved pt of intersection is in world view!
            glm::vec3 temppt = r1.origin + t_near * r1.direction;
            pt.point = glm::vec3(this->transform.invT() * glm::vec4(temppt[0], temppt[1], temppt[2], 1));
            pt.normal = glm::normalize(glm::vec3(this->transform.invTransT()*normNear));
            pt.t = glm::distance(pt.point,r.origin);
            pt.object_hit = this;
            if (pt.object_hit->material->texture == NULL) {
                pt.hit_color = this->material->base_color;
            } else {
                pt.hit_color = this->material->base_color*this->material->GetImageColor(GetUVCoordinates(temppt), this->material->texture);
            }
        }
    }
    return pt;
}



/**
 * @brief Cube::GetUVCoordinates
 * @brief maps local/object space xyx coordinates into uv coordinates
 *
 * @param glm::vec3 point (in object/local space)
 * @return glm::vec2 texture coordinates at this point
 */

glm::vec2 Cube::GetUVCoordinates(const glm::vec3 &point) {

    glm::vec2 uv;
    //UNFOLDED UV MAP: in shape of upside down cross
    //axis are: x=outwards, y=horizontal, z=vertical
    //     1     (1) back
    //     2     (2) top
    //   3 4 5   (3)left, (4)front, (5)right
    //     6     (6)bottom

    //find which face its on
    //x value is largest (0.5) so
    if (glm::abs(point[0]) >= glm::abs(point[1]) && glm::abs(point[0]) >= glm::abs(point[2])) {
        //on x slabs: y=u(horizontal), z=v(vertical)
        if (point[0] < 0) { //negative x slab (1)
            uv[0] = 0.25*(point[1] + 0.5) + 0.25; //now range is between 0.25 -> 0.5
            uv[1] = 0.25*(point[2] + 0.5) + 0.75; //range: 0.75 -> 1.0
        } else { //positive x slab (4)
            uv[0] = 0.25*(point[1] + 0.5) + 0.25; //now range is between 0.25 -> 0.5
            uv[1] = 0.25*(point[2] + 0.5) + 0.25; //range: 0.25 -> 0.5
        }
    } else if (glm::abs(point[1]) >= glm::abs(point[0]) && glm::abs(point[1]) >= glm::abs(point[2])) {
        //on y slabs : x=u(horizontal), z=v(vertical)
        if (point[1] < 0) { //negative y slab (3)
            uv[0] = 0.25*(point[0] + 0.5) + 0.25; //now range is between 0.25 -> 0.5
            uv[1] = 0.25*(point[2] + 0.5) + 0.75; //range: 0.75 -> 1.0
        } else { //positive y slab (5)
            uv[0] = 0.25*(point[0] + 0.5); //range: 0 -> 0.25
            uv[1] = 0.25*(point[2] + 0.5) + 0.25; //range: 0.25 -> 0.5
        }
    } else if (glm::abs(point[2]) >= glm::abs(point[0]) && glm::abs(point[2]) >= glm:: abs(point[1])) {
        //on z slabs : y=u(horizontal), x=v(vertical)
        if (point[2] < 0) { //negative z slab (6)
            uv[0] = 0.25*(point[1] + 0.5) + 0.25; //range: 0.25 -> 0.5
            uv[1]= 0.25*(point[0] + 0.5); //range: 0 -> 0.25
        } else { //positive z slab (2)
            uv[0] = 0.25*(point[1] + 0.5) + 0.5; //range:0.5 -> 0.75
            uv[1] = 0.25*(point[0] + 0.5) + 0.25; //range:0.25 -> 0.5
        }
    }
    return uv;
}












/**
 * @brief createCubeVertexPositions
 * @brief These are functions that are only defined in this cpp file.
 * @brief They're used for organizational purposeswhen filling the arrays
 * @brief used to hold the vertex and index data.
 */

void createCubeVertexPositions(glm::vec3 (&cub_vert_pos)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);

    //Right face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);

    //Left face
    //UR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);

    //Back face
    //UR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);

    //Top face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);

    //Bottom face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
}


/**
 * @brief createCubeVertexNormals
 */
void createCubeVertexNormals(glm::vec3 (&cub_vert_nor)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,0,1);
    }
    //Right
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(1,0,0);
    }
    //Left
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(-1,0,0);
    }
    //Back
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,0,-1);
    }
    //Top
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,1,0);
    }
    //Bottom
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,-1,0);
    }
}

/**
 * @brief createCubeIndices
 */
void createCubeIndices(GLuint (&cub_idx)[CUB_IDX_COUNT])
{
    int idx = 0;
    for(int i = 0; i < 6; i++){
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+1;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4+3;
    }
}

/**
 * @brief Cube::create
 * @brief also calls for setting of boundingBox min/max bounds
 */
void Cube::create()
{
    GLuint cub_idx[CUB_IDX_COUNT];
    glm::vec3 cub_vert_pos[CUB_VERT_COUNT];
    glm::vec3 cub_vert_nor[CUB_VERT_COUNT];
    glm::vec3 cub_vert_col[CUB_VERT_COUNT];

    createCubeVertexPositions(cub_vert_pos);
    createCubeVertexNormals(cub_vert_nor);
    createCubeIndices(cub_idx);

    for(int i = 0; i < CUB_VERT_COUNT; i++){
        cub_vert_col[i] = material->base_color;
    }

    count = CUB_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(cub_idx, CUB_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(cub_vert_pos,CUB_VERT_COUNT * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(cub_vert_nor, CUB_VERT_COUNT * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(cub_vert_col, CUB_VERT_COUNT * sizeof(glm::vec3));

    this->boundingBox = new BoundingBox();
    setBoundingBox();
}



/**
 * @brief Cube::setBoundingBox
 * @brief called from after this.transform is defined
 * @brief and TRANSFORMED (max x, max y, max z)
 *
 * @param cub_vert_pos (vertex positions of cubes)
 */
void Cube::setBoundingBox() {
    this->boundingBox->setTransformedBox(this->transform.T());
    boundingBox->create();
}




bool Cube::isMesh() {
    return false;
}


