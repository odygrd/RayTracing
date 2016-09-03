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

/**
 * Point light definitiion
 */
class PointLight : public Light
{
public:
    /**
     * Constructor
     * @param intensity
     * @param position
     * @return
     */
    PointLight(const Color& intensity, const Point& position)
        : Light { intensity, Point { 0, 0, 0} },
          position_ { position }
    {}

    /**
     * Get the direction of the light
     * @param point
     * @return
     */
    const Point& Direction(const Point& point) const override
    {
        direction_ = (point - position_).GetNormalized();
        return direction_;
    }

private:
    Point position_; ///< location of light
};

#endif //RAYTRACING_POINT_H
