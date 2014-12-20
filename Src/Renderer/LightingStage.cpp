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

#include "Renderer/LightingStage.h"
#include "Renderer/Renderer.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"
#include "Graphics/ShaderParameterBlock.h"

namespace Huurre3D
{

LightingStage::LightingStage(Renderer* renderer):
RenderStage(renderer)
{
}

void LightingStage::init(const LightingStageDescription& lightingStageDescription)
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();

    if(lightingStageDescription.hasSSAOPass)
    {
        //Create SSAO renderpasses: 1. Raw SSAO; 2. Horizontal blur; 3. Vertical blur.
        RenderTarget* SSAORenderTarget = graphicSystem->createRenderTarget(screenViewPort.width, screenViewPort.height, false, 1, 2);
        Texture* SSAOTexture = graphicSystem->createTexture(TextureTargetMode::Texture2DArray, TextureWrapMode::ClampEdge, TextureFilterMode::Bilinear, TexturePixelFormat::Red16F, screenViewPort.width, screenViewPort.height);
        SSAOTexture->setDepth(2);
        SSAOTexture->setSlotIndex(TextureSlotIndex::SSAO);
        SSAORenderTarget->setColorBuffer(SSAOTexture);

        RenderPass SSAORenderPass;
        SSAORenderPass.colorWrite = true;
        SSAORenderPass.depthWrite = false;
        SSAORenderPass.flags = CLEAR_COLOR;
        SSAORenderPass.renderTargetLayer = 0;
        SSAORenderPass.renderTarget = SSAORenderTarget;
        SSAORenderPass.viewPort = screenViewPort;

        ShaderPass SSAOPass;
        Shader* SSAOVert = graphicSystem->createShader(ShaderType::Vertex, lightingStageDescription.ssaoPass.vertexShader);
        Shader* SSAOFrag = graphicSystem->createShader(ShaderType::Fragment, lightingStageDescription.ssaoPass.SSAOFragmentShader);
        SSAOFrag->setDefine(ShaderDefineType::SAO);
        SSAOFrag->setDefine(ShaderDefineType::NumSAOSamples, lightingStageDescription.ssaoPass.numSAOSamples);
        SSAOFrag->setDefine(ShaderDefineType::NumSAOSpiralTurns, lightingStageDescription.ssaoPass.numSAOSpiralTurns);
        SSAOPass.program = graphicSystem->createShaderProgram(SSAOVert, SSAOFrag);
        SSAOPass.vertexData = renderer->getFullScreenQuad();

        ShaderParameterBlock* SSAOParametersBlock = graphicSystem->createShaderParameterBlock(sp_SSAOParameters);
        Vector4 ssaoParameters = Vector4(500.0f, lightingStageDescription.ssaoPass.ssaoRadius, lightingStageDescription.ssaoPass.ssaoBias, lightingStageDescription.ssaoPass.ssaoIntensity);
        SSAOParametersBlock->addParameter(ssaoParameters);

        SSAOPass.shaderParameterBlocks.pushBack(SSAOParametersBlock);
        SSAOPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_renderTargetSize));
        SSAOPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_cameraParameters));

        SSAORenderPass.shaderPasses.pushBack(SSAOPass);

        //Horizontal blur
        RenderPass verticalBlurRenderPass;
        verticalBlurRenderPass.colorWrite = true;
        verticalBlurRenderPass.depthWrite = false;
        verticalBlurRenderPass.flags = 0;
        verticalBlurRenderPass.renderTargetLayer = 1;
        verticalBlurRenderPass.renderTarget = SSAORenderTarget;
        verticalBlurRenderPass.viewPort = screenViewPort;

        ShaderPass verticalBlurShaderPass;
        Shader* verticalBlurVert = graphicSystem->createShader(ShaderType::Vertex, lightingStageDescription.ssaoPass.vertexShader);
        Shader* verticalBlurFrag = graphicSystem->createShader(ShaderType::Fragment, lightingStageDescription.ssaoPass.SSAOBlurFragmentShader);
        verticalBlurShaderPass.program = graphicSystem->createShaderProgram(verticalBlurVert, verticalBlurFrag);
        verticalBlurShaderPass.vertexData = renderer->getFullScreenQuad();
        verticalBlurShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_renderTargetSize));
        verticalBlurShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_cameraParameters));

        verticalBlurRenderPass.shaderPasses.pushBack(verticalBlurShaderPass);

        //Vertical blur
        RenderPass horizontalBlurRenderPass;
        horizontalBlurRenderPass.colorWrite = true;
        horizontalBlurRenderPass.depthWrite = false;
        horizontalBlurRenderPass.flags = 0;
        horizontalBlurRenderPass.renderTargetLayer = 0;
        horizontalBlurRenderPass.renderTarget = SSAORenderTarget;
        horizontalBlurRenderPass.viewPort = screenViewPort;

        ShaderPass horizontalBlurShaderPass;
        Shader* horizontalBlurVert = graphicSystem->createShader(ShaderType::Vertex, lightingStageDescription.ssaoPass.vertexShader);
        Shader* horizontalBlurFrag = graphicSystem->createShader(ShaderType::Fragment, lightingStageDescription.ssaoPass.SSAOBlurFragmentShader);
        horizontalBlurFrag->setDefine(ShaderDefineType::VerticalBlur);
        horizontalBlurShaderPass.program = graphicSystem->createShaderProgram(horizontalBlurVert, horizontalBlurFrag);
        horizontalBlurShaderPass.vertexData = renderer->getFullScreenQuad();
        horizontalBlurShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_renderTargetSize));
        horizontalBlurShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_cameraParameters));

        horizontalBlurRenderPass.shaderPasses.pushBack(horizontalBlurShaderPass);

        renderPasses.pushBack(SSAORenderPass);
        renderPasses.pushBack(verticalBlurRenderPass);
        renderPasses.pushBack(horizontalBlurRenderPass);
    }

    tileGrid.setGridDimensions(lightingStageDescription.lightingPass.lightTileWidth, lightingStageDescription.lightingPass.lightTileHeight, screenViewPort.width, screenViewPort.height);

    RenderTarget* lightingRenderTarget = graphicSystem->createRenderTarget(screenViewPort.width, screenViewPort.height, false);
    Texture* lightingTexture = graphicSystem->createTexture(TextureTargetMode::Texture2D, TextureWrapMode::ClampEdge, TextureFilterMode::Bilinear, TexturePixelFormat::Rgba8, screenViewPort.width, screenViewPort.height);
    lightingTexture->setSlotIndex(TextureSlotIndex::Lighting);
    lightingRenderTarget->setColorBuffer(lightingTexture);
    lightingRenderTarget->setName("lighting");

    RenderPass lightingRenderPass;
    lightingRenderPass.colorWrite = true;
    lightingRenderPass.depthWrite = false;
    lightingRenderPass.flags = CLEAR_COLOR;
    lightingRenderPass.renderTargetLayer = 0;
    lightingRenderPass.renderTarget = lightingRenderTarget;
    lightingRenderPass.viewPort = screenViewPort;

    Shader* tiledDeferredVert = graphicSystem->createShader(ShaderType::Vertex, lightingStageDescription.lightingPass.vertexShader);
    Shader* tiledDeferredFrag = graphicSystem->createShader(ShaderType::Fragment, lightingStageDescription.lightingPass.fragmentShader);
    tiledDeferredFrag->setDefine(ShaderDefineType::MaxNumLights, MaXNumLights);
    tiledDeferredFrag->setDefine(ShaderDefineType::MaxNumMaterials, MaxNumMaterials);
    
    if(lightingStageDescription.hasSSAOPass)
        tiledDeferredFrag->setDefine(ShaderDefineType::SSAO);
    if(lightingStageDescription.lightingPass.hdr)
        tiledDeferredFrag->setDefine(ShaderDefineType::Hdr);

    Texture* tileLightTexture = graphicSystem->createTexture(TextureTargetMode::Texture2D, TextureWrapMode::ClampEdge, TextureFilterMode::Nearest, TexturePixelFormat::Red32I, 0, 0);
    tileLightTexture->setSlotIndex(TextureSlotIndex::TileLightInfo);

    ShaderPass tiledDeferredPass;
    tiledDeferredPass.program = graphicSystem->createShaderProgram(tiledDeferredVert, tiledDeferredFrag);
    tiledDeferredPass.vertexData = renderer->getFullScreenQuad();
    tiledDeferredPass.textures.pushBack(tileLightTexture);

    auto gridDimensions = tileGrid.getGridDimensions();
    ShaderParameterBlock* lightGridParametersBlock = graphicSystem->createShaderParameterBlock(sp_lightGridParameters);
    lightGridParametersBlock->setParameterData(&gridDimensions, sizeof(gridDimensions));

    tiledDeferredPass.shaderParameterBlocks.pushBack(graphicSystem->createShaderParameterBlock(sp_lightParameters));
    tiledDeferredPass.shaderParameterBlocks.pushBack(lightGridParametersBlock);
    tiledDeferredPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_materialProperties));
    tiledDeferredPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_renderTargetSize));
    tiledDeferredPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_cameraParameters));

    lightingRenderPass.shaderPasses.pushBack(tiledDeferredPass);
    renderPasses.pushBack(lightingRenderPass);
}

