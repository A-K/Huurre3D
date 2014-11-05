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

#ifndef Shader_H
#define Shader_H

#include "Graphics/GraphicObject.h"
#include "Util/Vector.h"
#include <string>
#include <sstream>

namespace Huurre3D
{

struct ShaderDefine
{
    ShaderDefineType defineType;
    std::string value;

    ShaderDefine() = default;

    ShaderDefine(ShaderDefineType define, std::string value):
    defineType(define),
    value(value)
    {
    }
};

class Shader : public GraphicObject
{
public:
    Shader(GraphicSystem* graphicSystem, ShaderType shaderType, const std::string& sourceFileName);
    ~Shader() = default;

    void setCompiled(bool compiled) {this->compiled = compiled;}
    template<typename T> void setDefine(ShaderDefineType define, const T& value)
    {
        std::ostringstream oss; 
        oss << value; 
        shaderDefines.pushBack(ShaderDefine(define, oss.str()));
        dirty = true;
    }
    void setDefine(ShaderDefineType shaderDefineType);
    void setDefine(ShaderDefine shaderDefine);
    void setDefines(const Vector<ShaderDefine>& shaderDefines);
    void setSource(const std::string& source);
    ShaderType getShaderType() const {return shaderType;}
    bool isCompiled() const {return compiled;}
    std::string& getSource() {return source;}
    const std::string& getSourceFileName() const {return sourceFileName;}
    const Vector<ShaderDefine>& getDefines() const {return shaderDefines;}

private:
    ShaderType shaderType;
    std::string sourceFileName;
    std::string source;
    bool compiled = false;
    Vector<ShaderDefine> shaderDefines;
};

}

#endif