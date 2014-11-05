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

#include "TextureLoader.h"
#include "ThirdParty/Stb_image/stb_image.h"
#include <iostream>

namespace Huurre3D
{

const TextureLoadResult TextureLoader::loadFromFile(const std::string& fileName, bool flipVertically)
{
    TextureLoadResult result;
    result.cubeMap = false;
    result.pixelData.fill(nullptr);

    loadPixelDataFromFile(fileName.c_str(), false, result);

    if(result.pixelData[0] && flipVertically)
        this->flipVertically(result, 0);

    return result;
}

const TextureLoadResult TextureLoader::loadCubeMapFromFile(const FixedArray<std::string, NumCubeMapFaces>& fileNames, const FixedArray<bool, NumCubeMapFaces>& flipVertically)
{
    TextureLoadResult result;
    result.cubeMap = true;
    result.pixelData.fill(nullptr);

    for(unsigned int i = 0; i < NumCubeMapFaces; ++i)
    {
        loadPixelDataFromFile(fileNames[i].c_str(), i, result);

        if(result.pixelData[i] && flipVertically[i])
            this->flipVertically(result, i);
    }

    return result;
}

//TODO: Loading images with unusual width and height, for example (409, 447), leads to memory corruption
//and this will crash the program due to access violation later on when the data is used in glTexImage2D.
//Investigate texture sizes that are not divisible by two.
void TextureLoader::loadPixelDataFromFile(const char* fileName, int index, TextureLoadResult& result)
{
    int numComponents;

    unsigned char* pixelData = stbi_load(fileName, &result.width, &result.height, &numComponents, 0);

    if(!pixelData)
    {
        std::cout << "Failed to load image: " << fileName << " because " << std::string(stbi_failure_reason()) << std::endl;
    }
    else
    {
        switch(numComponents)
        {
            case 3:
                result.format = TexturePixelFormat::Rgb8;
                break;
            case 4:
                result.format = TexturePixelFormat::Rgba8;
                break;
        }

        result.pixelData[index] = pixelData;
    }
}

void TextureLoader::releasedata(TextureLoadResult& resultData)
{
    if(resultData.cubeMap)
    {
        for(unsigned int i = 0; i < NumCubeMapFaces; ++i)
        {
            if(resultData.pixelData[i])
                stbi_image_free(resultData.pixelData[i]);
        }
    }
    else
    {
        if(resultData.pixelData[0])
            stbi_image_free(resultData.pixelData[0]);
    }
}

void TextureLoader::flipVertically(TextureLoadResult& result, int index)
{
    unsigned int widthBytes = result.width * pixelFormaSizeInBytes[static_cast<int>(result.format)];
    unsigned char *top = 0;
    unsigned char *bottom = 0;
    unsigned char temp = 0;
    unsigned int halfHeight = result.height / 2;

    for(unsigned int row = 0; row < halfHeight; ++row)
    {
        top = result.pixelData[index] + row * widthBytes;
        bottom = result.pixelData[index] + (result.height - row - 1) * widthBytes;
        for(unsigned int col = 0; col < widthBytes; ++col)
        {
            temp = *top;
            *top = *bottom;
            *bottom = temp;
            top++;
            bottom++;
        }
    }
}

}