void LightingStage::resizeResources()
{
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();
    ViewPort screenViewPort = renderer->getScreenViewPort();
    int lightingRenderPassIndex = (renderPasses.size() > 1) ? 3 : 0;

    for(unsigned int i = 0; i < renderPasses.size(); ++i)
        renderPasses[i].viewPort = screenViewPort;

    if(lightingRenderPassIndex == 3)
    {
        //Resize the SSAO render target texture.
        Texture* SSAOTexture = renderPasses[0].renderTarget->getColorBuffers()[0];
        SSAOTexture->setWidth(screenViewPort.width);
        SSAOTexture->setHeight(screenViewPort.height);
    }

    //Resize lighting pass render target texture.
    Texture* lightingTexture = renderPasses[lightingRenderPassIndex].renderTarget->getColorBuffers()[0];
    lightingTexture->setWidth(screenViewPort.width);
    lightingTexture->setHeight(screenViewPort.height);

    //Resize the light grid.
    auto gridDimensions = tileGrid.getGridDimensions();
    tileGrid.setGridDimensions(gridDimensions.tileWidth, gridDimensions.tileHeight, screenViewPort.width, screenViewPort.height);

    //Resize the light grid dimensions shader parameter block.
    auto newGridDimensions = tileGrid.getGridDimensions();
    ShaderParameterBlock* lightGridParametersBlock = graphicSystem->getShaderParameterBlockByName(sp_lightGridParameters);
    lightGridParametersBlock->setParameterData(&newGridDimensions, sizeof(newGridDimensions));
}

