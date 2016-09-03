/**
 * @file   ILight.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_ILIGHT_H
#define RAYTRACING_ILIGHT_H

#include <memory>
#include <unordered_map>
#include <string>
#include "../../math/cyMath.h"

/**
 * Light base class
 */
class Light
{
public:
    virtual ~Light() = default;

    /**
     * Get the color of light
     * @param p
     * @return
     */
    virtual const Color& Illuminate(const Point& point) const { return intensity_; }

    /**
     * Get the direction of light (non-sensical)
     * @param p
     * @return
     */
    virtual const Point& Direction(const Point& point) const { return direction_; }

    // return true, since light is ambient
    /**
     * @return true if light is ammbient
     *         false otherwise
     */
    virtual bool IsAmbient() const { return false; }

protected:
    Light(const Color& intensity, const Point& direction)
        : intensity_ { intensity },
          direction_ { direction.GetNormalized() }
    {}

    Color         intensity_; ///< intensity (or color) of light
    mutable Point direction_; ///< direction of light;
};

using LightsMap = std::unordered_map<std::string, std::unique_ptr<Light>>;

#endif //RAYTRACING_ILIGHT_H
