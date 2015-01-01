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

#ifndef RenderTarget_H
#define RenderTarget_H

#include "Graphics/GraphicObject.h"
#include "Util/Vector.h"

namespace Huurre3D
{

class Texture;

class RenderTarget : public GraphicObject
{
public:
    RenderTarget(GraphicSystem* graphicSystem, int width, int height, int numBuffers, int numLayers);
    ~RenderTarget() = default;
	
    void setColorBuffer(Texture* colorBuffer);
    void setDepthBuffer(Texture* depthBuffer);
    void setRenderLayer(int layer);
    void setSize(int width, int height);
    void setName(const std::string& name) {this->name = name;}
    const Vector<Texture*>& getColorBuffers() const {return colorBuffers;}
    Texture* getDepthTexture() const {return depthBuffer;}
    bool isLayered() const {return numLayers > 1;}
    unsigned int getNumBuffers() const {return numBuffers;}
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    int getRenderLayer() const {return layer;}
    const std::string& getName() const {return name;}

private:
    int width;
    int height;
    unsigned int numBuffers;
    int layer;
    int numLayers;
    Vector<Texture*> colorBuffers;
    Texture* depthBuffer = nullptr;
    std::string name;
};

}

#endif