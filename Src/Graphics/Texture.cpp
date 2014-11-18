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

#include "Graphics/Texture.h"

namespace Huurre3D
{

Texture::Texture(GraphicSystem* graphicSystem, TextureTargetMode targetMode, TextureWrapMode wrapMode, TextureFilterMode filterMode, TexturePixelFormat pixelFormat, int width, int height):
targetMode(targetMode),
wrapMode(wrapMode),
filterMode(filterMode),
pixelFormat(pixelFormat),
width(width),
height(height),
GraphicObject(graphicSystem),
GraphicDataContainer(true)
{
}

void Texture::setWidth(int width)
{
    this->width = width;
    dataDirty = true;
}

void Texture::setHeight(int height)
{
    this->height = height;
    dataDirty = true;
}

void Texture::setDepth(int depth)
{
    this->depth = depth;
    dataDirty = true;
}

void Texture::setNumMipMaps(int numMipMaps)
{
    this->numMipMaps = numMipMaps;
    paramsDirty = true;
    dataDirty = true;
}

void Texture::setData(const TextureLoadResult& resultData)
{
    if(resultData.width == width && resultData.height == height && resultData.format == pixelFormat)
    {
        numMipMaps = resultData.numMipMaps;

        if(resultData.targetMode == TextureTargetMode::Texture2D)
        {
            setPixelData(resultData.pixelData[0], resultData.pixelDataSize);
        }
        else if(resultData.targetMode == TextureTargetMode::TextureCubeMap)
        {
            //Copy the data from each cube map face into the buffer.
            for(unsigned int i = 0; i < NumCubeMapFaces; ++i)
                append(resultData.pixelData[i], resultData.pixelDataSize);

            //Get the pointers to the data of each face.
            for(unsigned int i = 0; i < NumCubeMapFaces; ++i)
                cubeMapFaceData[i] = &data[i * resultData.pixelDataSize];
        }
    }
    else
        std::cout << "Failed to set data" << std::endl;
}

}