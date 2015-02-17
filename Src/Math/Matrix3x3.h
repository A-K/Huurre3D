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

#ifndef Matrix3x3_H
#define Matrix3x3_H

#include "Math/Vector3.h"

namespace Huurre3D
{

class Matrix3x3
{
public:
    Matrix3x3() = default;
    Matrix3x3(const Vector3& col0, const Vector3& col1, const Vector3& col2);
    Matrix3x3(const Matrix3x3& matrix);
    ~Matrix3x3() = default;

    void set(const Vector3& col0, const Vector3& col1, const Vector3& col2);
    const Vector3& operator [] (int index) const;
    Vector3& operator [] (int index);
    Matrix3x3& operator = (const Matrix3x3& rhs);
    bool operator == (const Matrix3x3& rhs) const;
    bool operator != (const Matrix3x3& rhs) const;
    Matrix3x3 operator + (const Matrix3x3& rhs) const;
    Matrix3x3& operator += (const Matrix3x3& rhs);
    Matrix3x3 operator - (const Matrix3x3& rhs) const;
    Matrix3x3& operator -= (const Matrix3x3& rhs);
    Matrix3x3 operator -() const;
    float trace() const;
    //Matrix multiplication, uses post-multiplication.
    Matrix3x3 operator * (const Matrix3x3& rhs) const;
    //Matrix multiplication assignement, uses post-multiplication.
    Matrix3x3& operator *= (const Matrix3x3& rhs);
    Matrix3x3 operator * (float scalar) const;
    Matrix3x3& operator *= (float scalar);
    //Vector multiplication, uses post-multiplication.
    Vector3 operator * (const Vector3& rhs) const;
    float determinant() const;
    Matrix3x3 transpose() const;
    //Inverse, inverted using Cramer's rule. M_inv = (1 / det(M)) * adj(M).
    Matrix3x3 inverse() const;
    const float* toArray() const { return cols[0].toArray(); }
    float* toArray() { return cols[0].toArray(); }

    static const Matrix3x3 ZERO;
    static const Matrix3x3 IDENTITY;

private:
    //Matrix elemets presented as column vectors, uses column-major order notation:
    //0 3 6
    //1 4 7
    //2 5 8
    Vector3 cols[3];
};

}

#endif