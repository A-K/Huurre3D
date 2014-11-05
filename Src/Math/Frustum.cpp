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

#include "Math/Frustum.h"

namespace Huurre3D
{

Frustum::Frustum(const Matrix4x4& matrix)
{
    set(matrix, true);
}

void Frustum::set(const Matrix4x4& matrix, bool normalize)
{
    planes[LEFT_PLANE].set(matrix[3] + matrix[0]);
    planes[RIGHT_PLANE].set(matrix[3] - matrix[0]);
    planes[BOTTOM_PLANE].set(matrix[3] + matrix[1]);
    planes[TOP_PLANE].set(matrix[3] - matrix[1]);
    planes[NEAR_PLANE].set(matrix[3] + matrix[2]);
    planes[FAR_PLANE].set(matrix[3] - matrix[2]);
	
    if(normalize)
    {
        for(unsigned int i = 0; i < NUM_FRUSTUM_PLANES; ++i)
            planes[i].normalize();
    }

    //calculateCorners();
}

Intersection Frustum::isInside(const Vector3& point) const
{
    for(unsigned int i = 0; i < NUM_FRUSTUM_PLANES; ++i)
    {
        if(planes[i].distance(point) < 0.0f)
            return Intersection::Outside;
    }
        
    return Intersection::Inside;
}

Intersection Frustum::isInside(const Sphere& sphere) const
{
    Intersection result = Intersection::Inside;
    float dist = 0;
    float radius = sphere.getRadius();
    Vector3 center = sphere.getCenter();

    for(unsigned int i = 0; i < NUM_FRUSTUM_PLANES; ++i)
    {
        dist = planes[i].distance(center);
        if(dist < -radius)
            return Intersection::Outside;
        else if(dist < radius)
            result = Intersection::Intersects;
    }

    return result;
}

Intersection Frustum::isInside(const BoundingBox& box) const
{
    Intersection result = Intersection::Inside;
    Vector3 center = box.getCenter();
    Vector3 edge = center - box.getMin();
        
    for(unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
    {
        float dist = planes[i].distance(center);
        float absDist = planes[i].getNormal().absDot(edge);
            
        if(dist < -absDist)
            return Intersection::Outside;
        else if(dist < absDist)
            result = Intersection::Intersects;
    }
        
    return result;
}

bool Frustum::isInsideNoIntersection(const Sphere& sphere) const
{
    float radius = sphere.getRadius();
    Vector3 center = sphere.getCenter();

    for(unsigned int i = 0; i < NUM_FRUSTUM_PLANES; ++i)
    {
        if(planes[i].distance(center) < -radius)
            return false;
    }

    return true;
}

bool Frustum::isInsideNoIntersection(const BoundingBox& box) const
{
    Vector3 center = box.getCenter();
    Vector3 edge = center - box.getMin();
        
    for(unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
    {       
        if(planes[i].distance(center) < -planes[i].getNormal().absDot(edge))
            return false;
    }
        
    return true;
}

const FixedArray<Vector3, 8>& Frustum::getCorners()
{
    calculateCorners();
    return corners;
}

void Frustum::calculateCorners()
{
    corners[0] = getIntersectionPoint(planes[NEAR_PLANE], planes[LEFT_PLANE], planes[TOP_PLANE]);
    corners[1] = getIntersectionPoint(planes[NEAR_PLANE], planes[RIGHT_PLANE], planes[TOP_PLANE]);
    corners[2] = getIntersectionPoint(planes[NEAR_PLANE], planes[LEFT_PLANE], planes[BOTTOM_PLANE]);
    corners[3] = getIntersectionPoint(planes[NEAR_PLANE], planes[RIGHT_PLANE], planes[BOTTOM_PLANE]);
    corners[4] = getIntersectionPoint(planes[FAR_PLANE], planes[LEFT_PLANE], planes[TOP_PLANE]);
    corners[5] = getIntersectionPoint(planes[FAR_PLANE], planes[RIGHT_PLANE], planes[TOP_PLANE]);
    corners[6] = getIntersectionPoint(planes[FAR_PLANE], planes[LEFT_PLANE], planes[BOTTOM_PLANE]);
    corners[7] = getIntersectionPoint(planes[FAR_PLANE], planes[RIGHT_PLANE], planes[BOTTOM_PLANE]);
}

Vector3 Frustum::getIntersectionPoint(const Plane& plane1, const Plane& plane2, const Plane& plane3)
{
    Vector3 normal1 = plane1.getNormal();
    Vector3 normal2 = plane2.getNormal();
    Vector3 normal3 = plane3.getNormal();

    float denom = normal1.dot(normal2.cross(normal3));

    return Vector3(normal2.cross(normal3) * plane1.getOffset() +
                   normal3.cross(normal1) * plane2.getOffset() +
                   normal1.cross(normal2) * plane3.getOffset()) / -denom;
}

}