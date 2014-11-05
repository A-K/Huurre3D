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

void ShadowStage::init()
{
    ViewPort screenViewPort = renderer->getScreenViewPort();

    //Create the render target, shader program and viewport for shadow depth pass.
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();
    Shader* shadowDepthVert = graphicSystem->createShader(ShaderType::Vertex, Engine::getShaderPath() + std::string("ShadowDepth.vert"));
    Shader* shadowDepthFrag = graphicSystem->createShader(ShaderType::Fragment, Engine::getShaderPath() + std::string("ShadowDepth.frag"));
    shadowDepthProgram = graphicSystem->createShaderProgram(shadowDepthVert, shadowDepthFrag);

    shadowDepthRenderTarget = graphicSystem->createRenderTarget(ShadowMapSize, ShadowMapSize, false, 1, NumCubeMapFaces);
    Texture* shadowDepth = graphicSystem->createTexture(TextureTargetMode::Texture2DArray, TextureWrapMode::ClampEdge, TextureFilterMode::Bilinear, TexturePixelFormat::Depth24, ShadowMapSize, ShadowMapSize);
    shadowDepth->setDepth(NumCubeMapFaces);
    shadowDepth->setSlotIndex(TextureSlotIndex::ShadowDepth);
    shadowDepthRenderTarget->setDepthBuffer(shadowDepth);
    shadowDepthViewPort.set(0, 0, ShadowMapSize, ShadowMapSize);

    //Create the shadow occlusion pass.
    shadowOcclusionRenderTarget = graphicSystem->createRenderTarget(screenViewPort.width, screenViewPort.height, false);
    Texture* shadowOcclusionTexture = graphicSystem->createTexture(TextureTargetMode::Texture2D, TextureWrapMode::ClampEdge, TextureFilterMode::Bilinear, TexturePixelFormat::Rgba8, screenViewPort.width, screenViewPort.height);
    shadowOcclusionTexture->setSlotIndex(TextureSlotIndex::ShadowOcclusion);
    shadowOcclusionRenderTarget->setColorBuffer(shadowOcclusionTexture);

    shadowOcllusionShaderPass.rasterState = RasterState(BlendMode::Add, CompareMode::Never, CullMode::None);
    Shader* shadowOcclusionVert = graphicSystem->createShader(ShaderType::Vertex, Engine::getShaderPath() + std::string("FullScreenQuad.vert"));
    Shader* shadowOcclusionFrag = graphicSystem->createShader(ShaderType::Fragment, Engine::getShaderPath() + std::string("ShadowOcclusion.frag"));
    shadowOcclusionVert->setDefine(ShaderDefineType::UseWorldSpaceParameters);
    shadowOcclusionFrag->setDefine(ShaderDefineType::MaxNumShadowLights, MaxNumShadowLights);
    shadowOcllusionShaderPass.program = graphicSystem->createShaderProgram(shadowOcclusionVert, shadowOcclusionFrag);

    shadowOcllusionShaderPass.vertexData = renderer->getFullScreenQuad();

    shadowOcllusionShaderPass.shaderParameterBlocks.pushBack(graphicSystem->createShaderParameterBlock(sp_shadowOcclusionParameters));
    shadowOcllusionShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_cameraParameters));
    shadowOcllusionShaderPass.shaderParameters.pushBack(ShaderParameter(sp_shadowOcclusionParameterIndex, 0));
}

void ShadowStage::resizeResources()
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    Texture* shadowOcclusionTexture = shadowOcclusionRenderTarget->getColorBuffers()[0];
    shadowOcclusionTexture->setWidth(screenViewPort.width);
    shadowOcclusionTexture->setHeight(screenViewPort.height);
}

void ShadowStage::setData(const Vector<RenderItem>& renderItems, const Vector<Light*>& lights, Camera* camera)
{
    if(!lights.empty())
    {
        calculateShadowCameraViewProjections(lights, camera);
        createLightShadowPasses(renderItems);
        shadowOcllusionShaderPass.shaderParameterBlocks[0]->clearBuffer();
        shadowOcllusionShaderPass.shaderParameterBlocks[0]->setParameterData(shadowOcclusionData.getData(), shadowOcclusionData.getSizeInBytes());
    }
}

void ShadowStage::clearStage()
{
    renderPasses.clear();
    shadowDepthData.clear();
    shadowOcclusionData.clear();
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
    ViewPort screenViewPort = renderer->getScreenViewPort();
    Frustum shadowFrustum;
    SceneCuller sceneCuller;

    for(unsigned int i = 0; i < shadowDepthData.size(); ++i)
    {
        for(int j = 0; j < shadowDepthData[i].numSplits; ++j)
        {
            //Cull the items which are in the shadow light's frustum.
            itemsInShadowfrustum.clear();
            shadowFrustum.set(shadowDepthData[i].shadowViewProjectionMatrices[j].transpose());
            sceneCuller.cullRenderItems(renderItems, itemsInShadowfrustum, shadowFrustum);

            //Construct the shadow depth pass with shaser pass for each render item.
            RenderPass depthPass;
            depthPass.renderTargetLayer = j;
            depthPass.flags = CLEAR_DEPTH;
            depthPass.colorWrite = false;
            depthPass.depthWrite = true;
            depthPass.viewPort = shadowDepthViewPort;
            depthPass.renderTarget = shadowDepthRenderTarget;
			 
            for(unsigned int k = 0; k < itemsInShadowfrustum.size(); ++k)
            {
                ShaderPass depthShaderPass;
                depthShaderPass.vertexData = itemsInShadowfrustum[k].geometry->getVertexData();
                depthShaderPass.shaderParameters.pushBack(ShaderParameter(sp_worldTransform, itemsInShadowfrustum[k].geometry->getWorldTransform()));
                depthShaderPass.shaderParameters.pushBack(ShaderParameter(sp_lightViewProjectionMatrix, shadowDepthData[i].shadowViewProjectionMatrices[j]));
                depthShaderPass.program = shadowDepthProgram;
                depthPass.shaderPasses.pushBack(depthShaderPass);
            }

            renderPasses.pushBack(depthPass);
        }

        //Construct the shadow occlusion pass.
        RenderPass shadowOcclusionPass;
        shadowOcclusionPass.colorWrite = true;
        shadowOcclusionPass.depthWrite = false;
        shadowOcclusionPass.renderTargetLayer = 0;
        shadowOcclusionPass.renderTarget = shadowOcclusionRenderTarget;
        shadowOcclusionPass.flags = (i==0) ? CLEAR_COLOR : 0;
        shadowOcclusionPass.viewPort = screenViewPort;
        shadowOcllusionShaderPass.shaderParameters[0] = ShaderParameter(sp_shadowOcclusionParameterIndex, i);
        shadowOcclusionPass.shaderPasses.pushBack(shadowOcllusionShaderPass);
        renderPasses.pushBack(shadowOcclusionPass);
    }
}

}