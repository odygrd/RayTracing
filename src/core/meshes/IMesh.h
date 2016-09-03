/**
 * @file   IMesh.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_IMESH_H
#define RAYTRACING_IMESH_H

#include <memory>
#include <vector>
#include "../Ray.h"

/**
 * Interface for different geometric shapes
 */
class IMesh
{
public:
    virtual ~IMesh() = default;
    virtual bool RayIntersection(const Ray &r, HitInfo &h, int face = HIT_FRONT) const = 0;
};

#endif //RAYTRACING_IMESH_H
