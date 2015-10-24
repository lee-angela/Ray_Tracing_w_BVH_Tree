#include "camera.h"

#include <la.h>
#include <iostream>
#include <math.h>

Camera::Camera():
    Camera(400, 400)
{
    look = glm::vec3(0,0,-1);
    up = glm::vec3(0,1,0);
    right = glm::vec3(1,0,0);
}

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp):
    fovy(45),
    width(w),
    height(h),
    near_clip(0.1f),
    far_clip(1000),
    eye(e),
    ref(r),
    world_up(worldUp)
{
    RecomputeAttributes();
}

Camera::Camera(const Camera &c):
    fovy(c.fovy),
    width(c.width),
    height(c.height),
    near_clip(c.near_clip),
    far_clip(c.far_clip),
    aspect(c.aspect),
    eye(c.eye),
    ref(c.ref),
    look(c.look),
    up(c.up),
    right(c.right),
    world_up(c.world_up),
    V(c.V),
    H(c.H)
{}

void Camera::CopyAttributes(const Camera &c)
{
    fovy = c.fovy;
    near_clip = c.near_clip;
    far_clip = c.far_clip;
    eye = c.eye;
    ref = c.ref;
    look = c.look;
    up = c.up;
    right = c.right;
    width = c.width;
    height = c.height;
    aspect = c.aspect;
    V = c.V;
    H = c.H;
}

void Camera::RecomputeAttributes()
{
    aspect = (float)width / (float)height; //if screen size is adjusted
    look = glm::normalize(ref - eye);
    right = glm::normalize(glm::cross(look, world_up));
    up = glm::normalize(glm::cross(right, look));

    //viewing frustrum values
    V = up * glm::length(ref - eye) * glm::tan(fovy/2 * DEG2RAD);
    H = right * glm::length(ref - eye) * aspect * glm::tan(fovy/2 * DEG2RAD);
}

glm::mat4 Camera::getViewProj()
{
    return this->PerspectiveProjectionMatrix() * this->ViewMatrix();
}

glm::mat4 Camera::PerspectiveProjectionMatrix()
{
    float top = near_clip * glm::tan(fovy / 2 * DEG2RAD);
    float bottom = -top;
    float right_bound = top * aspect;
    float left_bound = -right_bound;

    glm::mat4 ppmat(0.0);
    ppmat[0][0] = (2 * near_clip) / (right_bound - left_bound);
    ppmat[1][1] = (2 * near_clip) / (top - bottom);

    ppmat[2][0] = -(right_bound + left_bound) / (right_bound - left_bound);
    ppmat[2][1] = -(top + bottom) / (top - bottom);
    ppmat[2][2] = far_clip / (far_clip - near_clip);
    ppmat[2][3] = 1;

    ppmat[3][2] = -(far_clip * near_clip) / (far_clip - near_clip);

    return ppmat;
}

glm::mat4 Camera::ViewMatrix()
{
    glm::mat4 omat(1.0);
    glm::mat4 tmat(1.0);

    omat[0][0] = right[0];
    omat[1][0] = right[1];
    omat[2][0] = right[2];

    omat[0][1] = up[0];
    omat[1][1] = up[1];
    omat[2][1] = up[2];

    omat[0][2] = look[0];
    omat[1][2] = look[1];
    omat[2][2] = look[2];

    tmat[3][0] = -eye[0];
    tmat[3][1] = -eye[1];
    tmat[3][2] = -eye[2];

    return omat * tmat;
}

void Camera::RotateAboutUp(float deg)
{
    deg *= DEG2RAD;
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, up);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    RecomputeAttributes();
}
void Camera::RotateAboutRight(float deg)
{
    deg *= DEG2RAD;
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, right);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    RecomputeAttributes();
}

void Camera::TranslateAlongLook(float amt)
{
    glm::vec3 translation = look * amt;
    eye += translation;
    ref += translation;
}

