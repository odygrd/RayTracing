/**
 * @file   Sphere.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "IMesh.h"

/**
 * Sphere shape definition.
 * This is always a fixed (0,0,0) radius 1 sphere.
 * All the transformation for this object are stored under the SceneNode class
 * @see SceneNode Transformation
 */
class Sphere : public IMesh
{
public:

    /**
     * Constructors
     */
    constexpr Sphere() = default;

    /**
     * Intersect a ray against the unit sphere.
     * @param ray      The ray, transformed into model (local) space
     * @param hitInfo  Populate object hit information
     * @param face
     * @return         true if the object was hit, false otherwise
     */
    bool RayIntersection(const Ray& ray,
                         HitInfo& hitInfo,
                         int face = HIT_FRONT) const override;

private:
    Point center_ { 0, 0, 0 };
    float radius_ { 1.0 };
};


#endif //RAYTRACING_SPHERE_H
