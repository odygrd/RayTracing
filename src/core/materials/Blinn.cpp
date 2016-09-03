/**
 * @file   Blinn.cpp
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief
 *
 */

#include "Blinn.h"

Color BlinnMaterial::Shade(const Ray& ray,
                           const HitInfo& hitInfo,
                           const LightsMap& sceneLights) const
{
    // initialize color at pixel
    Color color { 0.0, 0.0, 0.0 };

    // calculate shading from each light
    for (const auto& elem : sceneLights)
    {
        // grab light
        const Light& light { *elem.second.get() };

        if (!light.IsAmbient()) // ambient light check
        {
            //add diffuse and specular components from light

            // grab vector to light
            // direction is already normalised.
            const Point lightDir { -light.GetDirection(hitInfo.point) };

            // grab vector to camera
            const Point rayDir { -ray.dir.GetNormalized() };

            // grab normal
            const Point hitNormal = hitInfo.normal.GetNormalized();

            // calculate geometry term
            const float geom { hitNormal % lightDir };

            // calculate half-way vector
            const Point half { (rayDir + lightDir).GetNormalized() };

            // calculate total specular factor
            float specularFactor {
                static_cast<float>(pow(half % hitNormal, shininess_)) };

            // add specular and diffuse lighting terms (only if positive)
            if (geom > 0)
            {
                color += light.Illuminate(hitInfo.point)
                    * (geom * diffuse_ + specularFactor * specular_);
            }
        }
        else // add ambient light
        {
            color += diffuse_ * light.Illuminate(hitInfo.point);
        }
    }

    // return final shaded color
    return color;
}