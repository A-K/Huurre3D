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

#ifndef GraphicDataContainer_H
#define GraphicDataContainer_H

#include "Util/MemoryBuffer.h"

namespace Huurre3D
{

class GraphicDataContainer : public MemoryBuffer
{
public:
    GraphicDataContainer(bool discardDataWhenGPU):
    discardDataWhenGPU(discardDataWhenGPU),
    sizeInBytesGPU(0)
    {
    }
    virtual ~GraphicDataContainer() = default;

    bool discardDataWhenCommittedToGPU() const {return discardDataWhenGPU;}
    unsigned int getCurrentDataSizeInGPU() const {return sizeInBytesGPU;}
    unsigned int getCurrentDataSizeInCPU() const {return sizeInBytes;}
    void setDataSizeInGPU(unsigned int size) {unsigned int sizeInBytesGPU = size;}
    void setDataSizeInGPUIsDataSizeInCPU() {sizeInBytesGPU = sizeInBytes;}
    void discardData() 
    {
        sizeInBytesGPU = sizeInBytes; 
        resetBuffer();
    }

protected:
    bool discardDataWhenGPU;
    unsigned int sizeInBytesGPU;
};

}

#endif