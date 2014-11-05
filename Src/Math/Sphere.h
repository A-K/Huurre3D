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

#ifndef Sphere_H
#define Sphere_H

#include "Math/Vector3.h"

namespace Huurre3D
{

class Sphere
{
public:
    Sphere() = default;
    Sphere(const Sphere& sphere);
    Sphere(const Vector3& center, float radius);
    ~Sphere() = default;

    void set(const Sphere& sphere);
    void set(const Vector3& center, float radius);
    Sphere translated(const Vector3& translation) const;
    Vector3 getCenter() const {return center;}
    float getRadius() const {return radius;}

private:
    Vector3 center = Vector3::ZERO;
    float radius = 0.0f;
};

}

#endif