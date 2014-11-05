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

#include "Math/Vector4.h"
#include "Math/MathFunctions.h"

namespace Huurre3D
{

//Initialize static variables
const Vector4 Vector4::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::UNIT_X(1.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::UNIT_Y(0.0f, 1.0f, 0.0f, 0.0f);
const Vector4 Vector4::UNIT_Z(0.0f, 0.0f, 1.0f, 0.0f);
const Vector4 Vector4::UNIT_W(0.0f, 0.0f, 0.0f, 1.0f);
const Vector4 Vector4::ONE(1.0f, 1.0f, 1.0f, 1.0f);

Vector4::Vector4(float x, float y,float z, float w):
x(x), y(y), z(z) , w(w)
{
}

Vector4::Vector4(const Vector4& vector):
x(vector.x), y(vector.y), z(vector.z), w(vector.w) 
{
}

Vector4::Vector4(const Vector3& vector, float w):
x(vector.x), y(vector.y), z(vector.z), w(w)
{
}

Vector4::Vector4(const Vector2& vector1, const Vector2& vector2):
x(vector1.x), y(vector1.y), z(vector2.x), w(vector2.y) 
{
}

Vector4& Vector4::operator = (const Vector4& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
    return *this;
}

bool Vector4::operator == (const Vector4& rhs) const
{
    return equals(x, rhs.x) && equals(y, rhs.y) && equals(z, rhs.z) && equals(w, rhs.w);
}

bool Vector4::operator != (const Vector4& rhs) const
{
    return !equals(x, rhs.x) || !equals(y, rhs.y) || !equals(z, rhs.z) || !equals(w, rhs.w);
}

Vector4 Vector4::operator + (const Vector4& rhs) const
{
    return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

Vector4& Vector4::operator += (const Vector4& rhs)
{
	 x += rhs.x;
     y += rhs.y;
     z += rhs.z;
	 w += rhs.w;
     return *this;
}

Vector4 Vector4::operator - (const Vector4& rhs) const
{
    return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

Vector4& Vector4::operator -= (const Vector4& rhs)
{
	 x -= rhs.x;
     y -= rhs.y;
     z -= rhs.z;
	 w -= rhs.w;
     return *this;
}

Vector4 Vector4::operator -() const
{
    return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator * (float scalar) const
{
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vector4& Vector4::operator *= (float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

Vector4 Vector4::operator / (float scalar) const
{
    float invScalar = 1.f / scalar;
    return Vector4(x * invScalar, y * invScalar, z * invScalar, w * invScalar);
}

Vector4& Vector4::operator /= (float scalar)
{
    float invScalar = 1.f / scalar;

    x *= invScalar;
    y *= invScalar;
    z *= invScalar;
    w *= invScalar;
    return *this;
}

void Vector4::set(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4 Vector4::absolute() const
{
    return Vector4(abs(x), abs(y), abs(z), abs(w));
}

float Vector4::dot(const Vector4& rhs) const
{
    return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

float Vector4::absDot(const Vector4& rhs) const
{
    return abs(x * rhs.x) + abs(y * rhs.y) + abs(z * rhs.z) + abs(w * rhs.w);
}

Vector4 Vector4::cross(const Vector4& rhs1, const Vector4& rhs2) const
{
    //Precompute 2x2 matrices.
    float xy = rhs1.x * rhs2.y - rhs1.y * rhs2.x;
    float xz = rhs1.x * rhs2.z - rhs1.z * rhs2.x;
    float xw = rhs1.x * rhs2.w - rhs1.w * rhs2.x;
    float yz = rhs1.y * rhs2.z - rhs1.z * rhs2.y;
    float yw = rhs1.y * rhs2.w - rhs1.w * rhs2.y;
    float zw = rhs1.z * rhs2.w - rhs1.w * rhs2.z;

    return Vector4(y * zw - z * yw + w * yz,
                   z * xw - x * zw - w * xz,
                   x * yw - y * xw + w * xy,
                   y * xz - x * yz - z * xy);
}

Vector4 Vector4::cross3D(const Vector4& rhs) const
{
    return Vector4(y * rhs.z - z * rhs.y,
                   z * rhs.x - x * rhs.z,
                   x * rhs.y - y * rhs.x,
                   0);

}

Vector3 Vector4::xyz() const
{
    return Vector3(x, y, z);
}

Vector3 Vector4::project3D() const
{
    return Vector3(x, y, z) * (1/w);
}

float Vector4::length() const
{
    return sqrt(x * x + y * y + z * z + w * w);
}

float Vector4::lengthSquared() const
{
    return x * x + y * y + z * z + w * w;
}

float Vector4::normalize()
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

Vector4 Vector4::normalized() const
{
    Vector4 copy = *this;
    copy.normalize();
    return copy;
}

Vector4 Vector4::lerp(const Vector4& rhs, float weight) const
{
    return *this * (1.0f - weight) + rhs * weight;
}

}