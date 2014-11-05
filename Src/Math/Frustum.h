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

#ifndef Frustum_H
#define Frustum_H

#include "Math/Plane.h"
#include "Math/MathFunctions.h"
#include "Math/Matrix4x4.h"
#include "Math/BoundingBox.h"
#include "Math/Sphere.h"
#include "Util/FixedArray.h"

namespace Huurre3D
{

enum FrustumPlane
{
    LEFT_PLANE = 0,
    RIGHT_PLANE,
    BOTTOM_PLANE,
    TOP_PLANE,
    NEAR_PLANE,
    FAR_PLANE,
    NUM_FRUSTUM_PLANES
};

class Frustum
{
public:
    Frustum() = default;
    Frustum(const Matrix4x4& matrix);
    ~Frustum() = default;

    void set(const Matrix4x4& matrix, bool normalize = true);
    Intersection isInside(const Vector3& point) const;
    Intersection isInside(const Sphere& sphere) const;
    Intersection isInside(const BoundingBox& box) const;
    bool isInsideNoIntersection(const Sphere& sphere) const;
    bool isInsideNoIntersection(const BoundingBox& box) const;
    const FixedArray<Vector3, 8>& getCorners();

private:
    void calculateCorners();
    Vector3 getIntersectionPoint(const Plane& plane1, const Plane& plane2, const Plane& plane3);
    Plane planes[NUM_FRUSTUM_PLANES];
    FixedArray<Vector3, 8> corners;
};

}

#endif