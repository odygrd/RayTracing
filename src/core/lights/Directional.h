/**
 * @file   Directional.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_DIRECTIONAL_H
#define RAYTRACING_DIRECTIONAL_H

#include "ILight.h"

// direct light definition
class DirectLight: public ILight{
public:

    // constructor
    DirectLight(){
        intensity.Set(0, 0, 0);
        dir.Set(0, 0, 1);
    }

    // get color of direct light
    Color illuminate(Point p){
        return intensity;
    }

    // get direction of direct light (constant)
    Point direction(Point p){
        return dir;
    }

    // set color of direct light
    void setIntensity(Color c){
        intensity = c;
    }

    // set direction of direct light
    void setDirection(Point d){
        dir = d.GetNormalized();
    }

private:

    // intensity (or color) of light
    Color intensity;

    // direction of light
    Point dir;
};



#endif //RAYTRACING_DIRECTIONAL_H
