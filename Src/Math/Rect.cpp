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

#include "Math/Rect.h"
#include "Math/MathFunctions.h"

namespace Huurre3D
{

Rect::Rect(const Rect &rect)
{
    set(rect);
}

Rect::Rect(const Vector2 &min, const Vector2 &max)
{
    set(min, max);
}

Rect::Rect(float left, float bottom, float right, float top)
{
    set(left, bottom, right, top);
}

void Rect::set(const Rect &rect)
{
    min = rect.min;
    max = rect.max;
}

void Rect::set(const Vector2 &min, const Vector2 &max)
{
    this->min = min;
    this->max = max;
}

void Rect::set(float left, float bottom, float right, float top)
{
    min.x = left;
    min.y = bottom;
    max.x = right;
    max.y = top;
}

void Rect::mergePoint(const Vector2& point)
{
    if(point.x < min.x)
        min.x = point.x;
    if(point.x > max.x)
        max.x = point.x;
    if(point.y < min.y)
        min.y = point.y;
    if(point.y > max.y)
        max.y = point.y;
}

bool Rect::overlap(const Rect& rect) const
{
    return (min.x <= rect.max.x && max.x >= rect.min.x && min.y <= rect.max.y && max.y >= rect.min.y);
}

}