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

#ifndef ShaderParameters_H
#define ShaderParameters_H

#include "Math/Matrix4x4.h"
#include "Math/MathFunctions.h"
#include <string>

namespace Huurre3D
{

struct ShaderParameter
{
    std::string name;
    unsigned int nameHash;
    unsigned char value[64];
    ShaderParameter() = default;

    ShaderParameter(const std::string& name, const Matrix4x4& parameter):
    name(name)
    {
        nameHash = generateHash((unsigned char*)name.c_str(), name.size());
        memcpy(value, parameter.toArray(), 64);
    }

    ShaderParameter(const std::string& name, const Vector4& parameter):
    name(name)
    {
        nameHash = generateHash((unsigned char*)name.c_str(), name.size());
        memcpy(value, parameter.toArray(), 16);
    }

    ShaderParameter(const std::string& name, int parameter):
    name(name)
    {
        nameHash = generateHash((unsigned char*)name.c_str(), name.size());
        memcpy(value, &parameter, 4);
    }
};

}

#endif