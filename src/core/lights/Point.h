/**
 * @file   Point.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_POINT_H
#define RAYTRACING_POINT_H

#include "Light.h"
// point light definition
class PointLight : public Light
{
public:

    // constructor
    PointLight(const Color& intensity, const Point& position)
        : Light ( intensity, Point { 0, 0, 0} ),
          position_ (position)
    {}

    // get direction of point light (calculated)

    const Point& Direction(const Point& point) const override
    {
        direction_ = (point - position_).GetNormalized();
        return direction_;
    }

private:
    Point position_; ///< location of light
};


#endif //RAYTRACING_POINT_H
