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

#ifndef ShadowStage_H
#define ShadowStage_H

#include "Renderer/RenderStage.h"
#include "Renderer/ShadowProjector.h"

namespace Huurre3D
{

class Renderer;
class Light;
class Camera;

class ShadowStage : public RenderStage
{
    RENDERSTAGE_TYPE(ShadowStage)

public:
    ShadowStage(Renderer* renderer);
    ~ShadowStage() = default;
    void clearStage() override;
    void init(const JSONValue& shadowStageJSON) override;
    void resizeResources() override;
    void update(const Scene* scene) override;

private:
    void calculateShadowCameraViewProjections(const Vector<Light*>& lights, Camera* camera);
    void drawShadowDepthPasses();
    void createLightShadowPasses(const Vector<RenderItem>& renderItems);
    Vector<Light*> shadowLights;
    Vector<RenderItem> shadowRenderItems;
    RenderPass shadowOcllusionRenderPass;
    RenderPass shadowDepthRenderPass;
    ShadowProjector shadowProjector;
    Vector<RenderItem> itemsInShadowfrustum;
    Vector<ShadowDepthData> shadowDepthData;
    Vector<ShadowOcclusionData> shadowOcclusionData;
};

}

#endif