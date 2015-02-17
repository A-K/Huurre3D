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

#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"
#include "Math/MathFunctions.h"

namespace Huurre3D
{

//Initialize static variables
const Matrix4x4 Matrix4x4::ZERO(Vector4(0.0f,0.0f,0.0f,0.0f), Vector4(0.0f,0.0f,0.0f,0.0f), Vector4(0.0f,0.0f,0.0f,0.0f), Vector4(0.0f,0.0f,0.0f,0.0f));
const Matrix4x4 Matrix4x4::IDENTITY(Vector4(1.0f,0.0f,0.0f,0.0f), Vector4(0.0f,1.0f,0.0f,0.0f), Vector4(0.0f,0.0f,1.0f,0.0f), Vector4(0.0f,0.0f,0.0f,1.0f));

Matrix4x4::Matrix4x4(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3)
{
    cols[0] = col0;
    cols[1] = col1;
    cols[2] = col2;
    cols[3] = col3;
}

Matrix4x4::Matrix4x4(const Matrix4x4& matrix)
{
    cols[0] = matrix.cols[0];
    cols[1] = matrix.cols[1];
    cols[2] = matrix.cols[2];
    cols[3] = matrix.cols[3];
}

const Vector4& Matrix4x4::operator[](int index) const
{
    return cols[index];
}

Vector4& Matrix4x4::operator[](int index)
{
    return cols[index];
}

Matrix4x4& Matrix4x4::operator = (const Matrix4x4& rhs)
{
    cols[0] = rhs.cols[0];
    cols[1] = rhs.cols[1];
    cols[2] = rhs.cols[2];
    cols[3] = rhs.cols[3];
    return *this;
}

bool Matrix4x4::operator == (const Matrix4x4& rhs) const
{
    return cols[0] == rhs.cols[0] && cols[1] == rhs.cols[1] && cols[2] == rhs.cols[2] && cols[3] == rhs.cols[3];
}

bool Matrix4x4::operator != (const Matrix4x4& rhs) const
{
    return cols[0] != rhs.cols[0] || cols[1] != rhs.cols[1] || cols[2] != rhs.cols[2] || cols[3] != rhs.cols[3];
}

Matrix4x4 Matrix4x4::operator + (const Matrix4x4& rhs) const
{
    return Matrix4x4(cols[0] + rhs.cols[0], cols[1] + rhs.cols[1], cols[2] + rhs.cols[2], cols[3] + rhs.cols[3]);
}

Matrix4x4& Matrix4x4::operator += (const Matrix4x4& rhs)
{
    cols[0] += rhs.cols[0];
    cols[1] += rhs.cols[1];
    cols[2] += rhs.cols[2];
    cols[3] += rhs.cols[3];
    return *this;
}

Matrix4x4 Matrix4x4::operator - (const Matrix4x4& rhs) const
{
    return Matrix4x4(cols[0] - rhs.cols[0], cols[1] - rhs.cols[1], cols[2] - rhs.cols[2], cols[3] - rhs.cols[3]);
}
	
Matrix4x4& Matrix4x4::operator -= (const Matrix4x4& rhs)
{
    cols[0] -= rhs.cols[0];
    cols[1] -= rhs.cols[1];
    cols[2] -= rhs.cols[2];
    cols[3] -= rhs.cols[3];
    return *this;
}

Matrix4x4 Matrix4x4::operator -() const
{
    return Matrix4x4(-cols[0], -cols[1], -cols[2], -cols[3]);
}

Vector4 Matrix4x4::operator * (const Vector4& rhs) const
{
    return Vector4(cols[0] * rhs.x + cols[1] * rhs.y + cols[2] * rhs.z + cols[3] * rhs.w);
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4& rhs) const
{
    return Matrix4x4(*this * rhs.cols[0], *this * rhs.cols[1], *this * rhs.cols[2], *this * rhs.cols[3]);
}

Matrix4x4 Matrix4x4::operator * (float scalar) const
{
    return Matrix4x4(cols[0] * scalar, cols[1] * scalar, cols[2] * scalar, cols[3] * scalar);
}

Matrix4x4& Matrix4x4::operator *= (float scalar)
{
    cols[0] *= scalar;
    cols[1] *= scalar;
    cols[2] *= scalar;
    cols[3] *= scalar;
    return *this;
}

float Matrix4x4::determinant() const
{
    //calculate determinant using triple product 
    return cols[0].dot(cols[1].cross(cols[2], cols[3]));
}

Matrix4x4 Matrix4x4::transpose() const
{
    return Matrix4x4(Vector4(cols[0].x, cols[1].x, cols[2].x, cols[3].x),
                     Vector4(cols[0].y, cols[1].y, cols[2].y, cols[3].y),
                     Vector4(cols[0].z, cols[1].z, cols[2].z, cols[3].z),
                     Vector4(cols[0].w, cols[1].w, cols[2].w, cols[3].w));
}

Matrix4x4 Matrix4x4::inverse() const
{
    Matrix4x4 transpose = this->transpose(); 
    float invDet = 1.0f / this->determinant();

    return Matrix4x4(transpose.cols[1].cross(transpose.cols[2], transpose.cols[3]) * invDet,
                     transpose.cols[3].cross(transpose.cols[2], transpose.cols[0]) * invDet,
                     transpose.cols[0].cross(transpose.cols[1], transpose.cols[3]) * invDet,
                     transpose.cols[2].cross(transpose.cols[1], transpose.cols[0]) * invDet);
}

void Matrix4x4::setTransform(const Vector3& position, const Quaternion &rotation, const Vector3& scale)
{
    Matrix3x3 rot3x3 = rotation.rotationMatrix3x3();
    cols[0] = Vector4(rot3x3[0] * scale.x,  0.0f);
    cols[1] = Vector4(rot3x3[1] * scale.y,  0.0f);
    cols[2] = Vector4(rot3x3[2] * scale.z,  0.0f);
    cols[3] = Vector4(position.x, position.y, position.z, 1.0f);
}

void Matrix4x4::setInverseTransform(const Vector3& position, const Quaternion &rotation, const Vector3& scale)
{
    //Invert the parameters.
    Vector3 invTranslate = -position;
    Vector3 invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
    Quaternion invRot = rotation.inverse();

    //Switch the order of operations.
    invTranslate = invRot.rotate(invTranslate);
    invTranslate *= invScale;

    Matrix3x3 rot3x3;
    rot3x3 = invRot.rotationMatrix3x3();

    cols[0] = Vector4(rot3x3[0] * invScale,  0.0f);
    cols[1] = Vector4(rot3x3[1] * invScale,  0.0f);
    cols[2] = Vector4(rot3x3[2] * invScale,  0.0f);
    cols[3] = Vector4(invTranslate.x, invTranslate.y, invTranslate.z, 1.0f);
}

void Matrix4x4::setPerspective(float fov, float aspect, float near, float far)
{
    float top = near * tan(fov * 0.5f * DEGTORAD);
    float bottom = -top;
    float left = bottom * aspect;
    float right = top * aspect;

    cols[0].set(2.0f * near / (right - left), 0.0f, 0.0f, 0.0f);
    cols[1].set(0.0f, 2.0f * near / (top - bottom), 0.0f, 0.0f);
    cols[2].set((right + left) / (right - left), (top + bottom) / (top - bottom), -(far + near) / (far - near), -1.0f);
    cols[3].set(0.0f, 0.0f, -2.0f * far * near / (far - near), 0.0f);
}

void Matrix4x4::setOrtho(float left, float right, float bottom, float top, float near, float far)
{
    cols[0].set(2.0f / (right - left), 0.0f, 0.0f, 0.0f);
    cols[1].set(0.0f, 2.0f / (top - bottom), 0.0f, 0.0f);
    cols[2].set(0.0f, 0.0f, -2.0f / (far - near), 0.0f);
    cols[3].set(-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.0f);
}

void Matrix4x4::setLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{		
    Vector3 targetZ = (eye - target).normalized();
    Vector3 targetX = up.cross(targetZ).normalized();
    Vector3 targetY = targetZ.cross(targetX).normalized();

    cols[0].set(targetX.x, targetY.x, targetZ.x, 0.0f);
    cols[1].set(targetX.y, targetY.y, targetZ.y, 0.0f);
    cols[2].set(targetX.z, targetY.z, targetZ.z, 0.0f);
    cols[3].set(-targetX.dot(eye), -targetY.dot(eye), -targetZ.dot(eye), 1.0f);
}

}