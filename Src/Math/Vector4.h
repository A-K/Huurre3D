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

#ifndef Vector4_H
#define Vector4_H

#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace Huurre3D
{

class Vector4
{
public:
    Vector4() = default;
    Vector4(float x, float y,float z, float w);
    Vector4(const Vector4& vector);
    Vector4(const Vector3& vector, float w);
    Vector4(const Vector2& vector1, const Vector2& vector2);
    Vector4(const float* data);
    ~Vector4() = default;

    Vector4& operator = (const Vector4& rhs);
    bool operator == (const Vector4& rhs) const;
    bool operator != (const Vector4& rhs) const;
    Vector4 operator + (const Vector4& rhs) const;
    Vector4& operator += (const Vector4& rhs);
    Vector4 operator - (const Vector4& rhs) const;
    Vector4& operator -= (const Vector4& rhs);
    Vector4 operator -() const;
    Vector4 operator * (float scalar) const;
    Vector4& operator *= (float scalar);
    Vector4 operator / (float scalar) const;
    Vector4& operator /= (float scalar);
    void set( float x, float y, float z, float w);
    Vector4 absolute() const;
    float dot(const Vector4& rhs) const;
    float absDot(const Vector4& rhs) const;
    Vector4 cross(const Vector4& rhs1, const Vector4& rhs2) const;
    //Cross product for the xyz part. Computes the cross product in 3D (x, y, z) and w=0.
    Vector4 cross3D(const Vector4& rhs) const;
    //Returns the (x, y, z) part of this vector.
    Vector3 xyz() const;
    //Projects this vector to vector3.
    Vector3 project3D() const;
    float length() const;
    float lengthSquared() const;
    float normalize();
    Vector4 normalized() const;
    //Linearly interpolates between this and another vector.
    Vector4 lerp(const Vector4& b, float t) const;
    //Returns the x, y, z as a float array.
    const float* toArray() const { return &x; }
    float* toArray() { return &x; }

    float x;
    float y;
    float z;
    float w;

    static const Vector4 ZERO;
    static const Vector4 UNIT_X;
    static const Vector4 UNIT_Y;
    static const Vector4 UNIT_Z;
    static const Vector4 UNIT_W;
    static const Vector4 ONE;
};

}

#endif
