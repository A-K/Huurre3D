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

#include "Graphics/ShaderProgram.h"

namespace Huurre3D
{

ShaderProgram::ShaderProgram(Shader* vertexShader, Shader* fragmentShader):
vertexShader(vertexShader),
fragmentShader(fragmentShader)
{
}

void ShaderProgram::setShaderParameterDescription(ShaderParameterDescription& parameter)
{
    parameterDescriptions.pushBack(parameter);
}

void ShaderProgram::setShaderParameterBlockDescription(ShaderParameterBlockDescription& parameterBlock)
{
    parameterBlockDescriptions.pushBack(parameterBlock);
}

void ShaderProgram::setShaderCombinationTag(int shaderCombinationTag)
{
    this->shaderCombinationTag = shaderCombinationTag;
}

ShaderParameterDescription* ShaderProgram::getShaderParameterDescription(const std::string& name)
{
    int index = parameterDescriptions.getIndexToItem([name](const ShaderParameterDescription description){return description.name.compare(name) == 0;});
    return index != -1 ? &parameterDescriptions[index] : nullptr;
}

ShaderParameterBlockDescription* ShaderProgram::getShaderParameterBlockDescription(const std::string& name)
{
    int index = parameterBlockDescriptions.getIndexToItem([name](const ShaderParameterBlockDescription description){return description.name.compare(name) == 0;});
    return index != -1 ? &parameterBlockDescriptions[index] : nullptr;
}

ShaderParameterDescription* ShaderProgram::getShaderParameterDescription(const unsigned int nameHash)
{
    int index = parameterDescriptions.getIndexToItem([nameHash](const ShaderParameterDescription description){return description.nameHash == nameHash;});
    return index != -1 ? &parameterDescriptions[index] : nullptr;
}

ShaderParameterBlockDescription* ShaderProgram::getShaderParameterBlockDescription(unsigned int nameHash)
{
    int index = parameterBlockDescriptions.getIndexToItem([nameHash](const ShaderParameterBlockDescription description){return description.nameHash == nameHash;});
    return index != -1 ? &parameterBlockDescriptions[index] : nullptr;
}

}