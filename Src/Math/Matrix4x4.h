//
// Copyright (c) 2013-2014 Antti Karhu.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef Matrix4x4_H
#define Matrix4x4_H

#include "Math/Vector4.h"

namespace Huurre3D
{

class Quaternion;
class Matrix3x3;
class Vector3;

class Matrix4x4
{
public:
    Matrix4x4() = default;
    Matrix4x4(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3);
    Matrix4x4(const Matrix4x4& matrix);
    ~Matrix4x4() = default;

    const Vector4& operator [] (int index) const;
    Vector4& operator [] (int index);
    Matrix4x4& operator = (const Matrix4x4& rhs);
    bool operator == (const Matrix4x4& rhs) const;
    bool operator != (const Matrix4x4& rhs) const;
    Matrix4x4 operator + (const Matrix4x4& rhs) const;
    Matrix4x4& operator += (const Matrix4x4& rhs);
    Matrix4x4 operator - (const Matrix4x4& rhs) const;
    Matrix4x4& operator -= (const Matrix4x4& rhs);
    Matrix4x4 operator - () const;
    //Matrix multiplication, uses post-multiplication. M3 * (M2 * M1).
    Matrix4x4 operator * (const Matrix4x4& rhs) const;
    //Matrix multiplication assignement, uses post-multiplication. M3 * (M2 * M1).
    Matrix4x4& operator *= (const Matrix4x4& rhs);
    Matrix4x4 operator * (float scalar) const;
    Matrix4x4& operator *= (float scalar);
    //Vector multiplication, uses post-multiplication. M3 * (M2 * (M1 * V)).
    Vector4 operator * (const Vector4& rhs) const;
    float determinant() const;
    Matrix4x4 transpose() const;
    //Inverse, inverted using Cramer's rule. M_inv = (1 / det(M)) * adj(M).
    Matrix4x4 inverse() const;
    //Composes a transform matrix from position, rotation and scale.
    void setTransform(const Vector3& position, const Quaternion &rotation, const Vector3& scale);
    //Composes an inverse transform matrix from position, rotation and scale.
    void setInverseTransform(const Vector3& position, const Quaternion &rotation, const Vector3& scale);
    //Composes projective matrix from given components.
    void setPerspective(float fov, float aspect, float near, float far);
    //Composes orthogonal matrix from given components.
    void setOrtho(float left, float right, float bottom, float top, float near, float far);
    //Composes a look-at-matrix from the given parameters
    void setLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
    const float* toArray() const { return cols[0].toArray(); }
    float* toArray() { return cols[0].toArray(); }

    static const Matrix4x4 ZERO;
    static const Matrix4x4 IDENTITY;

private:
    //Matrix elemets presented as column vectors, uses the column-major order notation:
    //0 4 8  12
    //1 5 9  13
    //2 6 10 14
    //3 7 11 15
    Vector4 cols[4];
};

}

#endif