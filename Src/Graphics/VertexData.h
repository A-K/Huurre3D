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

#ifndef VertexData_H
#define VertexData_H

#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexStream.h"

namespace Huurre3D
{

class VertexData : public GraphicObject
{
public:
    VertexData(PrimitiveType primitiveType, int numVertices) :
    primitiveType(primitiveType),
    numVertices(numVertices)
    {}
    ~VertexData() = default;
	
    void setIndexBuffer(IndexBuffer* indexBuffer)
    {
        this->indexBuffer = indexBuffer;
        indexed = true;
    }
    void setVertexStream(VertexStream* vertexStream) {vertexStreams.pushBack(vertexStream);}
    bool isIndexed() const {return indexed;};
    PrimitiveType getPrimitiveType() const {return primitiveType;}
    const Vector<VertexStream*>& getVertexStreams() const {return vertexStreams;}
    IndexBuffer* getIndexBuffer() const {return indexBuffer;}
    int getNumVertices() const {return numVertices;}

private:
    PrimitiveType primitiveType;
    Vector<VertexStream*> vertexStreams;
    IndexBuffer* indexBuffer = nullptr;
    bool indexed = false;
    int numVertices;
};

}

#endif