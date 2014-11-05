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

#include "Math/Plane.h"
#include "Math/MathFunctions.h"

namespace Huurre3D
{

Plane::Plane(const Vector3 &normal, float offset):
normal(normal), offset(offset)
{
}

Plane::Plane(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
    set(v1, v2, v3);
}

Plane::Plane(const Vector3& normal, const Vector3& point)
{
    set(normal, point);
}

Plane& Plane::operator = (const Plane& rhs)
{
    normal = rhs.normal;
    offset = rhs.offset;
    return *this;
}

bool Plane::operator == (const Plane& rhs) const
{
    return (normal == rhs.normal && equals(offset, rhs.offset));
}

bool Plane::operator!=(const Plane& rhs) const
{
    return (normal != rhs.normal || !equals(offset, rhs.offset));
}

void Plane::set(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
    normal = (v2 - v1).cross(v3 - v1);
    normal.normalize();
    offset = normal.dot(v1);
}

void Plane::set(const Vector3& normal, const Vector3& point)
{
    this->normal = normal;
    offset = normal.dot(point);
}

void Plane::set(const Vector4& plane)
{
    normal.x = plane.x;
    normal.y = plane.y;
    normal.z = plane.z;
    offset = plane.w;
}

float Plane::distance(const Vector3& point) const
{
    return normal.dot(point) + offset;
}

float Plane::normalize()
{
    float len = normal.length();

    if(len >= EPSILON)
    {
        float invLen = 1.0f / len;
        normal *= invLen;
        offset *= invLen;
    }

    return len;
}

}