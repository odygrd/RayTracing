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
#include "../../math/cyMath.h"

class Ray;
class HitInfo;

class Material
{
public:
    /**
     * Shade method which calls all lights in the list and calculates the color.
     * @param ray     the incoming ray
     * @param hitInfo the hit info of rendering pixel
     * @param lights  all scene lights
     * @return        final shaded color
     */
    virtual Color Shade(const Ray& ray,
                        const HitInfo& hitInfo,
                        const LightsMap& sceneLights) const = 0;

protected:
    Material(const Color& diffuse, const Color& specular, float shininess)
        : diffuse_ { diffuse },
          specular_ { specular },
          shininess_ { shininess }
    {}

    Color diffuse_;   ///< diffuse color for shading
    Color specular_;  ///< specular color for shading
    float shininess_; ///< shininess factor for shading
};

using MaterialsMap = std::unordered_map<std::string, std::unique_ptr<Material>>;

#endif //RAYTRACING_IMATERIAL_H
