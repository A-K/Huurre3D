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


#include "TextureLoader.h"
#include "Math/MathFunctions.h"
#include "ThirdParty/Stb_image/stb_image.h"
#include <iostream>
#include <fstream>

namespace Huurre3D
{

// compressed texture types
const unsigned int FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
const unsigned int FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
const unsigned int FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))

struct DDSPixelFormat 
{
    unsigned int dwSize;
    unsigned int dwFlags;
    unsigned int dwFourCC;
    unsigned int dwRGBBitCount;
    unsigned int dwRBitMask;
    unsigned int dwGBitMask;
    unsigned int dwBBitMask;
    unsigned int dwABitMask;
};

struct DDSHeader 
{
    unsigned int dwSize;
    unsigned int dwFlags;
    unsigned int dwHeight;
    unsigned int dwWidth;
    unsigned int dwPitchOrLinearSize;
    unsigned int dwDepth;
    unsigned int dwMipMapCount;
    unsigned int dwReserved1[11];
    DDSPixelFormat ddspf;
    unsigned int dwCaps1;
    unsigned int dwCaps2;
    unsigned int dwReserved2[3];
};

bool TextureLoader::loadFromFile(const std::string& fileName, bool flipVertically, TextureLoadResult& result) const
{
    result.targetMode = TextureTargetMode::Texture2D;
    Vector<std::string> fileNames;
    fileNames.pushBack(fileName);
    return loadPixelDataFromFiles(fileNames, result, flipVertically);
}

bool TextureLoader::loadCubeMapFromFile(const FixedArray<std::string, NumCubeMapFaces>& cubeFileNames, bool flipVertically, TextureLoadResult& result) const
{
    result.targetMode = TextureTargetMode::TextureCubeMap;
    Vector<std::string> fileNames;
    fileNames.pushBack(cubeFileNames.data(), NumCubeMapFaces);
    return loadPixelDataFromFiles(fileNames, result, flipVertically);
}

//TODO: Loading images with unusual width and height, for example (409, 447), leads to memory corruption
//and this will crash the program due to access violation later on when the data is used in glTexImage2D.
//Investigate texture sizes that are not divisible by two.
bool TextureLoader::loadPixelDataFromFiles(const Vector<std::string>& fileNames, TextureLoadResult& result, bool flipVertically) const
{
    bool compressed = false;
    unsigned int offset = 0;

    for(unsigned int i = 0; i < fileNames.size(); ++i)
    {
        std::string fileExtension = "";
        if(fileNames[i].find_last_of(".") != std::string::npos)
            fileExtension = fileNames[i].substr(fileNames[i].find_last_of(".") + 1);

        if(fileExtension == "dds" || fileExtension == "DDS")
        {
            std::ifstream is(fileNames[i], std::ifstream::binary);
            if(!is)
                std::cout << "Failed to open file " << fileNames[i] << std::endl;

            //Verify the type of file.
            char filecode[4];
            is.read(filecode, 4);
            if(strncmp(filecode, "DDS ", 4) != 0)
            {
                is.close();
                std::cout << "Failed to load image: " << fileNames[i] << " because it is not a direct draw surface file: " << std::endl;
                return false;
            }

            //Read in DDS header
            DDSHeader ddsHeader;
            is.read((char*)&ddsHeader, sizeof(DDSHeader));

            switch(ddsHeader.ddspf.dwFourCC)
            {
            case FOURCC_DXT1:
                result.format = TexturePixelFormat::DXT1;
                break;
            case FOURCC_DXT3:
                result.format = TexturePixelFormat::DXT3;
                break;
            case FOURCC_DXT5:
                result.format = TexturePixelFormat::DXT5;
                break;
            default:
                is.close();
                char fourcc[4];
                fourcc[0] = ddsHeader.ddspf.dwFourCC >> 0 & 0xFF;
                fourcc[1] = ddsHeader.ddspf.dwFourCC >> 8 & 0xFF;
                fourcc[2] = ddsHeader.ddspf.dwFourCC >> 16 & 0xFF;
                fourcc[3] = ddsHeader.ddspf.dwFourCC >> 24 & 0xFF;
                std::cout << "Unsupported DDS format: " << fourcc << std::endl;
                return false;
            }

            result.width = ddsHeader.dwWidth;
            result.height = ddsHeader.dwHeight;
            result.numMipMaps = ddsHeader.dwMipMapCount;
            int size = ddsHeader.dwPitchOrLinearSize * 2;
            result.pixelData.resize(size + offset);
            is.read((char*)&result.pixelData.getData()[offset], size);
            is.close();
            compressed = true;
        }
        else
        {
            int numComponents;
            unsigned char* pixelData = stbi_load(fileNames[i].c_str(), &result.width, &result.height, &numComponents, 0);

            if(!pixelData)
            {
                std::cout << "Failed to load image: " << fileNames[i] << " because " << std::string(stbi_failure_reason()) << std::endl;
                return false;
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

                result.pixelData.append(pixelData, result.width * result.height * pixelFormatSizeInBytes[static_cast<int>(result.format)]);
                stbi_image_free(pixelData);
            }
        }

        if(flipVertically)
            compressed ? flipCompressedVertically(result, offset) : this->flipVertically(result, offset);
    
        offset += i * result.width * result.height * pixelFormatSizeInBytes[static_cast<int>(result.format)];
    }
    return true;
}

