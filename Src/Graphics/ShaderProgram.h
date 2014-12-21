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

#ifndef ShaderProgram_H
#define ShaderProgram_H

#include "Graphics/Shader.h"
//#include "Graphics/ShaderParameter.h"
#include "Util/Vector.h"
#include "Math/MathFunctions.h"

namespace Huurre3D
{

struct ShaderParameterDescription
{
    ShaderParameterType type;
    unsigned int nameHash;
    std::string name;
    int arraySize; //If this parameter is an array, the size is described here.
    int shaderIndex;

    void setName(const std::string paramName)
    {
        name = paramName;
        nameHash = generateHash((unsigned char*)paramName.c_str(), paramName.size());
    }
};

struct ShaderParameterBlockDescription
{
    std::string name;
    unsigned int nameHash;
    int size;
    unsigned int shaderIndex;
    unsigned int bindingPoint;
    bool sourceBlockSetted;
    ShaderParameterBlockDescription():
    sourceBlockSetted(false)
    {}

    void setName(const std::string paramName)
    {
        name = paramName;
        nameHash = generateHash((unsigned char*)paramName.c_str(), paramName.size());
    }
};

class ShaderProgram : public GraphicObject
{
public:
    ShaderProgram(GraphicSystem* graphicSystem, Shader* vertexShader, Shader* fragmentShader);
    ~ShaderProgram() = default;

    void setLinked(bool linked) {this->linked = linked;}
    void setShaderParameterDescription(ShaderParameterDescription& parameter);
    void setShaderParameterBlockDescription(ShaderParameterBlockDescription& parameterBlock);
    void setShaderCombinationTag(int shaderCombinationTag);
    ShaderParameterDescription* getShaderParameterDescription(const std::string& name);
    ShaderParameterBlockDescription* getShaderParameterBlockDescription(const std::string& name);
    ShaderParameterDescription* getShaderParameterDescription(const unsigned int nameHash);
    ShaderParameterBlockDescription* getShaderParameterBlockDescription(unsigned int nameHash);
    Shader* getVertexShader() const {return vertexShader;}
    Shader* getFragmentShader() const {return fragmentShader;}
    bool isLinked() const {return linked;}
    int getShaderCombinationTag() const {return shaderCombinationTag;}

private:
    Shader* vertexShader;
    Shader* fragmentShader;
    bool linked = false;
    int shaderCombinationTag = 0;
    Vector<ShaderParameterDescription> parameterDescriptions;
    Vector<ShaderParameterBlockDescription> parameterBlockDescriptions;
};

}

#endif