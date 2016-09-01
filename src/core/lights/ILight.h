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

class ILight
{
public:
    // get color of ambient light
    virtual Color illuminate(Point p) = 0;

    // get direction of ambient light (non-sensical)
    virtual Point direction(Point p) = 0;

    // return true, since light is ambient
    virtual bool isAmbient() { return false; }
};

using LightsMap = std::unordered_map<std::string, std::unique_ptr<ILight>>;

#endif //RAYTRACING_ILIGHT_H
