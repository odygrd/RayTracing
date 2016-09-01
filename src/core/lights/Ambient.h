/**
 * @file   Ambient.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_AMBIENT_H
#define RAYTRACING_AMBIENT_H

#include "ILight.h"

// ambient light definition
class AmbientLight: public ILight{
public:

    // constructor
    AmbientLight(){
        intensity.Set(0, 0, 0);
    }

    // get color of ambient light
    Color illuminate(Point p){
        return intensity;
    }

    // get direction of ambient light (non-sensical)
    Point direction(Point p){
        return Point(0, 0, 0);
    }

    // return true, since light is ambient
    bool isAmbient(){
        return true;
    }

    // set color of ambient light
    void setIntensity(Color c){
        intensity = c;
    }

private:

    // intensity (or color) of light
    Color intensity;
};


#endif //RAYTRACING_AMBIENT_H
