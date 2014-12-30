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

#ifndef EnumClassDeclaration_H
#define EnumClassDeclaration_H

#include "Util/Vector.h"
#include <string>
#include <algorithm>

namespace Huurre3D
{
    
//EnumStrings holds all the string values of each enum.
//Each enum will declare its own specialization.
template<class T> struct EnumStrings
{
    static Vector<std::string> strings;
};

static Vector<std::string> setEnumStrings(const char* enumVarList)
{
    Vector<std::string> result;
    std::string str(enumVarList);
    const char delimeter = ',';

    //Remove spaces.
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

    //Parse varArgs.
    int start = 0, end = 0;
    while((end = str.find(delimeter, start)) != std::string::npos)
    {
        result.pushBack(str.substr(start, end - start));
        start = end + 1;
    }
    result.pushBack(str.substr(start));

    return result;
}

#define DECLARE_ENUM_CLASS(Name, ...)\
enum class Name\
{\
    __VA_ARGS__\
};\
template<> Vector<std::string> EnumStrings<Name>::strings = setEnumStrings(#__VA_ARGS__);


template<class T> T enumFromString(const std::string& str) 
{ 
    int index = EnumStrings<T>::strings.getIndexToItem(str);
    return static_cast<T>(index);
}

}

#endif