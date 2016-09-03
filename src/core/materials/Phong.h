/**
 * @file   Phong.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief
 *
 */

#ifndef RAYTRACING_PHONG_H
#define RAYTRACING_PHONG_H

#include "Material.h"
#include "../Ray.h"

/**
 * The Phong material type takes into account the surface curvature,
 * amount of light, and camera angle to get accurate shading and highlights.
 * The algorithm results in tight highlights that are excellent for polished
 * shiny surfaces, such as plastic, porcelain, and glazed ceramic.
 */
class PhongMaterial: public Material
{
public:

    /**
     * Constructors
     */
    PhongMaterial()
        : Material(Color { 0.5, 0.5, 0.5 }, Color { 0.7, 0.7, 0.7 }, 20)
    {}

    PhongMaterial(const Color& diffuse,
                  const Color& specular,
                  float        shininess)
        : Material(diffuse, specular, shininess)
    {}

    /**
     * Shading function (phong)
     */
    Color Shade(const Ray& ray,
                const HitInfo& hitInfo,
                const LightsMap& sceneLights) const override;
};

#endif //RAYTRACING_PHONG_H
