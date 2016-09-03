/**
 * @file   Sphere.cpp
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#include "Sphere.h"

bool Sphere::RayIntersection(const Ray& ray,
                             HitInfo& hitInfo,
                             int face /* = HIT_FRONT */) const
{
    // pre-compute values for quadratic solution
    const Point pos { ray.pos - center_ };
    const float A   { ray.dir % ray.dir };
    const float B   { 2.0 * pos % ray.dir };
    const float C   { pos % pos - radius_ * radius_ };
    const float det { (B * B) - (4 * A * C) };

    if (det < 0)
    {
        // ray didn't hit anything, return false
        return false;
    }

    // otherwise, the ray intersects, compute the z-buffer value

    // solve the quadratic and store the result
    hitInfo.zDistance = (-B - sqrt(det)) / (2.0 * A);

    // also get the surface intersection and normal
    hitInfo.point = ray.pos + hitInfo.zDistance * ray.dir;

    // also get the surface normal
    hitInfo.normal = hitInfo.point.GetNormalized();

    // return true, ray is hit
    return true;
}