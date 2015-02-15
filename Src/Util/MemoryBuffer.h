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

#ifndef MemoryBuffer_H
#define MemoryBuffer_H

#include <string>

namespace Huurre3D
{

class MemoryBuffer
{
public:
    MemoryBuffer() = default;
    MemoryBuffer(MemoryBuffer&& buffer) {*this = std::move(buffer);}
    ~MemoryBuffer() {resetBuffer();}
    MemoryBuffer& operator = (const MemoryBuffer& rhs) 
    {
        bufferData(rhs.getData(), rhs.getSizeInBytes());
        return *this;
    }
    MemoryBuffer& operator = (MemoryBuffer&& rhs)
    {
        this->resetBuffer();
        data = rhs.data;
        capacity = rhs.capacity;
        sizeInBytes = rhs.sizeInBytes;
        rhs.capacity = 0;
        rhs.sizeInBytes = 0;
        rhs.data = nullptr;
        return *this;
    }

    unsigned char* getData() const {return data;}
    unsigned int getSizeInBytes() const {return sizeInBytes;}
    unsigned int getCapacity() const {return capacity;}
    bool isNull() const {return data == nullptr;}
    void clearBuffer() {sizeInBytes = 0;}
    void resetBuffer()
    {
        sizeInBytes = 0;
        capacity = 0;
        delete[] data;
        data = nullptr;
    }

    template<typename T> void bufferData(const T* data, unsigned int dataSize)
    {
        resize(dataSize);
        copyData(this->data, data, dataSize);
    }

    template<typename T> void append(const T* data, unsigned int dataSize)
    {
        (sizeInBytes + dataSize) < capacity ? sizeInBytes += dataSize : resize(sizeInBytes + dataSize);
        //Copy the new data into the end of this buffer. 
        copyData(&this->data[sizeInBytes - dataSize], data, dataSize);
    }

    void resize(unsigned int newSize)
    {
        if(newSize > capacity)
        {
            if(capacity)
            {
                while(capacity < newSize)
                    capacity += capacity;
            }
            else
                capacity = newSize;

            unsigned char* newData = allocate(capacity);
            //Move the data into the new buffer and delete the old
            if(data)
            {
                copyData(newData, data, sizeInBytes);
                delete[] data;
            }
            data = newData;
        }
        sizeInBytes = newSize;
    }

    void reserve(unsigned int newCapacity)
    {
        if(newCapacity > capacity)
        {
            unsigned char* newData = nullptr;
            capacity = newCapacity;

            if(capacity)
            {
                newData = allocate(capacity);
                //Move the data into the new buffer
                copyData(newData, data, sizeInBytes);
            }

            // Delete the old buffer
            delete[] data;
            data = newData;
        }
    }

private:
    unsigned char* allocate(unsigned int size) const {return new unsigned char[size];}
    template<typename T> void copyData(unsigned char* destination, const T* source, unsigned int size) {memcpy(destination, source, size);}

    unsigned char* data = nullptr;
    unsigned int sizeInBytes = 0;
    unsigned int capacity = 0;
};

}

#endif