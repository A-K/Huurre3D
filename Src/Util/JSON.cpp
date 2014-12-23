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

#include "Util/JSON.h"
#include <fstream>
#include <iostream>

namespace Huurre3D
{
    
JSON::~JSON()
{
    cJSON_Delete(root.value);
}

bool JSON::parseFromFile(const std::string& fileName)
{
    std::ifstream ifs(fileName);
    bool success = false;

    if(ifs)
    {
        std::string fileContent((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        cJSON* cJSONroot = cJSON_Parse(fileContent.c_str());
        if(!cJSONroot)
            std::cout << "Failed to parse json file. Error before: " << cJSON_GetErrorPtr() << std::endl;
        else
        {
            root.value = cJSONroot;
            success = true;
        }
    }
    else
        std::cout << "Failed to open file " << fileName << std::endl;

    return success;
}

}