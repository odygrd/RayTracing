/**
 * @file   SceneNode.cpp
 * @author Odysseus Georgoudis
 * @date   31/08/2016
 * @brief  
 *
 */

#include "SceneNode.h"

Ray SceneNode::toModelSpace(const Ray& ray) const
{
    auto position  = TransformTo(ray.pos);
    auto direction = TransformTo(ray.pos + ray.dir) - position;
    return Ray { position, direction };
}

void SceneNode::fromModelSpace(HitInfo& hitInfo) const
{
    hitInfo.point = TransformFrom(hitInfo.point);
    hitInfo.normal = VecTransformFrom(hitInfo.normal).GetNormalized();
}