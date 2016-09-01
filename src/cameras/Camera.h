/**
 * @file   Camera.h
 * @author Odysseus Georgoudis
 * @date   30/08/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include "../math/cyMath.h"
#include "../core/Transformation.h"
class Camera : public Transformation
{
public:
    /**
     * Constructors
     */
    Camera() = default;

    Camera(const Point& inPosition,
           const Point& inDirection,
           const Point& inUp,
           float inFov,
           int   inImageWidth,
           int   inImageHeight);

    /**
     * Calculate necessary camera values from input.
     * Setup all the camera variables first before calling this function
     */
    void setup();

    /**
     * Class public members
     */
    Point position    { 0, 0, 0 };
    Point direction   { 0, 0, -1 };
    Point up          { 0, 1, 0 };
    Point cross       { 0, 0, 0 };
    float fov         { 40 };
    int   imageWidth  { 200 };
    int   imageHeight { 150 };
};

#endif //RAYTRACING_CAMERA_H
