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

#include "Math/BoundingBox.h"
#include "Util/FixedArray.h"
#include <iostream>

namespace Huurre3D
{

BoundingBox::BoundingBox(const Vector3& min, const Vector3& max)
{
    set(min, max);
}

BoundingBox::BoundingBox(float min, float max):
BoundingBox(Vector3(min, min, min), Vector3(max, max, max))
{
}

BoundingBox::BoundingBox(const Sphere& sphere)
{
    set(sphere);
}

BoundingBox::BoundingBox(const BoundingBox& box)
{
    min = box.min;
    max = box.max;
}

BoundingBox& BoundingBox::operator = (const BoundingBox& rhs)
{
    min = rhs.min;
    max = rhs.max;
    return *this;
}

void BoundingBox::set(const Vector3& min, const Vector3& max)
{
    this->min = min;
    this->max = max;
}

void BoundingBox::set(const Sphere& sphere)
{
    Vector3 center = sphere.getCenter();
    float radius = sphere.getRadius();
    
    min = center + Vector3(-radius, -radius, -radius);
    max = center + Vector3(radius, radius, radius);
}

Intersection BoundingBox::isInside(const Vector3& point) const
{
    return (point.x < min.x || point.x > max.x || point.y < min.y || point.y > max.y || point.z < min.z || point.z > max.z) ? Intersection::Outside : Intersection::Inside;
}

Intersection BoundingBox::isInside(const BoundingBox& box) const
{
    Intersection result;

    if(box.max.x < min.x || box.min.x > max.x || box.max.y < min.y || box.min.y > max.y || box.max.z < min.z || box.min.z > max.z)
        result = Intersection::Outside;
    else if(box.min.x < min.x || box.max.x > max.x || box.min.y < min.y || box.max.y > max.y ||box.min.z < min.z || box.max.z > max.z)
        result = Intersection::Intersects;
    else
        result = Intersection::Inside;

    return result;
}

bool BoundingBox::isInsideNoIntersection(const BoundingBox& box) const
{
    return (box.max.x < min.x || box.min.x > max.x || box.max.y < min.y || box.min.y > max.y || box.max.z < min.z || box.min.z > max.z);
}

void BoundingBox::mergePoint(const Vector3& point)
{
    if(point.x < min.x)
        min.x = point.x;
    if(point.y < min.y)
        min.y = point.y;
    if(point.z < min.z)
        min.z = point.z;
    if(point.x > max.x)
        max.x = point.x;
    if(point.y > max.y)
        max.y = point.y;
    if(point.z > max.z)
        max.z = point.z;
}

void BoundingBox::mergePoints(const Vector3* points, unsigned int count)
{
    for(unsigned int i = 0; i < count; ++i)
        mergePoint(points[i]);
}

void BoundingBox::mergePoints(const float* points, unsigned int count)
{
    for(unsigned int i = 0; i < count; ++i)
        mergePoint(Vector3(points[i * 3], points[i * 3 + 1], points[i * 3 + 2]));
}

void BoundingBox::mergeBoundingBox(const BoundingBox& box)
{
    if(box.min.x < min.x)
        min.x = box.min.x;
    if(box.min.y < min.y)
        min.y = box.min.y;
    if(box.min.z < min.z)
        min.z = box.min.z;
    if(box.max.x > max.x)
        max.x = box.max.x;
    if(box.max.y > max.y)
        max.y = box.max.y;
    if(box.max.z > max.z)
        max.z = box.max.z;
}

void BoundingBox::transform(const Matrix4x4& transform)
{
    Vector3 newCenter = (transform * Vector4(getCenter(), 1.0)).xyz();
    Vector3 halfSize = getHalfSize();
    
    Vector3 newHalfSize = Vector3(
        transform[0].xyz().absolute() * halfSize.x + 
        transform[1].xyz().absolute() * halfSize.y + 
        transform[2].xyz().absolute() * halfSize.z);

    set(newCenter - newHalfSize, newCenter + newHalfSize);
}

BoundingBox BoundingBox::transformed(const Matrix4x4& transform) const
{
    BoundingBox copy = *this;
    copy.transform(transform);
    return copy;
}

float BoundingBox::distanceToClosestPoint(const Vector3& point) const
{
    //Get the closest point
    Vector3 closestPoint = clamp(point, min, max);
    return closestPoint.distance(point);
}

float BoundingBox::distanceToCenterPoint(const Vector3& point) const
{
    return getCenter().distance(point);
}

Rect BoundingBox::project2D(const Matrix4x4& projection) const
{
    Vector3 projMin = min;
    Vector3 projMax = max;
    
    //     2+------+3
    //     /|     /|
    //    / |    / |
    //   / 0+---/--+1
    // 6+------+7 /
    //  | /    | /
    //  |/     |/
    // 4+------+5

    FixedArray<Vector4, 8> corners;
    corners[0] = Vector4(projMin, 1.0);
    corners[1] = Vector4(projMax.x, projMin.y, projMin.z, 1.0);
    corners[2] = Vector4(projMin.x, projMax.y, projMin.z, 1.0);
    corners[3] = Vector4(projMax.x, projMax.y, projMin.z, 1.0);
    corners[4] = Vector4(projMin.x, projMin.y, projMax.z, 1.0);
    corners[5] = Vector4(projMax.x, projMin.y, projMax.z, 1.0);
    corners[6] = Vector4(projMin.x, projMax.y, projMax.z, 1.0);
    corners[7] = Vector4(projMax, 1.0);
   
    Rect rect;
    for(unsigned int i = 0; i < corners.size(); ++i)
    {
        Vector4 projected = projection * corners[i];
        rect.mergePoint(Vector2(projected.x / projected.w, projected.y / projected.w));
    }

    return rect;
}

}