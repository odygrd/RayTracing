/**
 * @file   Transformation.cpp
 * @author Odysseus Georgoudis
 * @date   31/08/2016
 * @brief  
 *
 */

#include "Transformation.h"

void Transformation::Transform(const Matrix &m)
{
    mat_ *= m;
    pos_ = m * pos_;
    mat_.GetInverse(imat_);
}

void Transformation::InitTransform()
{
    pos_.Zero();
    mat_.SetIdentity();
    imat_.SetIdentity();
}

void Transformation::Rotate(const Point& axis, float degree)
{
    Matrix m;
    m.SetRotation(axis, degree * (float) M_PI / 180.0);
    Transform(m);
}

void Transformation::Scale(float sx, float sy, float sz)
{
    Matrix m;
    m.Zero();
    m[0] = sx;
    m[4] = sy;
    m[8] = sz;
    Transform(m);
}