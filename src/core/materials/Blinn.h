/**
 * @file   Blinn.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_BLINN_H
#define RAYTRACING_BLINN_H

#include "Material.h"
#include "../Ray.h"

/**
 * The Blinn material type calculates surfaces similarly to Phong,
 * but the shape of the specular highlights in Blinn materials reflects light
 * more accurately.
 * Blinn is good for metallic surfaces with soft highlights,
 * such as brass or aluminum.
 */
#include <iostream>

class BlinnMaterial: public Material
{
public:
    /**
     * Constructors
     */
    BlinnMaterial()
        : Material(Color { 0.5, 0.5, 0.5 }, Color { 0.7, 0.7, 0.7 }, 20)
    {}

    BlinnMaterial(const Color& diffuse,
                  const Color& specular,
                  float        shininess)
        : Material(diffuse, specular, shininess)
    {}

    /**
     * shading function (blinn)
     */
    Color Shade(const Ray& ray,
                const HitInfo& hitInfo,
                const LightsMap& sceneLights) const override;
};


#endif //RAYTRACING_BLINN_H
