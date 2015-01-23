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

#ifndef AttributeBuffer_H
#define AttributeBuffer_H

#include "Graphics/GraphicObject.h"
#include "Graphics/GraphicDataContainer.h"
#include <iostream>

namespace Huurre3D
{

class AttributeBuffer : public GraphicObject, public GraphicDataContainer
{
public:
    AttributeBuffer(AttributeType type, AttributeSemantic semantic, int numComponentsPerVertex, bool normalized, bool dynamic):
    type(type),
    semantic(semantic),
    stride(0),
    numComponents(numComponentsPerVertex),
    normalized(normalized),
    dynamic(dynamic),
    GraphicDataContainer(true)
    {
        stride = numComponentsPerVertex * attributeSize[static_cast<int>(type)];
    }
    ~AttributeBuffer() = default;
	
    AttributeType getAttributeType() const {return type;}
    AttributeSemantic getAttributeSemantic() const {return semantic;}
    int getStride() const {return stride;}
    int getNumComponents() const {return numComponents;}
    bool isNormalized() const {return normalized;}
    bool isDynamic() const {return dynamic;}

    template<typename T> void setAttributes(const T* attributeData, int dataSize)
    {
        bufferData(attributeData, dataSize);

        if(!this->data)
            std::cout <<"Could not set attributes" <<std::endl;

        dirty = true;
    }

private:
    AttributeType type;
    AttributeSemantic semantic;
    int stride;
    int numComponents;
    bool normalized;
    bool dynamic;
};

}

#endif

