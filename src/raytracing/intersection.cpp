#include <raytracing/intersection.h>

Intersection::Intersection():
    point(glm::vec3(0)),
    normal(glm::vec3(0)),
    t(-1),
    hit_color(glm::vec3(0))
{
    object_hit = NULL;
}

IntersectionEngine::IntersectionEngine()
{
    scene = NULL;

    //initialize the root node of bvh tree
    BVHrootNode = new BVHnode();

}




/**
 * Helper function that recursively traverses the BVH tree
 *
 * input: BVHnode* node, Ray r
 * output: Intersection pt,
 *          (NOTE: this pt can be default if input node is NULL or there is no found intersection,
 *                  object_hit == NULL, and t = -1 for default pt)
 */

Intersection traverseBVH(BVHnode* node, Ray r) {
    Intersection pt;
    Intersection nothing;

    if (node == NULL) {
        return pt;
    }
    //check for intersection with this node's boundingBox
    pt = node->boundingBox->getIntersection(r);

    // if ray hits this bounding box
    if (pt.object_hit != NULL) { // ELSE pt.object_hit = NULL.
        //check if this is leaf node
        if (node->isLeaf()) {
            return node->geom->GetIntersection(r);
        }

        Intersection leftpt = node->leftChild->boundingBox->getIntersection(r);
        Intersection rightpt = node->rightChild->boundingBox->getIntersection(r);
        //if not leaf node, recurse down to children

        //hits neither of children
        if (leftpt.object_hit == NULL && rightpt.object_hit == NULL) {
            return nothing;
        }
        //IF BOTH CHILDREN ARE INTERSECTED BY RAY, need to check both
        if (leftpt.object_hit != NULL && rightpt.object_hit != NULL) {
            leftpt = traverseBVH(node->leftChild,r);
            rightpt = traverseBVH(node->rightChild,r);

            if (leftpt.t > 0 && rightpt.t < 0) { pt = leftpt; } //left child's geom intersection is closer

            else if (rightpt.t > 0 && leftpt.t < 0) { pt = rightpt;} //right child's geom intersection is closer

            else if (leftpt.t > 0 && rightpt.t > 0) { //ray hit both children's geom
                //take closer geom intersection
                if (leftpt.t < rightpt.t) { pt = leftpt;}
                else { pt = rightpt;}
            } else {
                return nothing;
            }
        }
        //JUST ONE (or none) CHILD IS INTERSECTED
        else {
            if (leftpt.object_hit != NULL) {
                pt = traverseBVH(node->leftChild,r);
            } else if (rightpt.object_hit != NULL){
                pt = traverseBVH(node->rightChild,r);
            } else {
                return leftpt;
            }
        }

    } //ray did not hit this boundingbox
    return pt;
}



/**
 *  Function that calls for recursion through the BVH tree through root
 *
 * input: Ray r
 * output: Intersection pt
 *          (NOTE: this pt can be default if input node is NULL or there is no found intersection,
 *                  object_hit == NULL, and t = -1 for default pt)
 */
Intersection IntersectionEngine::GetIntersection(Ray r)
{
    Intersection pt;

    if (this->BVHrootNode != NULL) {
        return traverseBVH(BVHrootNode, r);
    } else {
        return pt;
    }

    // previous implementation of checking for intersection in scene (WITHOUT BVH tree)
//        Intersection pt;
//        for (int i = 0; i < this->scene->objects.count(); i++) { //objects is a Qlist
//            Intersection curr = ((Geometry*)(this->scene->objects[i]))->GetIntersection(r);
//            if (curr.t >= 0.0f && (pt.t < 0.0f || curr.t < pt.t )) {
//                pt = curr;
//            }
//        }
//        return pt;
}
