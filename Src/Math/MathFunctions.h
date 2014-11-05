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

#ifndef MathFunctions_H
#define MathFunctions_H

#include "Math/Vector2.h"
#include "Math/Vector3.h"

#include <math.h>
#include <random>
#include <limits>

namespace Huurre3D
{

static const float PI = 3.1415926535897932384626433832795f;
static const float EPSILON = 1.0e-6f;
static const float INF = (float)std::numeric_limits<float>::infinity();
static const float DEGTORAD = PI / 180.0f;
static const float RADTODEG = 1.0f / DEGTORAD;

enum class Intersection 
{
    Outside,
    Inside,
    Intersects
};

inline bool isNaN(float value) 
{
    return value != value; 
}

inline bool isZero(float value)
{
    return abs(value) < EPSILON;
}

inline bool equals(float x, float y)
{
    return isZero(x - y);
}
	
inline float min(float x, float y)
{
    return x < y ? x : y;
}

inline float max(float x, float y)
{
    return x > y ? x : y;
}

inline float clamp (float value, float floor, float ceil) 
{
    return value <= ceil ? (value >= floor ? value : floor) : ceil;
}

inline int clampInt (int value, int floor, int ceil) 
{
    return value <= ceil ? (value >= floor ? value : floor) : ceil;
}

inline bool isPow2 (int value) 
{
    return value != 0 && (value & value - 1) == 0;
}
	
inline Vector2 sinCos(float angleRadians)
{
    return Vector2(sin(angleRadians), cos(angleRadians));
}

//Returns the numerical combination of the lowest value of x, y and z from both vectors.
inline Vector3 min(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3(min(lhs.x, rhs.x), min(lhs.y, rhs.y), min(lhs.z, rhs.z));
}

//Returns the numerical combination of the highest value of x, y and z from both vectors.
inline Vector3 max(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3(max(lhs.x, rhs.x), max(lhs.y, rhs.y), max(lhs.z, rhs.z));
}

inline Vector3 clamp(const Vector3& value, const Vector3& floor, const Vector3& ceil)
{
    return max(floor, min(value, ceil));
}

inline Vector3 floor(const Vector3& value)
{
    return Vector3(std::floor(value.x), std::floor(value.y), std::floor(value.z));
}

inline unsigned int generateHash(const unsigned char* data, unsigned int size) 
{
    unsigned int hash = 0;
    for(unsigned int i = 0; i < size; ++i) 
        hash = 65599 * hash + data[i];
   
    return hash ^ (hash >> 16);
}

inline float randomFloat(float a, float b)
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<float> distribution(a, b);

    return distribution(engine);
}

}

#endif