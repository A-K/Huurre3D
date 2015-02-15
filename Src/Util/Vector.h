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

template<class T> class Vector
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
        data.resize(numItems * sizeof(T));
        if(!pod)
            constructItems(items(), count);
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
        pod ? data.append(&item, sizeof(T)) : appendAndCopyConstruct(&item, 1);
        ++count;
    }

    void pushBack(T&& item)
    {
        pod ? data.append(&item, sizeof(T)) : appendAndMoveConstruct(std::move(item), 1);
        ++count;
    }

    void pushBack(const T* items, unsigned int numItems)
    {
        if(items)
        {
            pod ? data.append(items, sizeof(T) * numItems) : appendAndCopyConstruct(items, numItems);
            count += numItems;
        }
    }

    void pushBack(const Vector<T>& vector) 
    {
        if(!vector.empty())
        {
            pod ? data.append(vector.items(), vector.getSizeInBytes()) : appendAndCopyConstruct(vector.items(), vector.size());
            count += vector.size();
        }
    }

    void popBack()
    {
        if(!empty())
        {
            data.resize(data.getSizeInBytes() - sizeof(T));
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
		
        data.clearBuffer();
        count = 0;
    }

    //Removes all elements from this vector (calls their destructors).
    //Releases memory.
    void reset() 
    {
        if(!pod)
            destructItems(items(), count);

        data.resetBuffer();
        count = 0;
    }

    void fill(const T& value)
    {
        T* iter = items();
        while(iter != end())
        {
            *iter = value;
            ++iter;
        }
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
    T* getData() const {return items();}
    unsigned int getSizeInBytes() const {return data.getSizeInBytes();}
    void reserve(unsigned int numItems) {data.reserve(numItems * sizeof(T));}
    MemoryBuffer& getMemoryBuffer() {return data;}

private:
    unsigned int count;
    bool pod;
    MemoryBuffer data;
    T* items() const { return reinterpret_cast<T*>(data.getData()); }

    void appendAndCopyConstruct(const T* itemData, unsigned int numItems)
    {
        append(numItems);
        copyConstructItems(end(), itemData, numItems);
    }

    void appendAndMoveConstruct(T&& itemData, unsigned int numItems)
    {
        append(numItems);
        new(end())T(std::move(itemData));
    }

    void append(unsigned int numItems)
    {
        unsigned int dataCapacityInbytes = data.getCapacity();
        unsigned int newDataSizeInBytes = data.getSizeInBytes() + numItems * sizeof(T);

        if(newDataSizeInBytes > dataCapacityInbytes)
        {
            MemoryBuffer newData;
            dataCapacityInbytes == 0 ? newData.reserve(newDataSizeInBytes * 2) : newData.reserve(newDataSizeInBytes);
            moveConstructItems(reinterpret_cast<T*>(newData.getData()), items(), count);
            destructItems(items(), count);
            data = std::move(newData);
        }
        //Set current size.
        data.resize(newDataSizeInBytes);
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

    void moveConstructItems(T* items, const T* constructData, unsigned int numItems)
    {
        for(unsigned int i = 0; i < numItems; ++i)
            new(items + i)T(std::move(constructData[i]));
    }

    void destructItems(T* items, unsigned int numItems)
    {
        for(unsigned int i = 0; i < numItems; ++i)
            (items + i)->~T();
    }
};

}

#endif