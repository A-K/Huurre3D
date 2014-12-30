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

#ifndef JSONValue_H
#define JSONValue_H

#include "Math/Matrix4x4.h"
#include "Util/FixedArray.h"
#include "ThirdParty/cJSON/cJSON.h"
#include <string>

namespace Huurre3D
{

class JSONValue
{
    friend class JSON;

public:
    JSONValue() = default;
    JSONValue(cJSON* value);
    ~JSONValue() = default;

    JSONValue& operator = (const JSONValue& rhs);
    JSONValue getJSONValue(const std::string& name) const;
    JSONValue getJSONArrayItem(const unsigned int index) const;
    //Returns array size.
    unsigned int getSize() const;
    int getInt() const;
    FixedArray<int, 2> getInt2() const;
    FixedArray<int, 3> getInt3() const;
    FixedArray<int, 4> getInt4() const;
    bool getBool() const;
    float getFloat() const;
    Vector2 getVector2() const;
    Vector3 getVector3() const;
    Vector4 getVector4() const;
    Matrix4x4 getMatrix4x4() const;
    const std::string getString() const;
    bool isNull() const { return value == nullptr; }

private:
    cJSON* value = nullptr;
};

}

#endif