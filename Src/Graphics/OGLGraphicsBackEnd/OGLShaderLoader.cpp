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

#include "Engine/Engine.h"
#include "Graphics/OGLGraphicsBackEnd/OGLShaderLoader.h"
#include "Graphics/Shader.h"
#include <iostream>
#include <fstream>

namespace Huurre3D
{

void OGLShaderLoader::load(Shader* shader)
{
    shaderInProcess = shader;
    loadShaderSource(shader->getSourceFileName());
    shader->setSource(processedSource);
    processedSource.clear();
    processedIncludes.clear();
    currentDirectoryPath.clear();
    shaderInProcess = nullptr;
}

bool OGLShaderLoader::loadShaderSource(const std::string& fileName)
{
    std::ifstream file(fileName);

    if(!file)
    {
        std::cout <<"Failed to load Shader file: " << fileName << std::endl;
        return false;
    }

    currentDirectoryPath = fileName.substr(0, fileName.find_last_of("/") + 1);
    std::string line;
    while(file.good()) 
    {
        std::getline(file, line);
        parseLine(line);
        processedSource.append(line + "\n");
    }

    file.close();
    return true;
}

void OGLShaderLoader::parseLine(std::string& line)
{
    if(line.find(version) == 0)
        addDefines(line);
    else if(line.find(include) == 0)
        processInclude(line);
}

void OGLShaderLoader::processInclude(std::string& line)
{
    std::string includeName = std::string(line, include.size(), std::string::npos);
    includeName.erase(0, includeName.find_first_not_of(" \""));
    includeName.erase(includeName.find_last_not_of(" \"") + 1);

    if(!processedIncludes.containsItem(includeName))
    {
        processedIncludes.pushBack(includeName);
        //Remove the include definition
        line = "";

        //Get the path to the actual file.
        std::string includeFile = currentDirectoryPath + includeName;
        if(!loadShaderSource(includeFile.c_str()))
            std::cout <<"Failed to load include file: " <<includeFile<<std::endl;
    }
}

void OGLShaderLoader::addDefines(std::string& line)
{
    Vector<ShaderDefine> shaderDefines = shaderInProcess->getDefines();
    std::string s;
    for(unsigned int i = 0; i < shaderDefines.size(); ++i)
    {
        line += "\n" + shaderDefines[i].defineName + ' ' + shaderDefines[i].value + "\n";
    }
}

}