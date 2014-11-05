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

#ifndef GBuffer_H
#define GBuffer_H

#include "Graphics/GraphicSystem.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderTarget.h"

namespace Huurre3D
{

class GraphicSystem;

struct GBuffer
{
    RenderTarget* renderTarget = nullptr;
    Texture* diffuse = nullptr;
    Texture* specular = nullptr;
    Texture* normal = nullptr;
    GraphicSystem* graphicSystem = nullptr;

    GBuffer(GraphicSystem* graphicSystem, int width, int height)
    {
        renderTarget = graphicSystem->createRenderTarget(width, height, true, 3);

        diffuse = graphicSystem->createTexture(TextureTargetMode::Texture2D, TextureWrapMode::ClampEdge, TextureFilterMode::Bilinear, TexturePixelFormat::Rgba8, width, height);
        diffuse->setSlotIndex(TextureSlotIndex::DiffuseBuffer);
        renderTarget->setColorBuffer(diffuse);

        specular = graphicSystem->createTexture(TextureTargetMode::Texture2D, TextureWrapMode::ClampEdge, TextureFilterMode::Bilinear, TexturePixelFormat::Rgba8, width, height);
        specular->setSlotIndex(TextureSlotIndex::SpecularBuffer);
        renderTarget->setColorBuffer(specular);

        normal = graphicSystem->createTexture(TextureTargetMode::Texture2D, TextureWrapMode::ClampEdge, TextureFilterMode::Bilinear , TexturePixelFormat::Rgba32F, width, height);
        normal->setSlotIndex(TextureSlotIndex::NormalBuffer);
        renderTarget->setColorBuffer(normal);

        this->graphicSystem = graphicSystem;
    }

    ~GBuffer()
    {
        graphicSystem->removeRenderTarget(renderTarget);
        graphicSystem->removeTexture(diffuse);
        graphicSystem->removeTexture(specular);
        graphicSystem->removeTexture(normal);
    }
};

}

#endif