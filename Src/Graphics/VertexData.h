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

#ifndef VertexData_H
#define VertexData_H

#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexStream.h"

namespace Huurre3D
{

class VertexData : public GraphicObject
{
public:
    VertexData(GraphicSystem* graphicSystem, PrimitiveType primitiveType, int numVertices);
    ~VertexData();
	
    void setIndexBuffer(IndexBuffer* indexBuffer);
    void setAttributeBuffer(AttributeBuffer* attributeBuffer);
    AttributeBuffer* getAtrributeBufferBySemantic(const AttributeSemantic semantic);
    bool isIndexed() const {return indexed;};
    PrimitiveType getPrimitiveType() const {return primitiveType;}
    VertexStream* getDynamicVertexStream() const {return dynamicVertexStream;}
    VertexStream* getStaticVertexStream() const {return staticVertexStream;}
    IndexBuffer* getIndexBuffer() const {return indexBuffer;}
    int getNumVertices() const {return numVertices;}

    template<typename T> void setIndexBuffer(IndexType indexType, int numIndices, const T* indices, bool dynamic = false)
    {
        indexBuffer = graphicSystem->createIndexBuffer(indexType, numIndices, dynamic);
        indexBuffer->setIndices(indices);
        indexed = true;
    }
    //If the attribute buffer is not dynamic it will be interleaved with other non dynamic buffers into a one buffer in gpu.
    //All dynamic attribute buffers will have a own buffer in gpu.
    template<typename T> void setAttributeBuffer(AttributeType type, AttributeSemantic semantic, int numComponentsPerVertex, int bufferLength, const T* attributeData, bool normalized = false, bool dynamic = true)
    {
        AttributeBuffer* attributeBuffer = graphicSystem->createAttributeBuffer(type, semantic, numComponentsPerVertex, normalized, dynamic);
        int bufferSize = bufferLength * attributeSize[static_cast<int>(type)];
        attributeBuffer->setAttributes(attributeData, bufferSize);
        setAttributeBuffer(attributeBuffer);
    }

private:
    PrimitiveType primitiveType;
    VertexStream* dynamicVertexStream = nullptr;
    VertexStream* staticVertexStream = nullptr;
    IndexBuffer* indexBuffer = nullptr;
    bool indexed = false;
    int numVertices;
};

}

#endif