void TextureLoader::flipVertically(TextureLoadResult& result, unsigned int offset) const
{
    unsigned int widthBytes = result.width * pixelFormatSizeInBytes[static_cast<int>(result.format)];
    unsigned char* top = nullptr;
    unsigned char* bottom = nullptr;
    unsigned char temp;
    unsigned int halfHeight = result.height / 2;

    for(unsigned int row = 0; row < halfHeight; ++row)
    {
        top = &result.pixelData.getData()[offset] + row * widthBytes;
        bottom = &result.pixelData.getData()[offset] + (result.height - row - 1) * widthBytes;
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

void TextureLoader::flipCompressedVertically(TextureLoadResult& result, unsigned int offset) const
{
    unsigned int blockSizeBytes = pixelFormatSizeInBytes[static_cast<int>(result.format)];
    unsigned int width = result.width;
    unsigned int height = result.height;
    unsigned char* top = nullptr;
    unsigned char* bottom = nullptr;
    unsigned char* temp = new unsigned char[blockSizeBytes * ((result.width + 3) / 4)];
    unsigned int mipMapOffset = 0;

    for(unsigned int i = 0; i < result.numMipMaps; ++i)
    {
        unsigned int numBlocksHorizontal = (width + 3) / 4;
        unsigned int numBlocksVertical = (height + 3) / 4;
        unsigned int halfHeight = numBlocksVertical / 2;
        unsigned int rowByteCount = blockSizeBytes * numBlocksHorizontal;

        for(unsigned int row = 0; row < halfHeight; ++row)
        {
            top = &result.pixelData.getData()[offset] + mipMapOffset + row * rowByteCount;
            bottom = &result.pixelData.getData()[offset] + mipMapOffset + (numBlocksVertical - row - 1) * rowByteCount;

            flipCompressedBlocks(top, blockSizeBytes, numBlocksHorizontal, result.format);
            flipCompressedBlocks(bottom, blockSizeBytes, numBlocksHorizontal, result.format);

            memcpy(temp, top, rowByteCount);
            memcpy(top, bottom, rowByteCount);
            memcpy(bottom, temp, rowByteCount);
        }

        mipMapOffset += numBlocksHorizontal * rowByteCount;
        width = max(1, width / 2);
        height = max(1, height / 2);
    }
    delete[] temp;
}

void TextureLoader::flipCompressedBlocks(unsigned char* blockData, unsigned int blockSizeBytes, unsigned int numBlocks, TexturePixelFormat format) const
{
    switch(format)
    {
        case TexturePixelFormat::DXT1:
            //Block size in DXT1 is 8 bytes, first 4 bytes contains color values, and remaining 4 bytes contains a lookup table.
            //Only the lookup table (4-7), needs to be flipped.
            for(unsigned int block = 0; block < numBlocks * blockSizeBytes; block += blockSizeBytes)
            {
                std::swap(blockData[block + 4], blockData[block + 7]);
                std::swap(blockData[block + 5], blockData[block + 6]);
            }
            break;

    case TexturePixelFormat::DXT3:
        //Block size in DXT3 is 16 bytes, first 8 bytes contains alpha values and the remaining 8 bytes are encoded as in DXT1.
        //Alpha values (0-7) and lookup table (12-15)  must be flipped.
        for(unsigned int block = 0; block < numBlocks * blockSizeBytes; block += blockSizeBytes)
        {
            std::swap(blockData[block + 0], blockData[block + 6]);
            std::swap(blockData[block + 1], blockData[block + 7]);
            std::swap(blockData[block + 2], blockData[block + 4]);
            std::swap(blockData[block + 3], blockData[block + 5]);

            std::swap(blockData[block + 12], blockData[block + 15]);
            std::swap(blockData[block + 13], blockData[block + 14]);
        }
        break;

    case TexturePixelFormat::DXT5:
        //Block size in DXT5 is 16 bytes, first 2 bytes contains alpha values, next 6 bytes contains a 4x4 3 bit lookup table, and the remaining 8 bytes are encoded as in DXT1.
        //Alpha lookup table (2-7) and color lookup table (12-15)  must be flipped.
        for(unsigned int block = 0; block < numBlocks * blockSizeBytes; block += blockSizeBytes)
        {
            unsigned int row01 = blockData[block + 2] | ((unsigned int)blockData[block + 3] << 8) | ((unsigned int)blockData[block + 4] << 16);
            unsigned int row23 = blockData[block + 5] | ((unsigned int)blockData[block + 6] << 8) | ((unsigned int)blockData[block + 7] << 16);
            unsigned int row10 = ((row01 & 0x000fff) << 12) | (row01 & 0xfff000) >> 12;
            unsigned int row32 = ((row23 & 0x000fff) << 12) | (row23 & 0xfff000) >> 12;
            blockData[block + 2] = row32 & 0xff;
            blockData[block + 3] = (row32 >> 8) & 0xff;
            blockData[block + 4] = (row32 >> 16) & 0xff;
            blockData[block + 5] = row10 & 0xff;
            blockData[block + 6] = (row10 >> 8) & 0xff;
            blockData[block + 7] = (row10 >> 16) & 0xff;

            std::swap(blockData[block + 12], blockData[block + 15]);
            std::swap(blockData[block + 13], blockData[block + 14]);
        }
        break;
    }
}

}