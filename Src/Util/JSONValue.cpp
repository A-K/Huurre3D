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

#include "JSONValue.h"

namespace Huurre3D
{

JSONValue::JSONValue(cJSON* value) :
value(value)
{
}

JSONValue& JSONValue::operator = (const JSONValue& rhs)
{
    value = rhs.value;
    return *this;
}

JSONValue JSONValue::getJSONObject(const std::string& name) const
{
    return JSONValue(cJSON_GetObjectItem(value, name.c_str()));
}

int JSONValue::getInt(const std::string& name) const
{
    return cJSON_GetObjectItem(value, name.c_str())->valueint;
}

bool JSONValue::getBool(const std::string& name) const
{
    return cJSON_GetObjectItem(value, name.c_str())->type != 0;
}

float JSONValue::getFloat(const std::string& name) const
{
    return static_cast<float>(cJSON_GetObjectItem(value, name.c_str())->valuedouble);
}

Vector4 JSONValue::getVector4(const std::string& name) const
{
    float data[4];
    cJSON* cJSONarray = cJSON_GetObjectItem(value, name.c_str());
    for(int i = 0; i < cJSON_GetArraySize(cJSONarray); i++)
    {
        cJSON* subitem = cJSON_GetArrayItem(cJSONarray, i);
        data[0] = static_cast<float>(subitem->valuedouble);
    }

    return Vector4(data);
}

const std::string JSONValue::getString(const std::string& name) const
{
    return std::string(cJSON_GetObjectItem(value, name.c_str())->valuestring);
}

}