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

#ifndef Rect_H
#define Rect_H

#include "Math/Vector2.h"
#include "Math/MathFunctions.h"

namespace Huurre3D
{

class Rect
{
public:
    Rect() = default;
    Rect(const Rect &rect);
    Rect(const Vector2 &min, const Vector2 &max);
    Rect(float left, float bottom, float right, float top);
    ~Rect() = default;

    void set(const Rect &rect);
    void set(const Vector2 &min, const Vector2 &max);
    void set(float left, float bottom, float right, float top);
    void mergePoint(const Vector2& point);
    bool overlap(const Rect& rect) const;
    const Vector2& getMin() const {return min;}
    const Vector2& getMax() const {return max;}

private:
    Vector2 min = Vector2(INF, INF);
    Vector2 max = Vector2(-INF, -INF);
};

}

#endif
