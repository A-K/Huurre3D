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

#include "Renderer/DeferredStage.h"
#include "Renderer/Renderer.h"
#include "Renderer/Material.h"
#include "Renderer/Geometry.h"

namespace Huurre3D
{

DeferredStage::DeferredStage(Renderer* renderer):
RenderStage(renderer)
{
}

void DeferredStage::init(const JSONValue& deferredgStageJSON)
{
    auto gbufferRenderPassJSON = deferredgStageJSON.getJSONValue("GbufferRenderPass");
    if(!gbufferRenderPassJSON.isNull())
        renderPasses.pushBack(createRenderPassFromJson(gbufferRenderPassJSON));
}

void DeferredStage::clearStage()
{
    deferredRenderItems.clear();
    renderPasses[0].shaderPasses.clear();
}

void DeferredStage::update(const Scene* scene)
{
    Frustum worldSpaceCameraViewFrustum = scene->getMainCamera()->getViewFrustumInWorldSpace();
    cullRenderItems(scene, deferredRenderItems, worldSpaceCameraViewFrustum);

    Vector<unsigned int> materialBufferIndicies = renderer->getMaterialBufferIndicies();
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();
    ShaderParameterBlock* cameraShaderParameterBlock = graphicSystem->getShaderParameterBlockByName(sp_cameraParameters);

    //Create a shader pass for each material.
    for(unsigned int i = 0; i < deferredRenderItems.size(); ++i)
    {
        ShaderPass materialPass;
        deferredRenderItems[i].material->getTextures(materialPass.textures);
        materialPass.rasterState = deferredRenderItems[i].material->getRasterState();
        materialPass.vertexData = deferredRenderItems[i].geometry->getVertexData();
        materialPass.shaderParameters.pushBack(ShaderParameter(sp_worldTransform, deferredRenderItems[i].geometry->getWorldTransform()));

        unsigned int materialParameterId = deferredRenderItems[i].material->getParameterId();
        int materialBufferIndex = materialBufferIndicies.getIndexToItem(materialParameterId);
        materialPass.shaderParameters.pushBack(ShaderParameter(sp_materialParameterIndex, materialBufferIndex));
        materialPass.shaderParameterBlocks.pushBack(cameraShaderParameterBlock);

        materialPass.program = graphicSystem->getShaderCombination(deferredRenderItems[i].material->getCurrentShaderCombinationTag());
        renderPasses[0].shaderPasses.pushBack(materialPass);
    }
}

}