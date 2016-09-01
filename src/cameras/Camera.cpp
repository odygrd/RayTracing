/**
 * @file   Camera.cpp
 * @author Odysseus Georgoudis
 * @date   30/08/2016
 * @brief  
 *
 */

#include "Camera.h"

Camera::Camera(const Point& inPosition,
               const Point& inDirection,
               const Point& inUp,
               float inFov,
               int   inImageWidth,
               int   inImageHeight)
    : position(inPosition),
      direction(inDirection),
      up(inUp),
      fov(inFov),
      imageWidth(inImageWidth),
      imageHeight(inImageHeight)
{
    setup();
}

void Camera::setup()
{
    direction = direction - position;
    direction.Normalize();

    up.Normalize();

    cross = direction ^ up;
    cross.Normalize();

    up = (cross ^ direction).GetNormalized();

    // set up camera transformation (only need to rotate coordinates)
    Matrix rotate;
    rotate.Set(cross,
               up,
               -direction);
    Transform(rotate);
}