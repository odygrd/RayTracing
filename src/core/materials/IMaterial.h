/**
 * @file   IMaterial.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_IMATERIAL_H
#define RAYTRACING_IMATERIAL_H

#include <memory>
#include <unordered_map>
#include <string>
#include "../lights/Light.h"

class Ray;
class HitInfo;

class IMaterial
{
public:
    // shade method which calls all lights in the list
    // uses the incoming ray, hit info of rendering pixel, and all lights
    virtual Color shade(Ray& ray, HitInfo& hitInfo, const LightsMap& lights) const = 0;
};

using MaterialsMap = std::unordered_map<std::string, std::unique_ptr<IMaterial>>;

#endif //RAYTRACING_IMATERIAL_H
