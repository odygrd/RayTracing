/**
 * @file   Phong.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_PHONG_H
#define RAYTRACING_PHONG_H

#include "IMaterial.h"
#include "../Ray.h"
/**
 * The Phong material type takes into account the surface curvature,
 * amount of light, and camera angle to get accurate shading and highlights.
 * The algorithm results in tight highlights that are excellent for polished
 * shiny surfaces, such as plastic, porcelain, and glazed ceramic.
 */
class PhongMaterial: public IMaterial
{
public:

    // constructor
    PhongMaterial(){
        diffuse.Set(0.5, 0.5, 0.5);
        specular.Set(0.7, 0.7, 0.7);
        shininess = 20.0;
    }

    // shading function (phong)
    Color shade(Ray &r, HitInfo &h, const LightsMap& lights) const{

        // initialize color at pixel
        Color c;
        c.Set(0.0, 0.0, 0.0);

        // add shading from each light
        int numLights = lights.size();
        for(auto& elem : lights){

            // grab light
            ILight *light = elem.second.get();

            // ambient light check
            if(light->isAmbient()){

                // add ambient lighting term
                c += diffuse * light->illuminate(h.point);

                // otherwise, add diffuse and specular components from light
            }else{

                // grab vector to light
                Point l = -light->direction(h.point);
                l.Normalize();

                // grab vector to camera
                Point v = -r.dir;
                v.Normalize();

                // grab normal
                Point n = h.normal;
                n.Normalize();

                // calculate geometry term
                float geom = n % l;

                // calculate reflection vector
                Point refl = l - 2.0 * (l % n) * n;

                // calculate total specular factor
                // (adjusted shininess to match blinn-phong values)
                float s = pow(refl % v, shininess);

                // add specular and diffuse lighting terms (only if positive)
                if(geom > 0)
                    c += light->illuminate(h.point) * (geom * diffuse + s * specular);
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



#endif //RAYTRACING_PHONG_H
