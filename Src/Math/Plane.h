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

#ifndef Plane_H
#define Plane_H

#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace Huurre3D
{

class Plane
{
public:
    Plane() = default;
    //Construct by specifying the normal and offset/distance
    Plane(const Vector3 &normal, float offset);
    Plane(const Vector3& v1, const Vector3& v2, const Vector3& v3);
    //Construct from a normal vector and a point on the plane
    Plane(const Vector3& normal, const Vector3& point);
    ~Plane() = default;

    Plane& operator = (const Plane& rhs);
    bool operator == (const Plane& rhs) const;
    bool operator != (const Plane& rhs) const;
    void set(const Vector3& v1, const Vector3& v2, const Vector3& v3);
    void set(const Vector3& normal, const Vector3& point);
    //Set from vector4, where (x, y, z) part is normal and w is offset.
    void set(const Vector4& plane);
    //Zero: point is on plane, negative/positive: point is negative/positive side of plane.
    float distance(const Vector3& point) const;
    float normalize();
    const Vector3& getNormal() const {return normal;}
    float getOffset() const {return offset;}

private:
    Vector3 normal;
    float offset;
};

}

#endif