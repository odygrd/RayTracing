/**
 * @file   Sphere.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "IMesh.h"

// Sphere definition
class Sphere : public IMesh
{
public:

    // constructor
    Sphere(){
        center.Set(0, 0, 0);
        radius = 1.0;
    }

    // intsersect a ray against the unit sphere
    // ray must be transformed into model space, first
    bool intersectRay(Ray &r, HitInfo &h, int face=HIT_FRONT) const {

        // pre-compute values for quadratic solution
        Point pos = r.pos - center;
        float A = r.dir % r.dir;
        float B = 2.0 * pos % r.dir;
        float C = pos % pos - radius * radius;
        float det = (B * B) - (4 * A * C);

        // if the ray intersects, compute the z-buffer value
        if(det >= 0){
            float z = (-B - sqrt(det)) / (2.0 * A);
            h.zDistance = z;

            // also get the surface intersection and normal
            h.point = r.pos + z * r.dir;
            h.normal = h.point.GetNormalized();

            // return true, ray is hit
            return true;

            // otherwise, return false
        }else
            return false;
    }

private:

    // sphere center and its radius
    Point center;
    float radius;
};


#endif //RAYTRACING_SPHERE_H
