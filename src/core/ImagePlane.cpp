/**
 * @file   ImagePlane.cpp
 * @author Odysseus Georgoudis
 * @date   02/09/2016
 * @brief  
 *
 */

#include "ImagePlane.h"
#include "../cameras/Camera.h"

ImagePlane::ImagePlane(int width, int height, float fov)
    : width_ { width },
      height_ { height },
      size_ { width_ * height_ }
{
    // Variables for camera ray generations
    float fov_ { fov * divideMPI_ };
    float aspectRatio { width_ / static_cast<float>(height_) };

    // viewPlaneHalfWidth
    float imageTipY { imageDistance_ * static_cast<float>(tan(fov_ / 2.0)) };
    // viewPlaneHalfHeight
    float imageTipX { imageTipY * aspectRatio };

    float dX { (2.0f * imageTipX) / static_cast<float> (width_) };
    float dY { (2.0f * imageTipY) / static_cast<float> (height_) };

    Point imageTopLeftV { -imageTipX, imageTipY, -imageDistance_ };

    dXV_.Set(dX, 0.0, 0.0);
    dYV_.Set(0.0, -dY, 0.0 );

    firstPixel_ = imageTopLeftV + (dXV_ * 0.5) + (dYV_ * 0.5);
}

ImagePlane::ImagePlane(const Camera& camera)
    :ImagePlane(camera.imageWidth,
                camera.imageHeight,
                camera.fov)
{}