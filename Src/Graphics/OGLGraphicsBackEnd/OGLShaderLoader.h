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

#ifndef OGLShaderLoader_H
#define OGLShaderLoader_H

#include "Util/Vector.h"
#include <string>

namespace Huurre3D
{

class Shader;

class OGLShaderLoader
{
public:
    OGLShaderLoader() = default;
    ~OGLShaderLoader() = default;
    void load(Shader* shader);

private: 
    bool loadShaderSource(const std::string& fileName);
    void parseLine(std::string& line);
    void processInclude(std::string& line);
    void addDefines(std::string& line);

    Shader* shaderInProcess = nullptr;
    std::string processedSource;
    Vector<std::string> processedIncludes;
    std::string currentDirectoryPath;
    const std::string include = "#include";
    const std::string version = "#version";
    const std::string define = "#define ";
};

}

#endif

