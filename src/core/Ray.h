/**
 * @file   Ray.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include "../math/cyMath.h"
#include "Transformation.h"

class SceneNode;

static constexpr float FLOAT_MAX = 1.0e30f;

#define HIT_NONE  0
#define HIT_FRONT 1
#define HIT_BACK  2
#define HIT_FRONT_AND_BACK (HIT_FRONT | HIT_BACK)

class Ray
{
public:
    Point pos; ///< position of ray
    Point dir; ///< direction of ray

    /**
     * Constructors
     */
    Ray(const Point &p, const Point& d)
        : pos { p },
          dir { d }
    {}

    /**
     * Normalize the ray (only the direction is necessary)
     */
    void Normalize() { dir.Normalize(); }

    /**
     * Transformation of ray to model (local) space
     * @param ray
     * @return
     */
    void ToModelSpace(const Transformation& transform)
    {
        auto position  = transform.TransformTo(pos);
        dir = transform.TransformTo(pos + dir) - position;
        pos = position;
    }
};

// Hit Info struct definitions (set for each node)
struct HitInfo
{
    float zDistance { FLOAT_MAX }; ///< distance from the ray to the hit point
    Point point;              ///< where the object gets hit
    Point normal;             ///< surface normal of the object at the hit point
    const SceneNode* node { nullptr }; ///< object node that ray hits
    bool front;   ///< true if object is hit on a front face
                 ///< false if back face

    /**
     * Transformation of hit information from model (local) space
     * back to world space
     * @param hitInfo
     */
    void FromModelSpace(const Transformation& transform)
    {
        point = transform.TransformFrom(point);
        normal = transform.VecTransformFrom(normal).GetNormalized();
    }
};

#endif //RAYTRACING_RAY_H
