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

#ifndef Vector2_H
#define Vector2_H

namespace Huurre3D
{

class Vector2
{
public:
    Vector2() = default;
    Vector2(float x, float y);
    Vector2(const Vector2& vector);
    Vector2(const float* data);
    ~Vector2() = default;

    Vector2& operator = (const Vector2& rhs);
    bool operator == (const Vector2& rhs) const;
    bool operator != (const Vector2& rhs) const;
    Vector2 operator + (const Vector2& rhs) const;
    Vector2& operator += (const Vector2& rhs);
    Vector2 operator - (const Vector2& rhs) const;
    Vector2& operator -= (const Vector2& rhs);
    Vector2 operator -() const;
    Vector2   operator * (float scalar) const;
    Vector2&  operator *= (float scalar);
    Vector2   operator / (float scalar) const;
    Vector2&  operator /= (float scalar);
    Vector2 operator * (const Vector2& rhs);
    Vector2& operator *= (const Vector2& rhs);
    const float* toArray() const { return &x; }
    float* toArray() { return &x; }

    float x;
    float y;

    static const Vector2 ZERO;
    static const Vector2 UNIT_X;
    static const Vector2 UNIT_Y;
    static const Vector2 ONE;
};

}

#endif