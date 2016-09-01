/**
 * @file   ImagePlane.h
 * @author Odysseus Georgoudis
 * @date   02/09/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_IMAGEPLANE_H
#define RAYTRACING_IMAGEPLANE_H

#include "../math/cyMath.h"

class Camera;

class ImagePlane
{
public:
    explicit ImagePlane(const Camera& camera);
    ImagePlane(int width, int height, float fov);

    const Point& getDXV() const        { return dXV_; }
    const Point& getDYV() const        { return dYV_; }
    const Point& getFirstPixel() const { return firstPixel_; }

    int getWidth() const      { return width_; }
    int getHeight() const     { return height_; }
    int getSize() const       { return size_; }

    Point dXV_;
    Point dYV_;
    Point firstPixel_;
    int width_;
    int height_;
    int size_;

    // static definitions
    static constexpr float divideMPI_ = M_PI / 180.0f;
    static constexpr float imageDistance_ = 1.0f;
};


#endif //RAYTRACING_IMAGEPLANE_H
