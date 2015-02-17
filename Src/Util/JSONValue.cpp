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

void JSONValue::setJSONValue(const JSONValue& valueJSON)
{
    cJSON_AddItemReferenceToObject(value, valueJSON.value->string, valueJSON.value);
}

JSONValue JSONValue::getJSONValue(const std::string& name) const
{
    return JSONValue(cJSON_GetObjectItem(value, name.c_str()));
}

JSONValue JSONValue::getJSONArrayItem(const unsigned int index) const
{
    return JSONValue(cJSON_GetArrayItem(value, index));
}

unsigned int JSONValue::getSize() const
{
    return cJSON_GetArraySize(value);
}

int JSONValue::getInt() const
{
    return value->valueint;
}

FixedArray<int, 2> JSONValue::getInt2() const
{
    FixedArray<int, 2> data;
    for(int i = 0; i < cJSON_GetArraySize(value); i++)
        data[i] = cJSON_GetArrayItem(value, i)->valueint;

    return data;
}

FixedArray<int, 3> JSONValue::getInt3() const
{
    FixedArray<int, 3> data;
    for(int i = 0; i < cJSON_GetArraySize(value); i++)
        data[i] = cJSON_GetArrayItem(value, i)->valueint;

    return data;
}

FixedArray<int, 4> JSONValue::getInt4() const
{
    FixedArray<int, 4> data;
    for(int i = 0; i < cJSON_GetArraySize(value); i++)
        data[i] = cJSON_GetArrayItem(value, i)->valueint;

    return data;
}

bool JSONValue::getBool() const
{
    return value->type != 0;
}

float JSONValue::getFloat() const
{
    return static_cast<float>(value->valuedouble);
}

Vector2 JSONValue::getVector2() const
{
    float data[2];
    for(int i = 0; i < cJSON_GetArraySize(value); i++)
    {
        cJSON* subitem = cJSON_GetArrayItem(value, i);
        data[i] = static_cast<float>(subitem->valuedouble);
    }

    return Vector2(data);
}

Vector3 JSONValue::getVector3() const
{
    float data[3];
    for(int i = 0; i < cJSON_GetArraySize(value); i++)
    {
        cJSON* subitem = cJSON_GetArrayItem(value, i);
        data[i] = static_cast<float>(subitem->valuedouble);
    }

    return Vector3(data);
}

Vector4 JSONValue::getVector4() const
{
    float data[4];
    for(int i = 0; i < cJSON_GetArraySize(value); i++)
    {
        cJSON* subitem = cJSON_GetArrayItem(value, i);
        data[i] = static_cast<float>(subitem->valuedouble);
    }

    return Vector4(data);
}

Matrix4x4 JSONValue::getMatrix4x4() const
{
    float data[16];
    for(int i = 0; i < cJSON_GetArraySize(value); i++)
    {
        cJSON* subitem = cJSON_GetArrayItem(value, i);
        data[i] = static_cast<float>(subitem->valuedouble);
    }

    return Matrix4x4(Vector4(&data[0]), Vector4(&data[4]), Vector4(&data[8]), Vector4(&data[12]));
}

const std::string JSONValue::getString() const
{
    return std::string(value->valuestring);
}

}