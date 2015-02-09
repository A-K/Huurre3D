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
#include "Renderer/ShadowStage.h"
#include "Renderer/Renderer.h"
#include "Renderer/Geometry.h"
#include "Graphics/ShaderParameterBlock.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"
#include "Scene/SceneCuller.h"

namespace Huurre3D
{

ShadowStage::ShadowStage(Renderer* renderer):
RenderStage(renderer)
{
}

void ShadowStage::init(const JSONValue& shadowStageJSON)
{
    auto shadowDepthRenderPassJSON = shadowStageJSON.getJSONValue("shadowDepthRenderPass");
    auto shadowOcclusionRenderPassJSON = shadowStageJSON.getJSONValue("shadowOcclusionRenderPass");

    if(!shadowDepthRenderPassJSON.isNull())
    {
        shadowDepthRenderPass = createRenderPassFromJson(shadowDepthRenderPassJSON);
        shadowProjector.setShadowMapSize(static_cast<float>(shadowDepthRenderPass.renderTarget->getWidth()));
    }

    if(!shadowOcclusionRenderPassJSON.isNull())
    {
        shadowOcllusionRenderPass = createRenderPassFromJson(shadowOcclusionRenderPassJSON);
        shadowOcllusionRenderPass.shaderPasses[0].shaderParameters.pushBack(ShaderParameter(sp_shadowOcclusionParameterIndex, 0));
    }
}

void ShadowStage::resizeResources()
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    shadowOcllusionRenderPass.viewPort = screenViewPort;
    shadowOcllusionRenderPass.renderTarget->setSize(screenViewPort.width, screenViewPort.height);
}

void ShadowStage::update(const Scene* scene)
{
    scene->getAllRenderItems(shadowRenderItems);
    Camera* camera = scene->getMainCamera();
    Frustum worldSpaceCameraViewFrustum = camera->getViewFrustumInWorldSpace();
    Vector<Light*> lights;
    cullLights(scene, lights, worldSpaceCameraViewFrustum);
    lights.findItems([](const Light* light) {return light->getCastShadow(); }, shadowLights);

    if(!shadowLights.empty())
    {
        for(unsigned int i = 0; i < shadowLights.size(); ++i)
        {
            int mask = 1 << i;
            shadowLights[i]->setShadowOcclusionMask(mask);
        }

        calculateShadowCameraViewProjections(shadowLights, camera);
        createLightShadowPasses(shadowRenderItems);
        shadowOcllusionRenderPass.shaderPasses[0].shaderParameterBlocks[0]->setParameterData(shadowOcclusionData.getData(), shadowOcclusionData.getSizeInBytes());
    }
}

void ShadowStage::clearStage()
{
    shadowRenderItems.clear();
    shadowLights.clear();
    renderPasses.clear();
    shadowDepthData.clear();
    shadowOcclusionData.clear();
    shadowOcllusionRenderPass.shaderPasses[0].shaderParameterBlocks[0]->clearParameters();
}

void ShadowStage::calculateShadowCameraViewProjections(const Vector<Light*>& lights, Camera* camera)
{
    Vector<Light*> pointLights;
    Vector<Light*> spotLights;
    Vector<Light*> directionalLights;

    lights.findItems([](const Light* light) {return light->getLightType() == LightType::Point;}, pointLights);
    lights.findItems([](const Light* light) {return light->getLightType() == LightType::Spot; }, spotLights);
    lights.findItems([](const Light* light) {return light->getLightType() == LightType::Directional; }, directionalLights);

    ShadowInputParameters parameters(camera);    
	
    if(!directionalLights.empty())
        shadowProjector.calculateDirectionalLightShadowViewProjections(directionalLights, parameters, shadowDepthData, shadowOcclusionData);
    if(!spotLights.empty())
        shadowProjector.calculateSpotLightShadowViewProjections(spotLights, parameters, shadowDepthData, shadowOcclusionData);
    if(!pointLights.empty())
        shadowProjector.calculatePointLightShadowViewProjections(pointLights, parameters, shadowDepthData, shadowOcclusionData);

}

void ShadowStage::createLightShadowPasses(const Vector<RenderItem>& renderItems)
{
    Frustum shadowFrustum;
    
    for(unsigned int i = 0; i < shadowDepthData.size(); ++i)
    {
        for(int j = 0; j < shadowDepthData[i].numSplits; ++j)
        {
            //Cull the items which are in the shadow light's frustum.
            itemsInShadowfrustum.clear();
            shadowFrustum.set(shadowDepthData[i].shadowViewProjectionMatrices[j].transpose());
            cullRenderItems(renderItems, itemsInShadowfrustum, shadowFrustum);

            shadowDepthRenderPass.renderTargetLayer = j;
            renderPasses.pushBack(shadowDepthRenderPass);
            renderPasses.back().shaderPasses.clear();
            ShaderPass depthShaderPass;
            for(unsigned int k = 0; k < itemsInShadowfrustum.size(); ++k)
            {
                if(!itemsInShadowfrustum[k].material->isSkinned())
                    depthShaderPass = shadowDepthRenderPass.shaderPasses[0];
                else
                    depthShaderPass = shadowDepthRenderPass.shaderPasses[1];

                depthShaderPass.vertexData = itemsInShadowfrustum[k].geometry->getVertexData();
                depthShaderPass.shaderParameters.pushBack(ShaderParameter(sp_worldTransform, itemsInShadowfrustum[k].geometry->getWorldTransform()));
                depthShaderPass.shaderParameters.pushBack(ShaderParameter(sp_lightViewProjectionMatrix, shadowDepthData[i].shadowViewProjectionMatrices[j]));
                renderPasses.back().shaderPasses.pushBack(depthShaderPass);
            }
        }

        //Construct the shadow occlusion pass.
        shadowOcllusionRenderPass.flags = (i == 0) ? CLEAR_COLOR : 0;
        shadowOcllusionRenderPass.shaderPasses[0].shaderParameters[0] = ShaderParameter(sp_shadowOcclusionParameterIndex, i);
        renderPasses.pushBack(shadowOcllusionRenderPass);
    }
}

}