void LightingStage::update(const Scene* scene)
{
    int lightingRenderPassIndex = (renderPasses.size() > 1) ? 3 : 0;

    Camera* camera = scene->getMainCamera();
    Frustum worldSpaceCameraViewFrustum = camera->getViewFrustumInWorldSpace();
    cullLights(scene, frustumLights, worldSpaceCameraViewFrustum);
    Vector3 globalAmbientLight = scene->getGlobalAmbientLight();

    //Bin lights to tiles.
    tileGrid.binLightsToTiles(frustumLights, camera);
    
    auto gridDimensions = tileGrid.getGridDimensions();
    auto lightParameterBlock = tileGrid.getLightParameterBlockValues();

    //Set the global ambient light into the parameter
    lightParameterBlock.numLightsAndGlobalAmbient.y = globalAmbientLight.x;
    lightParameterBlock.numLightsAndGlobalAmbient.z = globalAmbientLight.y;
    lightParameterBlock.numLightsAndGlobalAmbient.w = globalAmbientLight.z;

    //Update the lights parameter block in Tiled deferred shader pass.
    renderPasses[lightingRenderPassIndex].shaderPasses[0].shaderParameterBlocks[0]->clearBuffer();
    renderPasses[lightingRenderPassIndex].shaderPasses[0].shaderParameterBlocks[0]->setParameterData(&lightParameterBlock, sizeof(lightParameterBlock));

    //Update the light info texture in Tiled deferred shader pass.
    int width = frustumLights.size();;
    int height = gridDimensions.widthResolution * gridDimensions.heightResolution;
    int size = sizeof(int) * width * height;
    renderPasses[lightingRenderPassIndex].shaderPasses[0].textures[0]->setWidth(width);
    renderPasses[lightingRenderPassIndex].shaderPasses[0].textures[0]->setHeight(height);
    renderPasses[lightingRenderPassIndex].shaderPasses[0].textures[0]->setPixelData(tileGrid.getTileLightInfo().data(), size);
}

void LightingStage::clearStage()
{
    frustumLights.clear();
}

}