void Camera::TranslateAlongRight(float amt)
{
    glm::vec3 translation = right * amt;
    eye += translation;
    ref += translation;
}
void Camera::TranslateAlongUp(float amt)
{
    glm::vec3 translation = up * amt;
    eye += translation;
    ref += translation;
}

Ray Camera::Raycast(const glm::vec2 &pt)
{
    return Raycast(pt.x, pt.y);
}

Ray Camera::Raycast(float x, float y)
{
    //convert pts to NDC
    float sx = (2 * x / width) - 1;
    float sy = 1 - (2 * y / height);

    glm::vec3 p = ref + sx * H + sy * V; //where sx and sy are in NDC
    glm::vec3 rayCastDirection = glm::normalize(p - eye);

    Ray castRay(eye, rayCastDirection);
    return castRay;
}

Ray Camera::RaycastNDC(float ndc_x, float ndc_y)
{
    glm::vec3 p = ref + ndc_x * H + ndc_y * V; //where ndc_x and ndc_y are already in NDC
    glm::vec3 rayCastDirection = glm::normalize(p - eye);

    Ray castRay(eye, rayCastDirection);
    return castRay;
}

void Camera::create()
{
    std::vector<glm::vec3> pos;
    std::vector<glm::vec3> col;
    std::vector<GLuint> idx;

    //0: Eye position
    pos.push_back(eye);
    //1 - 4: Near clip
        //Lower-left
        Ray r = this->RaycastNDC(-1,-1);
        pos.push_back(eye + r.direction * near_clip);
        //Lower-right
        r = this->RaycastNDC(1,-1);
        pos.push_back(eye + r.direction * near_clip);
        //Upper-right
        r = this->RaycastNDC(1,1);
        pos.push_back(eye + r.direction * near_clip);
        //Upper-left
        r = this->RaycastNDC(-1,1);
        pos.push_back(eye + r.direction * near_clip);
    //5 - 8: Far clip
        //Lower-left
        r = this->RaycastNDC(-1,-1);
        pos.push_back(eye + r.direction * far_clip);
        //Lower-right
        r = this->RaycastNDC(1,-1);
        pos.push_back(eye + r.direction * far_clip);
        //Upper-right
        r = this->RaycastNDC(1,1);
        pos.push_back(eye + r.direction * far_clip);
        //Upper-left
        r = this->RaycastNDC(-1,1);
        pos.push_back(eye + r.direction * far_clip);

    for(int i = 0; i < 9; i++){
        col.push_back(glm::vec3(1,1,1));
    }

    //Frustum lines
    idx.push_back(1);idx.push_back(5);
    idx.push_back(2);idx.push_back(6);
    idx.push_back(3);idx.push_back(7);
    idx.push_back(4);idx.push_back(8);
    //Near clip
    idx.push_back(1);idx.push_back(2);
    idx.push_back(2);idx.push_back(3);
    idx.push_back(3);idx.push_back(4);
    idx.push_back(4);idx.push_back(1);
    //Far clip
    idx.push_back(5);idx.push_back(6);
    idx.push_back(6);idx.push_back(7);
    idx.push_back(7);idx.push_back(8);
    idx.push_back(8);idx.push_back(5);

    //Camera axis
    pos.push_back(eye); col.push_back(glm::vec3(0,0,1)); idx.push_back(9);
    pos.push_back(eye + look); col.push_back(glm::vec3(0,0,1));idx.push_back(10);
    pos.push_back(eye); col.push_back(glm::vec3(1,0,0));idx.push_back(11);
    pos.push_back(eye + right); col.push_back(glm::vec3(1,0,0));idx.push_back(12);
    pos.push_back(eye); col.push_back(glm::vec3(0,1,0));idx.push_back(13);
    pos.push_back(eye + up); col.push_back(glm::vec3(0,1,0));idx.push_back(14);

    count = idx.size();

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(idx.data(), count * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(pos.data(), pos.size() * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(col.data(), col.size() * sizeof(glm::vec3));
}

GLenum Camera::drawMode(){return GL_LINES;}
