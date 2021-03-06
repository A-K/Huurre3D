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

#ifndef TextureLoader_H
#define TextureLoader_H

#include "Graphics/GraphicDefs.h"
#include "Util/FixedArray.h"
#include <string>

namespace Huurre3D
{

struct TextureLoadResult
{
    TextureTargetMode targetMode;
    int width;
    int height;
    int numMipMaps = 10;
    TexturePixelFormat format;
    MemoryBuffer pixelData;
};

class TextureLoader
{
public:
    TextureLoader() = default;
    ~TextureLoader() = default;
    
    bool loadFromFile(const std::string& fileName, bool flipVertically, TextureLoadResult& result) const;
    bool loadCubeMapFromFile(const FixedArray<std::string, NumCubeMapFaces>& cubeFileNames, bool flipVertically, TextureLoadResult& result) const;

private:
    bool loadPixelDataFromFiles(const Vector<std::string>& fileNames, TextureLoadResult& result, bool flipVertically) const;
    void flipVertically(TextureLoadResult& result, unsigned int offset) const;
    void flipCompressedVertically(TextureLoadResult& result, unsigned int offset) const;
    void flipCompressedBlocks(unsigned char* blockData, unsigned int blockSizeBytes, unsigned int numBlocks, TexturePixelFormat format) const;
};

}

#endif