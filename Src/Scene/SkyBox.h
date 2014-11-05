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

#ifndef SkyBox_H
#define SkyBox_H

#include "Scene/SceneItem.h"
#include "Graphics/Texture.h"
#include "Util/FixedArray.h"

namespace Huurre3D
{

class SkyBox : public SceneItem
{
    SCENEITEM_TYPE(SkyBox);

public:
    SkyBox() = default;
    ~SkyBox() = default;

    void setTextureFiles(const FixedArray<std::string, NumCubeMapFaces>& fileNames);
    void setTexture(Texture* texture);
    const FixedArray<std::string, NumCubeMapFaces>& getTextureFiles() const { return cubeMapTextureFileNames; }
    Texture* getTexture() const {return cubeMapTexture;}

private:
    Texture* cubeMapTexture = nullptr;
    FixedArray<std::string, NumCubeMapFaces> cubeMapTextureFileNames;
};

}

#endif