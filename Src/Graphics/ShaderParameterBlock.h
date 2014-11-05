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

#ifndef ShaderParameterBlock_H
#define ShaderParameterBlock_H

#include "Graphics/GraphicObject.h"
#include "Graphics/GraphicDataContainer.h"
#include "Math/Matrix4x4.h"

namespace Huurre3D
{

class ShaderParameterBlock : public GraphicObject, public GraphicDataContainer
{
public:
    ShaderParameterBlock(GraphicSystem* graphicSystem, const std::string& name);
    ~ShaderParameterBlock() = default;
	
    void setBindingIndex(unsigned int bindingIndex);
    void addParameter(float parameter);
    void addParameter(const Vector4& parameter);
    void addParameter(const Matrix4x4& parameter);
    const std::string& getName() const {return name;}
    const unsigned int getNameHash() const {return nameHash;}
    int getBindingIndex() const {return bindingIndex;}
    bool hasBindingIndex() const {return binded;}

    template<typename T> void setParameterData(T* parameterData, int dataSize)
    {
        if(!parameterData || !dataSize || dataSize == 0)
            return;

        bufferData(parameterData, dataSize);
        dirty = true;
    }

private:
    void appendParameter(const float* parameter, unsigned int size);
    std::string name;
    unsigned int nameHash;
    unsigned int bindingIndex = 0;
    bool binded = false;
};

}

#endif