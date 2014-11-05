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

#ifndef Quaternion_H
#define Quaternion_H

#include "Math/Matrix3x3.h"

namespace Huurre3D
{

class Quaternion
{
public:
    Quaternion() = default;
    Quaternion(const Quaternion& quat);
    Quaternion(float w, float x, float y, float z);
    Quaternion(float angle, const Vector3& axis);
    //Construct from orthonormal axes.
    Quaternion(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
    //Construct from a rotation matrix.
    Quaternion(const Matrix3x3& matrix);
    ~Quaternion() = default;

    Quaternion& operator = (const Quaternion& rhs);
    bool operator == (const Quaternion& rhs) const;
    bool operator != (const Quaternion& rhs) const;
    Quaternion operator + (const Quaternion& rhs) const;
    Quaternion& operator += (const Quaternion& rhs);
    Quaternion operator - (const Quaternion& rhs) const;
    Quaternion operator * (float scalar) const;
    Quaternion&  operator *= (float scalar);
    Quaternion operator * (const Quaternion& rhs) const;
    void set(float angle, const Vector3& axis);
    //Set from a rotation matrix.
    void set(const Matrix3x3& rotation);
    //Set from orthonormal axes.
    void set(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
    Matrix3x3 rotationMatrix3x3() const;
    float length() const;
    float lengthSquared() const;
    float normalize();
    Quaternion normalized() const;
    //Rotates the given vector with this quaternion.
    Vector3 rotate(Vector3 rhs);
    Quaternion conjugated() const;
    Quaternion inverse() const;

    static const Quaternion IDENTITY;
    static const Quaternion ZERO;

    float w;
    float x;
    float y;
    float z;
};

}

#endif