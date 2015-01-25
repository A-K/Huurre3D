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

#ifndef FixedArray_H
#define FixedArray_H

#include <assert.h>
#include <initializer_list>

namespace Huurre3D
{

template<class T, unsigned int Count> class FixedArray
{
public:
    FixedArray() 
    {
        static_assert(Count > 0, "Size must be greater than zero");
    }

    FixedArray(const T* data) 
    {
        static_assert(Count > 0, "Size must be greater than zero");

        for(int i = 0; i < Count; ++i)
            arrayData[i] = data[i];
    }

    FixedArray(std::initializer_list<T> list)
    {
        static_assert(Count > 0, "Size must be greater than zero");

        const T* iter = list.begin();
        for(int i = 0; i < Count; ++i)
            arrayData[i] = *(iter + i);
    }

    ~FixedArray() = default;
    unsigned int size() const {return Count;}
    T* data() {return &arrayData[0];}
    const T* data() const {return &arrayData[0];}
    T& operator[](unsigned int i) {return arrayData[i];}
    const T& operator[](unsigned int i) const {return arrayData[i];}
    void fill(const T& value) 
    {
        //memset(arrayData, value, sizeof(arrayData));
        for(int i = 0; i < Count; ++i)
            arrayData[i] = value;
    }

private:
    T arrayData[Count];
};

}

#endif