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

#include "Graphics/RenderTarget.h"
#include "Graphics/Texture.h"

namespace Huurre3D
{

RenderTarget::RenderTarget(int width, int height, int numBuffers, int numLayers):
width(width),
height(height),
numBuffers(numBuffers),
numLayers(numLayers)
{
}

void RenderTarget::setColorBuffer(Texture* colorBuffer)
{
    if(colorBuffers.size() < numBuffers)
    {
        colorBuffers.pushBack(colorBuffer);
        dirty = true;
    }
}

void RenderTarget::setDepthBuffer(Texture* depthBuffer)
{
    this->depthBuffer = depthBuffer;
    dirty = true;
}

void RenderTarget::setRenderLayer(int layer)
{
    if(numLayers > 1)
    {
        this->layer = layer;
        dirty = true;
    }
}

void RenderTarget::setSize(int width, int height)
{
    this->width = width;
    this->height = height;

    for(unsigned int i = 0; i < colorBuffers.size(); ++i)
        colorBuffers[i]->setSize(width, height);

    if(depthBuffer)
        depthBuffer->setSize(width, height);
}

}