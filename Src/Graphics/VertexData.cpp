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

#include "Graphics/VertexData.h"
#include "Graphics/GraphicSystem.h"

namespace Huurre3D
{

VertexData::VertexData(GraphicSystem* graphicSystem, PrimitiveType primitiveType, int numVertices):
primitiveType(primitiveType),
numVertices(numVertices),
GraphicObject(graphicSystem)
{
    dynamicVertexStream = graphicSystem->createVertexStream(numVertices, false);
    staticVertexStream = graphicSystem->createVertexStream(numVertices, true);
}

VertexData::~VertexData()
{
    graphicSystem->removeVertexStream(dynamicVertexStream);
    graphicSystem->removeVertexStream(staticVertexStream);
}

void VertexData::setIndexBuffer(IndexBuffer* indexBuffer)
{
    this->indexBuffer = indexBuffer;
    indexed = true;
}

void VertexData::setAttributeBuffer(AttributeBuffer* attributeBuffer)
{
    attributeBuffer->isDynamic() ? dynamicVertexStream->setAttributeBuffer(attributeBuffer) : staticVertexStream->setAttributeBuffer(attributeBuffer);
}

AttributeBuffer* VertexData::getAtrributeBufferBySemantic(const AttributeSemantic semantic)
{
    AttributeBuffer* result = nullptr;
    auto searchFunc = [semantic](const AttributeBuffer* buffer) {return buffer->getAttributeSemantic() == semantic;};
    dynamicVertexStream->getAttributeBuffers().findItem(searchFunc, result);

    if(!result)
        staticVertexStream->getAttributeBuffers().findItem(searchFunc, result);

    return result;
}

}