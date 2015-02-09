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

#ifndef IndexBuffer_H
#define IndexBuffer_H

#include "Graphics/GraphicObject.h"

namespace Huurre3D
{

class IndexBuffer : public GraphicObject
{
public:
    IndexBuffer(IndexType indexType, int numIndices, bool dynamic):
    indexType(indexType),
    numIndices(numIndices),
    dynamic(dynamic)
    {}
    ~IndexBuffer() = default;

    IndexType getIndexType() const {return indexType;}
    int getNumIndices() const {return numIndices;}
    bool isDynamic() const {return dynamic;}
    template<typename T> void setIndices(const T* indices)
    {
        int bufferSize = numIndices * indexSize[static_cast<int>(indexType)];
        graphicData.bufferData(indices, bufferSize);

        if(graphicData.isNull())
            std::cout <<"Could not set indices" <<std::endl;

        dirty = true;
    }

private:
    IndexType indexType;
    int numIndices;
    bool dynamic;
};

}

#endif