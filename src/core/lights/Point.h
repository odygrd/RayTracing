/**
 * @file   Point.h
 * @author Odysseus Georgoudis
 * @date   01/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_POINT_H
#define RAYTRACING_POINT_H

#include "ILight.h"
// point light definition
class PointLight: public ILight{
public:

    // constructor
    PointLight(){
        intensity.Set(0, 0, 0);
        position.Set(0, 0, 0);
    }

    // get color of point light
    Color illuminate(Point p){
        return intensity;
    }

    // get direction of point light (calculated)
    Point direction(Point p){
        return (p - position).GetNormalized();
    }

    // set color of point light
    void setIntensity(Color c){
        intensity = c;
    }

    // set the location of the point light
    void setPosition(Point pos){
        position = pos;
    }

private:

    // intensity (or color) of light
    Color intensity;

    // location of light
    Point position;
};


#endif //RAYTRACING_POINT_H
