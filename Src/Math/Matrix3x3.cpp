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

#include "Math/Matrix3x3.h"

namespace Huurre3D
{

//Initialize static variables
const Matrix3x3 Matrix3x3::ZERO(Vector3(0,0,0), Vector3(0,0,0), Vector3(0,0,0));
const Matrix3x3 Matrix3x3::IDENTITY(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1));

Matrix3x3::Matrix3x3(const Vector3& col0, const Vector3& col1, const Vector3& col2)
{
    set(col0, col1, col2);
}

Matrix3x3::Matrix3x3(const Matrix3x3& matrix)
{
    cols[0] = matrix.cols[0];
    cols[1] = matrix.cols[1];
    cols[2] = matrix.cols[2];
}

void Matrix3x3::set(const Vector3& col0, const Vector3& col1, const Vector3& col2)
{
    cols[0] = col0;
    cols[1] = col1;
    cols[2] = col2;
}

const Vector3& Matrix3x3::operator[](int index) const
{
    return cols[index];
}

Vector3& Matrix3x3::operator[](int index)
{
    return cols[index];
}

Matrix3x3& Matrix3x3::operator = (const Matrix3x3& rhs)
{
    cols[0] = rhs.cols[0];
    cols[1] = rhs.cols[1];
    cols[2] = rhs.cols[2];
    return *this;
}

bool Matrix3x3::operator == (const Matrix3x3& rhs) const
{
    return cols[0] == rhs.cols[0] && cols[1] == rhs.cols[1] && cols[2] == rhs.cols[2];
}

bool Matrix3x3::operator != (const Matrix3x3& rhs) const
{
    return cols[0] != rhs.cols[0] || cols[1] != rhs.cols[1] || cols[2] != rhs.cols[2];
}

Matrix3x3 Matrix3x3::operator + (const Matrix3x3& rhs) const
{
    return Matrix3x3(cols[0] + rhs.cols[0], cols[1] + rhs.cols[1], cols[2] + rhs.cols[2]);
}

Matrix3x3& Matrix3x3::operator += (const Matrix3x3& rhs)
{
    cols[0] += rhs.cols[0];
    cols[1] += rhs.cols[1];
    cols[2] += rhs.cols[2];
    return *this;
}

Matrix3x3 Matrix3x3::operator - (const Matrix3x3& rhs) const
{
    return Matrix3x3(cols[0] - rhs.cols[0], cols[1] - rhs.cols[1], cols[2] - rhs.cols[2]);
}

Matrix3x3& Matrix3x3::operator -= (const Matrix3x3& rhs)
{
    cols[0] -= rhs.cols[0];
    cols[1] -= rhs.cols[1];
    cols[2] -= rhs.cols[2];
    return *this;
}

Matrix3x3 Matrix3x3::operator -() const
{
    return Matrix3x3(-cols[0], -cols[1], -cols[2]);
}

float Matrix3x3::trace() const
{
    return cols[0].x + cols[1].y + cols[2].z;
}

Matrix3x3 Matrix3x3::operator * (const Matrix3x3& rhs) const
{
    return Matrix3x3(*this * rhs.cols[0], *this * rhs.cols[1], *this * rhs.cols[2]);
}

Matrix3x3& Matrix3x3::operator *= (const Matrix3x3& rhs)
{
    set(*this * rhs.cols[0], *this * rhs.cols[1], *this * rhs.cols[2]);
    return *this;
}

Matrix3x3 Matrix3x3::operator * (float scalar) const
{
    return Matrix3x3(cols[0] * scalar, cols[1] * scalar, cols[2] * scalar);
}

Matrix3x3& Matrix3x3::operator *= (float scalar)
{
    cols[0] *= scalar;
    cols[1] *= scalar;
    cols[2] *= scalar;
    return *this;
}

Vector3 Matrix3x3::operator * (const Vector3& rhs) const
{
    return Vector3(cols[0] * rhs.x + cols[1] * rhs.y + cols[2] * rhs.z);
}

float Matrix3x3::determinant() const
{
    //Calculates determinant using triple product. 
    return cols[0].dot(cols[1].cross(cols[2]));
}

Matrix3x3 Matrix3x3::transpose() const
{
    return Matrix3x3(Vector3(cols[0].x, cols[1].x, cols[2].x),
                     Vector3(cols[0].y, cols[1].y, cols[2].y),
                     Vector3(cols[0].z, cols[1].z, cols[2].z));
}

Matrix3x3 Matrix3x3::inverse() const
{
    Matrix3x3 transpose = this->transpose(); 
    float invDet = 1.0f / this->determinant();

    return Matrix3x3(transpose.cols[1].cross(transpose.cols[2]) * invDet,
                     transpose.cols[2].cross(transpose.cols[0]) * invDet,
                     transpose.cols[0].cross(transpose.cols[1]) * invDet);
}

}