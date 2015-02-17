//
// Copyright (c) 2013-2015 Antti Karhu.
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

#ifndef BoundingBox_H
#define BoundingBox_H

#include "Math/Vector3.h"
#include "Math/MathFunctions.h"
#include "Math/Matrix4x4.h"
#include "Math/Sphere.h"
#include "Math/Rect.h"

namespace Huurre3D
{

class BoundingBox
{
public:
    BoundingBox() = default;
    BoundingBox(const Vector3& min, const Vector3& max);
    BoundingBox(float min, float max);
    BoundingBox(const BoundingBox& box);
    BoundingBox(const Sphere& sphere);
    ~BoundingBox() = default;

    BoundingBox& operator = (const BoundingBox& rhs);
    void set(const Vector3& min, const Vector3& max);
    void set(const Sphere& sphere);
    Intersection isInside(const Vector3& point) const;
    Intersection isInside(const BoundingBox& box) const;
    bool isInsideNoIntersection(const BoundingBox& box) const;
    void mergePoint(const Vector3& point);
    void mergePoints(const Vector3* points, unsigned int count);
    void mergePoints(const float* points, unsigned int count);
    void mergeBoundingBox(const BoundingBox& box);
    void transform(const Matrix4x4& transform);
    BoundingBox transformed(const Matrix4x4& transform) const;
    float distanceToClosestPoint(const Vector3& point) const;
    float distanceToCenterPoint(const Vector3& point) const;
    Rect project2D(const Matrix4x4& projection) const;
    Vector3 getCenter() const {return (max + min) * 0.5f;}
    Vector3 getSize() const {return max - min;}
    Vector3 getHalfSize() const {return (max - min) * 0.5f;}
    Vector3 getMin() const {return min;}
    Vector3 getMax() const {return max;}

private:
    Vector3 min = Vector3(INF, INF, INF);
    Vector3 max = Vector3(-INF, -INF, -INF);
};

}

#endif