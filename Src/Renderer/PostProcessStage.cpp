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

#include "Engine/Engine.h"
#include "Renderer/PostProcessStage.h"
#include "Renderer/Renderer.h"

namespace Huurre3D
{

PostProcessStage::PostProcessStage(Renderer& renderer):
RenderStage(renderer)
{
}

void PostProcessStage::update(const Scene& scene)
{
    Texture* skyBoxTexture = renderer.getGraphicSystem().getTextureBySlotIndex(TextureSlotIndex::SkyBoxTex);

    if(skyBoxTexture)
    {
        Vector<SkyBox*> skyBoxes;
        scene.getSceneItemsByType<SkyBox>(skyBoxes);

        if(currentSkyBox != skyBoxes[0])
        {
            TextureLoadResult result;
            if(renderer.getTextureLoader().loadCubeMapFromFile(skyBoxes[0]->getTextureFiles(), false, result))
            {
                skyBoxTexture->setPixelFormat(result.format);
                skyBoxTexture->setWidth(result.width);
                skyBoxTexture->setHeight(result.height);
                skyBoxTexture->setData(result);
            }
            currentSkyBox = skyBoxes[0];
        }
    }
}

}
