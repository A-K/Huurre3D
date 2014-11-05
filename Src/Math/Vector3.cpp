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

#include "Math/Vector3.h"
#include "Math/MathFunctions.h"

namespace Huurre3D
{

//Initialize static variables
const Vector3 Vector3::ZERO(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::UNIT_X(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UNIT_Y(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UNIT_Z(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::ONE(1.0f, 1.0f, 1.0f);

Vector3::Vector3(float x, float y,float z):
x(x), y(y), z(z) 
{
}

Vector3::Vector3(const Vector3& vector):
x(vector.x), y(vector.y), z(vector.z) 
{
}

Vector3& Vector3::operator = (const Vector3& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

bool Vector3::operator == (const Vector3& rhs) const
{
    return equals(x, rhs.x) && equals(y, rhs.y) && equals(z, rhs.z);
}

bool Vector3::operator != (const Vector3& rhs) const
{
    return !equals(x, rhs.x) || !equals(y, rhs.y) || !equals(z, rhs.z);
}

Vector3 Vector3::operator + (const Vector3& rhs) const
{
    return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3& Vector3::operator += (const Vector3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vector3 Vector3::operator - (const Vector3& rhs) const
{
    return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3& Vector3::operator -= (const Vector3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vector3 Vector3::operator - () const
{
    return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator * (float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3& Vector3::operator *= (float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3 Vector3::operator / (float scalar) const
{
    float invScalar = 1.0f / scalar;
    return Vector3(x * invScalar, y * invScalar, z * invScalar);
}

Vector3& Vector3::operator /= (float scalar)
{
    float invScalar = 1.0f / scalar;

    x *= invScalar;
    y *= invScalar;
    z *= invScalar;
    return *this;
}

Vector3 Vector3::operator * (const Vector3& rhs)
{
    return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
}

Vector3& Vector3::operator *= (const Vector3& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

Vector3 Vector3::absolute() const
{
    return Vector3(abs(x), abs(y), abs(z));
}

float Vector3::dot(const Vector3& rhs) const
{
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

float Vector3::absDot(const Vector3& rhs) const
{
    return abs(x * rhs.x) + abs(y * rhs.y) + abs(z * rhs.z);
}

Vector3 Vector3::cross(const Vector3& rhs) const
{
    return Vector3(y * rhs.z - z * rhs.y,
                   z * rhs.x - x * rhs.z,
                   x * rhs.y - y * rhs.x);

}

float Vector3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

float Vector3::lengthSquared() const
{
    return x * x + y * y + z * z;
}

float Vector3::distance(const Vector3& rhs) const
{
    return (*this - rhs).length();
}

float Vector3::distanceSquared(const Vector3& rhs) const
{
    return (*this - rhs).lengthSquared();
}

float Vector3::normalize()
{
    float len = length();

    if (len >= EPSILON)
    {
        *this /= len;
        return len;
    }
    else
    {
        *this = ZERO;
        return 0.0f;
    }
}

Vector3 Vector3::normalized() const
{
    Vector3 copy = *this;
    copy.normalize();
    return copy;
}

Vector3 Vector3::lerp(const Vector3& rhs, float weight) const
{
    return *this * (1.0f - weight) + rhs * weight;
}

}