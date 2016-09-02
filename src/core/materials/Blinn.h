/**
 * @file   Blinn.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_BLINN_H
#define RAYTRACING_BLINN_H

#include "IMaterial.h"
#include "../Ray.h"

/**
 * The Blinn material type calculates surfaces similarly to Phong,
 * but the shape of the specular highlights in Blinn materials reflects light
 * more accurately.
 * Blinn is good for metallic surfaces with soft highlights,
 * such as brass or aluminum.
 */
class BlinnMaterial: public IMaterial
{
public:

    // constructor
    BlinnMaterial(){
        diffuse.Set(0.5, 0.5, 0.5);
        specular.Set(0.7, 0.7, 0.7);
        shininess = 20.0;
    }

    // shading function (blinn-phong)
    Color shade(Ray &r, HitInfo &h, const LightsMap& lights) const{

        // initialize color at pixel
        Color c;
        c.Set(0.0, 0.0, 0.0);

        // add shading from each light
        size_t numLights = lights.size();
        for(auto& elem : lights){

            // grab light
            Light *light = elem.second.get();

            // ambient light check
            if(light->IsAmbient()){

                // add ambient lighting term
                c += diffuse * light->Illuminate(h.point);

                // otherwise, add diffuse and specular components from light
            }else{

                // grab vector to light
                Point l = -light->Direction(h.point);
                l.Normalize();

                // grab vector to camera
                Point v = -r.dir;
                v.Normalize();

                // grab normal
                Point n = h.normal;
                n.Normalize();

                // calculate geometry term
                float geom = n % l;

                // calculate half-way vector
                Point half = v + l;
                half.Normalize();

                // calculate total specular factor
                float s = static_cast<float>(pow(half % n, shininess));

                // add specular and diffuse lighting terms (only if positive)
                if (geom > 0)
                    c += light->Illuminate(h.point) * (geom * diffuse + s * specular);
            }
        }

        // return final shaded color
        return c;
    }

    // set the diffuse color of the material
    void setDiffuse(Color c){
        diffuse = c;
    }

    // set the specular color of the material
    void setSpecular(Color c){
        specular = c;
    }

    // set the shininess factor of the material
    void setShininess(float s){
        shininess = s;
    }

private:

    // colors for shading
    Color diffuse, specular;

    // shininess factor for shading
    float shininess;
};


#endif //RAYTRACING_BLINN_H
