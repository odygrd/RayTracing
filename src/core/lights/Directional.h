/**
 * @file   Directional.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_DIRECTIONAL_H
#define RAYTRACING_DIRECTIONAL_H

#include "Light.h"

/**
 * Directional light
 */
class DirectionalLight: public Light {
public:

    // constructor
    DirectionalLight(const Color& intensity, const Point& direction)
        : Light(intensity, direction.GetNormalized())
    {}
};



#endif //RAYTRACING_DIRECTIONAL_H
