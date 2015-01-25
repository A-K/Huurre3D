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

#ifndef Vector_H
#define Vector_H

#include "Util/MemoryBuffer.h"
#include <type_traits>
#include <initializer_list>

namespace Huurre3D
{

template<class T> class Vector : public MemoryBuffer
{
public:
    Vector():
    pod(std::is_pod<T>::value),
    count(0)
    {}

    Vector(unsigned int numItems):
    pod(std::is_pod<T>::value),
    count(numItems)
    {
        !pod ? reserveAndConstruct(numItems) : reserve(numItems * sizeof(T));
    }

    Vector(const Vector<T>& vector):
    pod(vector.pod),
    count(0)
    {
        clear();
        pushBack(vector);
    }

    Vector(std::initializer_list<T> list) :
    pod(std::is_pod<T>::value),
    count(0)
    {
        const T* iter = list.begin();
        while(iter != list.end())
        {
            pushBack(*iter);
            ++iter;
        }
    }

    ~Vector()
    {
        if(!pod)
            destructItems(items(), count);
    }

    Vector<T>& operator = (const Vector<T>& rhs)
    {
        pod = rhs.pod;
        clear();
        pushBack(rhs);
        return *this;
    }

    void pushBack(const T& item) 
    {
        pod ? append(&item, sizeof(T)) : appendAndConstruct(&item, 1);
        ++count;
    }

    void pushBack(const Vector<T>& vector) 
    {
        if(!vector.empty())
        {
            pod ? append(vector.items(), vector.getSizeInBytes()) : appendAndConstruct(vector.items(), vector.size());
            count += vector.size();
        }
    }

    void popBack()
    {
        if(!empty())
        {
            sizeInBytes -= sizeof(T);
            if(!pod)
                destructItems(end() - 1, 1);

            --count;
        }
    }

    //Doesn't resize the vector nor preserve the order of the items.
    void eraseUnordered(const T& item)
    {
        T* iter = findItem(item);
        if(iter != end())
        {
            if(*iter != back())
                *iter = std::move(*(end() - 1));

            popBack();
        }
    }

    void eraseUnordered(unsigned int index)
    {
        if(index < count)
        {
            T* iter = items();
            if (iter[index] != back())
                iter[index] = std::move(*(end() - 1));

            popBack();
        }
    }

    T* findItem(const T& item)
    {
        T* iter = items();
        while(iter != end() && *iter != item)
            ++iter;

        return iter;
    }

    const T* findItem(const T& item) const
    {
        T* iter = items();
        while(iter != end() && *iter != item)
            ++iter;

        return iter;
    }

    void findItems(const T& item, Vector<T>& result) const
    {
        T* iter = items();
        while(iter != end())
        {
            if(*iter == item)
                result.pushBack(*iter);

            ++iter;
        }
    }

    template<class F> bool findItem(const F& function, T& result) const
    {
        T* iter = items();
        while(iter != end())
        {
            if(function(*iter))
            {
                result = *iter;
                break;
            }

            ++iter;
        }

        return iter != end();
    }

    template<class F> void findItems(const F& function, Vector<T>& result) const
    {
        T* iter = items();
        while(iter != end())
        {
            if(function(*iter))
                result.pushBack(*iter);

            ++iter;
        }
    }

    int getIndexToItem(const T& item)
    {
        T* iter = items();
        int index = 0;

        while(iter != end())
        {
            if(*iter == item)
                return index;

            ++iter;
            ++index;
        }

        return -1;
    }

    template<class F>  int getIndexToItem(const F& function)
    {
        T* iter = items();
        int index = 0;

        while(iter != end())
        {
            if(function(*iter))
                return index;

            ++iter;
            ++index;
        }
        return -1;
    }

    //Removes all elements from this vector (calls their destructors).
    //Doesn't release memory.
    void clear()
    {
        if(!pod)
            destructItems(items(), count);
		
        clearBuffer();
        count = 0;
    }

    //Removes all elements from this vector (calls their destructors).
    //Releases memory.
    void reset() 
    {
        if(!pod)
            destructItems(items(), count);

        resetBuffer();
        count = 0;
    }

    T& operator [] (unsigned int index) {return items()[index];}
    const T& operator [] (unsigned int index) const {return items()[index];}
    bool containsItem(const T& item) const {return findItem(item) != end();}
    bool empty() const { return count == 0;}
    unsigned int size() const {return count;}
    T* begin() {return items();}
    T* end() {return items() + count;}
    const T* begin() const {return items();}
    const T* end() const {return items() + count;}
    T& front() {return items()[0];}
    const T& front() const {return items()[0];}
    T& back() {return items()[count - 1];}
    const T& back() const {return items()[count - 1];}

private:
    unsigned int count;
    bool pod;
    T* items() const {return reinterpret_cast<T*>(data);}

    //Used for not POD types, calls constructors/destructors.
    void appendAndConstruct(const T* data, unsigned int numItems)
    {
        unsigned int dataSize = numItems * sizeof(T);
        (sizeInBytes + dataSize) < capacity ? sizeInBytes += dataSize : resizeAndConstruct(sizeInBytes + dataSize);
        //Initialize the new copied elements
        copyConstructItems(end(), data, numItems);
    }

    //Used for not POD types, calls constructors/destructors.
    void reserveAndConstruct(unsigned int numItems)
    {
        unsigned int newCapacity = numItems * sizeof(T);

        if(newCapacity < sizeInBytes)
            newCapacity = sizeInBytes;
        
        if(newCapacity != capacity)
        {
            unsigned char* newData = nullptr;
            capacity = newCapacity;

            if(capacity)
            {
                newData = allocate(capacity);
                //Move the data into the new buffer
                constructItems(reinterpret_cast<T*>(newData), count);
            }

            data = newData;
        }
    }

    //Used for not POD types, calls constructors/destructors.
    void resizeAndConstruct(unsigned int newSize)
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
            //Move data into the new buffer and construct each item. 
            //Destruct and delete old items.
            if(data)
            {
                copyConstructItems(reinterpret_cast<T*>(newData), items(), count);
                destructItems(items(), count);
                delete[] data;
            }
            data = newData;
        }
        sizeInBytes = newSize;
    }

    void constructItems(T* items, unsigned int numItems)
    {
        for(unsigned int i = 0; i < numItems; ++i)
            new(items + i)T();
    }

    void copyConstructItems(T* items, const T* constructData, unsigned int numItems)
    {
        for(unsigned int i = 0; i < numItems; ++i)
            new(items + i)T(constructData[i]);
    }

    void destructItems(T* items, unsigned int numItems)
    {
        for(unsigned int i = 0; i < numItems; ++i)
            (items + i)->~T();
    }
};

}

#endif