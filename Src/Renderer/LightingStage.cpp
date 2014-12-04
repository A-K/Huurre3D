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

void LightingStage::init(int tileWidth, int tileHeight, bool ssao, bool hdr)
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();

    if(ssao)
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
        Shader* SSAOVert = graphicSystem->createShader(ShaderType::Vertex, Engine::getShaderPath() + std::string("FullScreenQuad.vert"));
        Shader* SSAOFrag = graphicSystem->createShader(ShaderType::Fragment, Engine::getShaderPath() + std::string("SSAO.frag"));
        SSAOFrag->setDefine(ShaderDefineType::SAO);
        SSAOFrag->setDefine(ShaderDefineType::NumSAOSamples, NumSAOSamples);
        SSAOFrag->setDefine(ShaderDefineType::NumSAOSpiralTurns, NumSAOSpiralTurns);
        SSAOPass.program = graphicSystem->createShaderProgram(SSAOVert, SSAOFrag);
        SSAOPass.vertexData = renderer->getFullScreenQuad();

        SSAOParameterValue SSAOValue;
        SSAOValue.renderTargetSize = Vector2(static_cast<float>(screenViewPort.width), static_cast<float>(screenViewPort.height));

        SSAOPass.shaderParameterBlocks.pushBack(graphicSystem->createShaderParameterBlock(sp_SSAOParameters));
        SSAOPass.shaderParameterBlocks[0]->setParameterData(&SSAOValue, sizeof(SSAOValue));
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
        Shader* verticalBlurVert = graphicSystem->createShader(ShaderType::Vertex, Engine::getShaderPath() + std::string("FullScreenQuad.vert"));
        Shader* verticalBlurFrag = graphicSystem->createShader(ShaderType::Fragment, Engine::getShaderPath() + std::string("SSAOBlur.frag"));
        verticalBlurShaderPass.program = graphicSystem->createShaderProgram(verticalBlurVert, verticalBlurFrag);
        verticalBlurShaderPass.vertexData = renderer->getFullScreenQuad();
        verticalBlurShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_SSAOParameters));
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
        Shader* horizontalBlurVert = graphicSystem->createShader(ShaderType::Vertex, Engine::getShaderPath() + std::string("FullScreenQuad.vert"));
        Shader* horizontalBlurFrag = graphicSystem->createShader(ShaderType::Fragment, Engine::getShaderPath() + std::string("SSAOBlur.frag"));
        horizontalBlurFrag->setDefine(ShaderDefineType::VerticalBlur);
        horizontalBlurShaderPass.program = graphicSystem->createShaderProgram(horizontalBlurVert, horizontalBlurFrag);
        horizontalBlurShaderPass.vertexData = renderer->getFullScreenQuad();
        horizontalBlurShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_SSAOParameters));
        horizontalBlurShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_cameraParameters));

        horizontalBlurRenderPass.shaderPasses.pushBack(horizontalBlurShaderPass);

        renderPasses.pushBack(SSAORenderPass);
        renderPasses.pushBack(verticalBlurRenderPass);
        renderPasses.pushBack(horizontalBlurRenderPass);
    }

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

    tileGrid.setGridDimensions(tileWidth, tileHeight, screenViewPort.width, screenViewPort.height);
    
    //Create the tiled lighting shader pass.
    auto gridDimensions = tileGrid.getGridDimensions();

    Shader* tiledDeferredVert = graphicSystem->createShader(ShaderType::Vertex, Engine::getShaderPath() + "FullScreenQuad.vert");
    Shader* tiledDeferredFrag = graphicSystem->createShader(ShaderType::Fragment, Engine::getShaderPath() + "TiledDeferredShading.frag");
    tiledDeferredFrag->setDefine(ShaderDefineType::MaxNumLights, MaXNumLights);
    tiledDeferredFrag->setDefine(ShaderDefineType::MaxNumMaterials, MaxNumMaterials);
    
    if(ssao)
        tiledDeferredFrag->setDefine(ShaderDefineType::SSAO);
    if(hdr)
        tiledDeferredFrag->setDefine(ShaderDefineType::Hdr);

    Texture* tileLightTexture = graphicSystem->createTexture(TextureTargetMode::Texture2D, TextureWrapMode::ClampEdge, TextureFilterMode::Nearest, TexturePixelFormat::Red32I, 0, 0);
    tileLightTexture->setSlotIndex(TextureSlotIndex::TileLightInfo);

    ShaderPass tiledDeferredPass;
    tiledDeferredPass.program = graphicSystem->createShaderProgram(tiledDeferredVert, tiledDeferredFrag);
    tiledDeferredPass.vertexData = renderer->getFullScreenQuad();
    tiledDeferredPass.textures.pushBack(tileLightTexture);

    LightGridParameterValue lightGridParameterValue;
    lightGridParameterValue.gridDimensions[0] = gridDimensions.tileWidth;
    lightGridParameterValue.gridDimensions[1] = gridDimensions.tileHeight;
    lightGridParameterValue.gridDimensions[2] = gridDimensions.widthResolution;
    lightGridParameterValue.gridDimensions[3] = gridDimensions.heightResolution;
    lightGridParameterValue.renderTargetSize = Vector2(static_cast<float>(screenViewPort.width), static_cast<float>(screenViewPort.height));

    tiledDeferredPass.shaderParameterBlocks.pushBack(graphicSystem->createShaderParameterBlock(sp_lightParameters));
    tiledDeferredPass.shaderParameterBlocks.pushBack(graphicSystem->createShaderParameterBlock(sp_lightGridParameters));
    tiledDeferredPass.shaderParameterBlocks[1]->setParameterData(&lightGridParameterValue, sizeof(lightGridParameterValue));
    tiledDeferredPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_cameraParameters));
    tiledDeferredPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_materialProperties));

    lightingRenderPass.shaderPasses.pushBack(tiledDeferredPass);
    renderPasses.pushBack(lightingRenderPass);
}

