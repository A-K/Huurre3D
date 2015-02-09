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

#include "Graphics/ShaderParameterBlock.h"
#include "Math/MathFunctions.h"

namespace Huurre3D
{

ShaderParameterBlock::ShaderParameterBlock(const std::string& name):
name(name)
{
    nameHash = generateHash((unsigned char*)name.c_str(), name.size());
}

void ShaderParameterBlock::setBindingIndex(unsigned int bindingIndex)
{
    this->bindingIndex = bindingIndex;
    binded = true;
}

void ShaderParameterBlock::addParameter(int parameter)
{
    appendParameterBlock(&parameter, sizeof(int));
}

void ShaderParameterBlock::addParameter(const FixedArray<int, 2>& parameter)
{
    appendParameterBlock(parameter.data(), 2 * sizeof(int));
}

void ShaderParameterBlock::addParameter(const FixedArray<int, 3>& parameter)
{
    appendParameterBlock(parameter.data(), 3 * sizeof(int));
}

void ShaderParameterBlock::addParameter(const FixedArray<int, 4>& parameter)
{
    appendParameterBlock(parameter.data(), 4 * sizeof(int));
}

void ShaderParameterBlock::addParameter(float parameter) 
{
    appendParameterBlock(&parameter, sizeof(float));
}

void ShaderParameterBlock::addParameter(const Vector2& parameter)
{
    appendParameterBlock(parameter.toArray(), 2 * sizeof(float));
}

void ShaderParameterBlock::addParameter(const Vector3& parameter)
{
    appendParameterBlock(parameter.toArray(), 3 * sizeof(float));
}

void ShaderParameterBlock::addParameter(const Vector4& parameter) 
{
    appendParameterBlock(parameter.toArray(), 4 * sizeof(float));
}

void ShaderParameterBlock::addParameter(const Matrix4x4& parameter) 
{
    appendParameterBlock(parameter.toArray(), 16 * sizeof(float));
}

void ShaderParameterBlock::appendParameterBlock(const float* parameter, unsigned int size)
{
    graphicData.append(parameter, size);
    dirty = true;
}

void ShaderParameterBlock::appendParameterBlock(const int* parameter, unsigned int size)
{
    graphicData.append(parameter, size);
    dirty = true;
}

}