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

#ifndef VertexStream_H
#define VertexStream_H

#include "Graphics/AttributeBuffer.h"
#include "Util/Vector.h"

namespace Huurre3D
{

class VertexStream : public GraphicObject
{
public:
    VertexStream(int numVertices);
    ~VertexStream() = default;
	
    void setAttributeBuffer(AttributeBuffer* attributeBuffer);
    const Vector<AttributeBuffer*>& getAttributeBuffers() const {return attributes;}
    unsigned int getNumVertices() const {return numVertices;}
    unsigned int getVertexSize() const {return vertexSize;}
    unsigned int getNumAttributeBuffers() const {return numBuffers;}
    const unsigned char* getInterleavedAttributeData();

private:
    unsigned int numVertices;
    unsigned int vertexSize = 0;
    unsigned int numBuffers = 0;
    Vector<AttributeBuffer*> attributes;
};

}

#endif