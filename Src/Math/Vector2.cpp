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

#include "Math/Vector2.h"
#include "Math/MathFunctions.h"

namespace Huurre3D
{

//Initialize static variables
const Vector2 Vector2::ZERO(0.0f, 0.0f);
const Vector2 Vector2::UNIT_X(1.0f, 0.0f);
const Vector2 Vector2::UNIT_Y(0.0f, 1.0f);
const Vector2 Vector2::ONE(1.0f, 1.0f);

Vector2::Vector2(float x, float y):
x(x), y(y)
{
}

Vector2::Vector2(const Vector2& vector):
x(vector.x), y(vector.y) 
{
}

Vector2::Vector2(const float* data):
x(data[0]), y(data[1])
{

}

Vector2& Vector2::operator = (const Vector2& rhs)
{
    x = rhs.x;
    y = rhs.y;
    return *this;
}

bool Vector2::operator == (const Vector2& rhs) const
{
    return equals(x, rhs.x) && equals(y, rhs.y);
}

bool Vector2::operator != (const Vector2& rhs) const
{
    return !equals(x, rhs.x) || !equals(y, rhs.y);
}

Vector2 Vector2::operator + (const Vector2& rhs) const
{
    return Vector2(x + rhs.x, y + rhs.y);
}

Vector2& Vector2::operator += (const Vector2& rhs)
{
	 x += rhs.x;
     y += rhs.y;
     return *this;
}

Vector2 Vector2::operator - (const Vector2& rhs) const
{
    return Vector2(x - rhs.x, y - rhs.y);
}

Vector2& Vector2::operator -= (const Vector2& rhs)
{
	 x -= rhs.x;
     y -= rhs.y;
     return *this;
}

Vector2 Vector2::operator -() const
{
    return Vector2(-x, -y);
}

Vector2 Vector2::operator * (float scalar) const
{
    return Vector2(x * scalar, y * scalar);
}

Vector2& Vector2::operator *= (float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2 Vector2::operator / (float scalar) const
{
    float invScalar = 1.0f / scalar;
    return Vector2(x * invScalar, y * invScalar);
}

Vector2& Vector2::operator /= (float scalar)
{
    float invScalar = 1.0f / scalar;

    x *= invScalar;
    y *= invScalar;
    return *this;
}

Vector2 Vector2::operator * (const Vector2& rhs)
{
    return Vector2(x * rhs.x, y * rhs.y);
}

Vector2& Vector2::operator *= (const Vector2& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}

}