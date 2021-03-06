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

#ifndef RenderStage_H
#define RenderStage_H

#include "Scene/Camera.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderItem.h"
#include "Renderer/RenderPasses.h"
#include "Renderer/RenderStageFactory.h"
#include "Util/JSONValue.h"

namespace Huurre3D
{

class ShaderProgram;
class ShaderParameterBlock;
class VertexData;

class Renderer;

class RenderStage
{
public:
    RenderStage(Renderer& renderer);
    virtual ~RenderStage() = default;
    virtual void init(const JSONValue& renderStageJSON);
    virtual void resizeResources();
    virtual void clearStage() {}
    virtual void update(const Scene& scene) {}
    virtual void execute() const { drawRenderPasses(renderPasses); }

protected:
    RenderPass createRenderPassFromJson(const JSONValue& renderPassJSON);
    void drawRenderPasses(const Vector<RenderPass>& renderPasses) const;
    Renderer& renderer;
    Vector<RenderPass> renderPasses;
};

}

#endif