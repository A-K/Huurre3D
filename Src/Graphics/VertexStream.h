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

#ifndef VertexStream_H
#define VertexStream_H

#include "Graphics/AttributeBuffer.h"
#include "Util/Vector.h"

namespace Huurre3D
{

class VertexStream : public GraphicObject
{
public:
    VertexStream(GraphicSystem* graphicSystem, int numVertices);
    ~VertexStream() = default;
	
    void setAttributeBuffer(AttributeBuffer* attributeBuffer);
    const Vector<AttributeBuffer*>& getAttributeBuffers() const {return attributes;}
    unsigned int getNumVertices() const {return numVertices;}
    unsigned int getVertexSize() const {return vertexSize;}
    unsigned int getNumAttributeBuffers() const {return numBuffers;}

private:
    unsigned int numVertices;
    unsigned int vertexSize = 0;
    unsigned int numBuffers = 0;
    Vector<AttributeBuffer*> attributes;
};

class VertexStreamInterleaver : public GraphicDataContainer
{
public:
    VertexStreamInterleaver():
    GraphicDataContainer(true)
    {}
    ~VertexStreamInterleaver() {}
    
    unsigned char* interleaveAttributeData(VertexStream* vertexStream)
    {
        auto attributes = vertexStream->getAttributeBuffers();
        unsigned int vertexSize = vertexStream->getVertexSize();
        unsigned int numVertices = vertexStream->getNumVertices();
        reserve(vertexSize * numVertices);

        unsigned char* chunk;
        unsigned int numBuffers = attributes.size();
        unsigned int* strides = new unsigned int[numBuffers];
        unsigned char** buffers = new unsigned char*[numBuffers];

        for(unsigned int i = 0; i < numBuffers; ++i)
        {
            strides[i] = attributes[i]->getStride();
            buffers[i] = attributes[i]->getData();
        }

        //Interleave the attribute buffers.
        for(unsigned int i = 0; i < numVertices; ++i)
        {
            for(unsigned int j = 0; j < numBuffers; ++j)
            {
                chunk = &buffers[j][i * strides[j]];
                append(chunk, strides[j]);
            }
        }

        delete[] strides;
        delete[] buffers;
        return data;
    }
};

}

#endif