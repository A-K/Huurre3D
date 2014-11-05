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

#include "Math/Quaternion.h"
#include "Math/MathFunctions.h"

namespace Huurre3D
{

//Initialize static variables.
const Quaternion Quaternion::ZERO(0, 0, 0, 0);
const Quaternion Quaternion::IDENTITY(1, 0, 0, 0);

Quaternion::Quaternion(const Quaternion& quat):
w(quat.w), x(quat.x), y(quat.y), z(quat.z)
{
}

Quaternion::Quaternion(float w, float x, float y, float z):
w(w), x(x), y(y), z(z)
{
}

Quaternion::Quaternion(float angle, const Vector3& axis)
{
    set(angle, axis);
}

Quaternion::Quaternion(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
{
    set(xAxis, yAxis, zAxis);
}

Quaternion::Quaternion(const Matrix3x3& matrix)
{
    set(matrix);
}

Quaternion& Quaternion::operator = (const Quaternion& rhs)
{
    w = rhs.w;
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

bool Quaternion::operator == (const Quaternion& rhs) const
{
    return equals(w, rhs.w) && equals(x, rhs.x) && equals(y, rhs.y) && equals(z, rhs.z);
}

bool Quaternion::operator != (const Quaternion& rhs) const
{
    return !equals(w, rhs.w) && !equals(x, rhs.x) || !equals(y, rhs.y) || !equals(z, rhs.z);
}

Quaternion Quaternion::operator + (const Quaternion& rhs) const
{
    return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
}

Quaternion& Quaternion::operator += (const Quaternion& rhs)
{
	 w += rhs.w;
     x += rhs.x;
     y += rhs.y;
     z += rhs.z;
     return *this;
}

Quaternion Quaternion::operator - (const Quaternion& rhs) const
{
    return Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
}

Quaternion Quaternion::operator * (float scalar) const
{
    return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
}

Quaternion&  Quaternion::operator *= (float scalar)
{
    w *= scalar;
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Quaternion Quaternion::operator * (const Quaternion& rhs) const
{
    return Quaternion(w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
                      w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,  
                      w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z, 
                      w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x);
}

void Quaternion::set(float angle, const Vector3& axis)
{
    if(isZero(axis.x) && isZero(axis.y) && isZero(axis.z)) 
    {
        *this = IDENTITY;
    }
    else 
    {
        Vector3 normAxis = axis.normalized();
        Vector2 sincos = sinCos(angle * 0.5f * DEGTORAD);
        	
        w = sincos.y;
        x = normAxis.x * sincos.x;
        y = normAxis.y * sincos.x;
        z = normAxis.z * sincos.x;     	
    }
}

void Quaternion::set(const Matrix3x3& rotation)
{
    float trace = rotation.trace();
		
    if(trace > 0.0f)
    {
        float s = 0.f / sqrt(trace + 1.0f);
         w = 0.25f / s;
         x = (rotation[2].y - rotation[1].z) * s;
         y = (rotation[0].z - rotation[2].x) * s;
         z = (rotation[1].x - rotation[0].y) * s;
    }
    else if((rotation[0].x > rotation[1].y) && (rotation[0].x > rotation[2].z))
    {
        float s = sqrt(rotation[0].x - rotation[1].y - rotation[2].z + 1.0f) * 2;
        float invS = 1.0f / s;
        w = (rotation[2].y - rotation[1].z) * invS;
        x = 0.25f * s;
        y = (rotation[0].y + rotation[1].x) * invS;
        z = (rotation[0].z + rotation[2].x) * invS;		
    }
    else if(rotation[1].y > rotation[2].z)
    {
        float s = sqrt(rotation[1].y - rotation[0].x - rotation[2].z + 1.0f) * 2;
        float invS = 1.0f / s;
        w = (rotation[0].z - rotation[2].x) * invS;
        x = (rotation[0].y + rotation[1].x) * invS;
        y = 0.25f * s;
        z = (rotation[1].y + rotation[2].y) * invS;		
    }
    else
    {
        float s = sqrt(rotation[2].z - rotation[0].x - rotation[1].y + 1.0f) * 2;
        float invS = 1.0f / s;
        w = (rotation[1].x - rotation[0].x) * invS;
        x = (rotation[0].z + rotation[2].x) * invS;
        y = (rotation[1].z + rotation[2].y) * invS;
        z = 0.25f * s;
    }
}

void Quaternion::set(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
{
    Matrix3x3 matrix = Matrix3x3(xAxis, yAxis, zAxis);
    set(matrix);
}

Matrix3x3 Quaternion::rotationMatrix3x3() const
{
    Vector3 col1 = Vector3(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y + w * z), 2.0f * (x * z - w * y));
    Vector3 col2 = Vector3(2.0f * (x * y -  w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z + w * x));
    Vector3 col3 = Vector3(2.0f * (x * z + w * y), 2.0f * (y * z - w * x), 1.0f - 2.0f * (x * x + y * y));
    return Matrix3x3(col1, col2, col3);
}

float Quaternion::length() const
{
    return sqrt(w * w + x * x + y * y + z * z);
}

float Quaternion::lengthSquared() const
{
    return w * w + x * x + y * y + z * z;
}

float Quaternion::normalize()
{
    float len = length();
		
    if(len > EPSILON)
    {
        float invLen = 1.0f / len;
        *this *= invLen;
        return len;
    }
    else
    {
        *this = IDENTITY;
         return 0.0f;
    }
}

Quaternion Quaternion::normalized() const
{
    Quaternion copy = *this;
    copy.normalize();
    return copy;
}

Vector3 Quaternion::rotate(Vector3 rhs)
{
    //nVidia SDK implementation
    Vector3 quatVec = Vector3(x, y, z);
    Vector3 cross1 = quatVec.cross(rhs);
    Vector3 cross2 = quatVec.cross(cross1);

    cross1 *= (2.0f * w);
    cross2 *= 2.0f;

    return rhs + cross1 + cross2;
}

Quaternion Quaternion::conjugated() const
{
    return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::inverse() const
{
    float lenSquared = lengthSquared();
		
    if(lenSquared > EPSILON)
    {
        float invLenSquared = 1.0f / lenSquared;
        Quaternion conj = conjugated();
        return conj * invLenSquared;
    }
    else
        return IDENTITY;
}

}