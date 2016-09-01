/**
 * @file   Transformation.h
 * @author Odysseus Georgoudis
 * @date   31/08/2016
 * @brief  Transformation class definition (how to change between spaces)
 *
 */

#ifndef RAYTRACING_TRANSFORMATION_H
#define RAYTRACING_TRANSFORMATION_H

#include "../math/cyMath.h"

class Transformation
{
public:
    /**
     * Constructor (identity transformation)
     */
    Transformation()
    {
        InitTransform();
    }

    virtual ~Transformation() = default;

    const Matrix& GetTransform() const { return mat_; }
    const Point& GetPosition() const { return pos_; }
    const Matrix& GetInverseTransform() const { return imat_; }

    /**
     * Set the translation of the local coordinate system
     * @param p
     */
    void Translate(const Point& p) { pos_ += p; }

    /**
     * Update the local coordinate system transformation matrix
     * @param m
     */
    void Transform(const Matrix &m);

    /**
     * Create an initial (identity) transformation matrix
     */
    void InitTransform();

    /**
     * Set the rotation of the local coordinate system
     * (about some axis and an amount (degrees) to rotate)
     * @param axis
     * @param degree
     */
    void Rotate(const Point& axis, float degree);

    /**
     * Set the scale of the local coordinate system
     * @param sx
     * @param sy
     * @param sz
     */
    void Scale(float sx, float sy, float sz);

    /**
     * Transform into local coordinate system
     * @param p
     * @return
     */
    Point TransformTo(const Point& p) const
    {
        return Point { imat_ * (p - pos_) };
    }

    /**
     * Transform from local coordinate system
     * @param p
     * @return
     */
    Point TransformFrom(const Point& p) const
    {
        return Point { mat_ * p + pos_ };
    }

    /**
     * Transform vector to local coordinate system
     * @param dir
     * @return
     */
    Point VecTransformTo(const Point& dir) const
    {
        return multiplyTranspose_(mat_, dir);
    }

    /**
     * Transform vector from local coordinate system
     * @param dir
     * @return
     */
    Point VecTransformFrom(const Point& dir) const
    {
        return multiplyTranspose_(imat_, dir);
    }

private:
    Matrix mat_;  ///< transformation matrix (to some coordinate system)
    Matrix imat_; ///< inverse of transformation matrix (cached)
    Point  pos_;  ///< translation part of transformation

    /**
     * Multiplies given vector with transpose of the matrix
     * @param m
     * @param dir
     * @return
     */
    static Point multiplyTranspose_(const Matrix& m, const Point& dir)
    {
        return Point { m.GetColumn(0) % dir,
                       m.GetColumn(1) % dir,
                       m.GetColumn(2) % dir };
    }
};



#endif //RAYTRACING_TRANSFORMATION_H
