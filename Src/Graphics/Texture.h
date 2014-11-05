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

#ifndef Texture_H
#define Texture_H

#include "Graphics/GraphicObject.h"
#include "Graphics/GraphicDataContainer.h"
#include "Renderer/TextureLoader.h"
#include "Util/FixedArray.h"
#include <iostream>

namespace Huurre3D
{

class Texture : public GraphicObject, public GraphicDataContainer
{
public:
    Texture(GraphicSystem* graphicSystem, TextureTargetMode targetMode, TextureWrapMode wrapMode, TextureFilterMode filterMode, TexturePixelFormat pixelFormat, int width, int height);
    ~Texture() = default;
	
    void setWidth(int width);
    void setHeight(int height);
    void setDepth(int depth);
    void setMipMapLevel(int mipMapLevel);
    void setSlotIndex(TextureSlotIndex slotIndex) {this->slotIndex = slotIndex;}
    void setData(const TextureLoadResult& resultData);
    TextureTargetMode getTargetMode() const {return targetMode;}
    TextureWrapMode getWrapMode() const {return wrapMode;}
    TextureFilterMode getFilterMode() const {return filterMode;}
    TexturePixelFormat getPixelFormat() const {return pixelFormat;}
    TextureSlotIndex getSlotIndex() const {return slotIndex;}
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    int getDepth() const {return depth;}
    int getMipMapLevel() const {return mipMapLevel;}
    bool isParamsDirty() const {return paramsDirty;}
    bool isDataDirty() const {return dataDirty;}
    void unDirtyParams() {paramsDirty = false;}
    void unDirtyData() {dataDirty = false;}
    const unsigned char* getCubeMapFaceData(CubeMapFace face) const {return cubeMapFaceData[static_cast<int>(face)];}
    template<typename T> void setPixelData(T* pixelData, int dataSize)
    {
        if(!pixelData || !dataSize || dataSize == 0)
            return;

        bufferData(pixelData, dataSize);
        if(!this->data)
        {
            std::cout <<"Failed to set pixelData" <<std::endl;
        }
        dataDirty = true;
    }

private:
    TextureTargetMode targetMode;
    TextureWrapMode wrapMode;
    TextureFilterMode filterMode;
    TexturePixelFormat pixelFormat;
    TextureSlotIndex slotIndex;
    int width;
    int height;
    int depth = 1;
    int mipMapLevel = 0; //numMipMaps could be better name?
    bool paramsDirty = true;
    bool dataDirty = true;
    FixedArray<unsigned char*, NumCubeMapFaces> cubeMapFaceData;
};

}

#endif