void LightingStage::resizeResources()
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    int lightingRenderPassIndex = (renderPasses.size() > 1) ? 3 : 0;

    if(lightingRenderPassIndex == 3)
    {
        //Resize the SSAO render target texture.
        Texture* SSAOTexture = renderPasses[0].renderTarget->getColorBuffers()[0];
        SSAOTexture->setWidth(screenViewPort.width);
        SSAOTexture->setHeight(screenViewPort.height);

        //Resize the viewport of SSAO and both blur passes
        renderPasses[0].viewPort = screenViewPort;
        renderPasses[1].viewPort = screenViewPort;
        renderPasses[2].viewPort = screenViewPort;

        //Resize the SSAO shader parameter block.
        SSAOParameterValue SSAOValue;
        SSAOValue.renderTargetSize = Vector2(static_cast<float>(screenViewPort.width), static_cast<float>(screenViewPort.height));
        renderPasses[0].shaderPasses[0].shaderParameterBlocks[0]->setParameterData(&SSAOValue, sizeof(SSAOValue));
    }

    renderPasses[lightingRenderPassIndex].viewPort = screenViewPort;

    //Resize the render target texture.
    Texture* lightingTexture = renderPasses[lightingRenderPassIndex].renderTarget->getColorBuffers()[0];
    lightingTexture->setWidth(screenViewPort.width);
    lightingTexture->setHeight(screenViewPort.height);

    //Resize the light grid.
    auto currentDimensions = tileGrid.getGridDimensions();
    tileGrid.setGridDimensions(currentDimensions.tileWidth, currentDimensions.tileHeight, screenViewPort.width, screenViewPort.height);

    //Resize the light grid dimensions shader parameter block.
    auto newGridDimensions = tileGrid.getGridDimensions();
    LightGridParameterValue lightGridParameterValue;
    lightGridParameterValue.gridDimensions[0] = newGridDimensions.tileWidth;
    lightGridParameterValue.gridDimensions[1] = newGridDimensions.tileHeight;
    lightGridParameterValue.gridDimensions[2] = newGridDimensions.widthResolution;
    lightGridParameterValue.gridDimensions[3] = newGridDimensions.heightResolution;
    lightGridParameterValue.renderTargetSize = Vector2(static_cast<float>(screenViewPort.width), static_cast<float>(screenViewPort.height));
    renderPasses[lightingRenderPassIndex].shaderPasses[0].shaderParameterBlocks[1]->setParameterData(&lightGridParameterValue, sizeof(lightGridParameterValue));
}

void LightingStage::setData(const Vector<Light*>& lights, const Vector3& globalAmbientLight, Camera* camera)
{
    int lightingRenderPassIndex = (renderPasses.size() > 1) ? 3 : 0;

    //Bin lights to tiles.
    tileGrid.binLightsToTiles(lights, camera);
    
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
    int width = lights.size();;
    int height = gridDimensions.widthResolution * gridDimensions.heightResolution;
    int size = sizeof(int) * width * height;
    renderPasses[lightingRenderPassIndex].shaderPasses[0].textures[0]->setWidth(width);
    renderPasses[lightingRenderPassIndex].shaderPasses[0].textures[0]->setHeight(height);
    renderPasses[lightingRenderPassIndex].shaderPasses[0].textures[0]->setPixelData(tileGrid.getTileLightInfo().data(), size);
}

}