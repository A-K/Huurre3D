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

#ifndef Vector3_H
#define Vector3_H

namespace Huurre3D
{

class Vector3
{
public:
    Vector3() = default;
    Vector3(float x, float y,float z);
    Vector3(const Vector3& vector); 
    ~Vector3() = default;

    Vector3& operator = (const Vector3& rhs);
    bool operator == (const Vector3& rhs) const;
    bool operator != (const Vector3& rhs) const;
    Vector3 operator + (const Vector3& rhs) const;
    Vector3& operator += (const Vector3& rhs);
    Vector3 operator - (const Vector3& rhs) const;
    Vector3& operator -= (const Vector3& rhs);
    Vector3 operator -() const;
    Vector3 operator * (float scalar) const;
    Vector3& operator *= (float scalar);
    Vector3 operator / (float scalar) const;
    Vector3& operator /= (float scalar);
    Vector3 operator * (const Vector3& rhs);
    Vector3& operator *= (const Vector3& rhs);
    Vector3 absolute() const;
    float dot(const Vector3& rhs) const;
    float absDot(const Vector3& rhs) const;
    Vector3 cross(const Vector3& rhs) const;
    float length() const;
    float lengthSquared() const;
    float distance(const Vector3& rhs) const;
    float distanceSquared(const Vector3& rhs) const;
    float normalize();
    Vector3 normalized() const;
    //Linearly interpolates between this and another vector.
    Vector3 lerp(const Vector3& rhs, float weight) const;
    //Returns the x, y, z as a float array.
    const float* toArray() const {return &x;}
    float* toArray() {return &x;}

    float x;
    float y;
    float z;

    static const Vector3 ZERO;
    static const Vector3 UNIT_X;
    static const Vector3 UNIT_Y;
    static const Vector3 UNIT_Z;
    static const Vector3 ONE;
};

}

#endif