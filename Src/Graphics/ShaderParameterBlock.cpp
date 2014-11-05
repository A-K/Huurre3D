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

#include "Graphics/ShaderParameterBlock.h"
#include "Math/MathFunctions.h"

namespace Huurre3D
{

ShaderParameterBlock::ShaderParameterBlock(GraphicSystem* graphicSystem, const std::string& name):
name(name),
GraphicObject(graphicSystem),
GraphicDataContainer(false)
{
    nameHash = generateHash((unsigned char*)name.c_str(), name.size());
}

void ShaderParameterBlock::setBindingIndex(unsigned int bindingIndex)
{
    this->bindingIndex = bindingIndex;
    binded = true;
}

void ShaderParameterBlock::addParameter(float parameter) 
{
    appendParameter(&parameter, 4);
}

void ShaderParameterBlock::addParameter(const Vector4& parameter) 
{
    appendParameter(parameter.toArray(), 16);
}

void ShaderParameterBlock::addParameter(const Matrix4x4& parameter) 
{
    appendParameter(parameter.toArray(), 64);
}

void ShaderParameterBlock::appendParameter(const float* parameter, unsigned int size)
{
    append(parameter, size);
    dirty = true;
}

}