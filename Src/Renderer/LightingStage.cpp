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

void LightingStage::init(int tileWidth, int tileHeight)
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();

    RenderTarget* renderTarget = graphicSystem->createRenderTarget(screenViewPort.width, screenViewPort.height, false);
    Texture* lightingTexture = graphicSystem->createTexture(TextureTargetMode::Texture2D, TextureWrapMode::ClampEdge, TextureFilterMode::Bilinear, TexturePixelFormat::Rgba8, screenViewPort.width, screenViewPort.height);
    lightingTexture->setSlotIndex(TextureSlotIndex::Lighting);
    renderTarget->setColorBuffer(lightingTexture);
    renderTarget->setName("lighting");

    RenderPass renderPass;
    renderPass.colorWrite = true;
    renderPass.depthWrite = false;
    renderPass.flags = CLEAR_COLOR;
    renderPass.renderTargetLayer = 0;
    renderPass.renderTarget = renderTarget;
    renderPass.viewPort = screenViewPort;
    renderPasses.pushBack(renderPass);

    tileGrid.setGridDimensions(tileWidth, tileHeight, screenViewPort.width, screenViewPort.height);
    
    //Create the tiled lighting shader pass.
    auto gridDimensions = tileGrid.getGridDimensions();

    Shader* tiledDeferredVert = graphicSystem->createShader(ShaderType::Vertex, Engine::getShaderPath() + "FullScreenQuad.vert");
    Shader* tiledDeferredFrag = graphicSystem->createShader(ShaderType::Fragment, Engine::getShaderPath() + "TiledDeferredShading.frag");
    tiledDeferredFrag->setDefine(ShaderDefineType::MaxNumLights, MaXNumLights);
    tiledDeferredFrag->setDefine(ShaderDefineType::MaxNumMaterials, MaxNumMaterials);
    tiledDeferredFrag->setDefine(ShaderDefineType::Hdr);

    Texture* tileLightTexture = graphicSystem->createTexture(TextureTargetMode::Texture2D, TextureWrapMode::ClampEdge, TextureFilterMode::Nearest, TexturePixelFormat::Red32I, 0, 0);
    tileLightTexture->setSlotIndex(TextureSlotIndex::TileLightInfo);

    ShaderPass tiledDeferredPass;
    tiledDeferredPass.rasterState = RasterState(BlendMode::Off, CompareMode::Never, CullMode::None);
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
    renderPasses[0].shaderPasses.pushBack(tiledDeferredPass);
}

void LightingStage::resizeResources()
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    renderPasses[0].viewPort = screenViewPort;

    //Resize the render target texture.
    Texture* lightingTexture = renderPasses[0].renderTarget->getColorBuffers()[0];
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
    renderPasses[0].shaderPasses[0].shaderParameterBlocks[1]->setParameterData(&lightGridParameterValue, sizeof(lightGridParameterValue));
}

void LightingStage::setData(const Vector<Light*>& lights, const Vector3& globalAmbientLight, Camera* camera)
{
    //Bin lights to tiles.
    tileGrid.binLightsToTiles(lights, camera);
    
    auto gridDimensions = tileGrid.getGridDimensions();
    auto lightParameterBlock = tileGrid.getLightParameterBlockValues();

    //Set the global ambient light into the parameter
    lightParameterBlock.numLightsAndGlobalAmbient.y = globalAmbientLight.x;
    lightParameterBlock.numLightsAndGlobalAmbient.z = globalAmbientLight.y;
    lightParameterBlock.numLightsAndGlobalAmbient.w = globalAmbientLight.z;

    //Update the lights parameter block in Tiled deferred shader pass.
    renderPasses[0].shaderPasses[0].shaderParameterBlocks[0]->clearBuffer();
    renderPasses[0].shaderPasses[0].shaderParameterBlocks[0]->setParameterData(&lightParameterBlock, sizeof(lightParameterBlock));

    //Update the light info texture in Tiled deferred shader pass.
    int width = lights.size();;
    int height = gridDimensions.widthResolution * gridDimensions.heightResolution;
    int size = sizeof(int) * width * height;
    renderPasses[0].shaderPasses[0].textures[0]->setWidth(width);
    renderPasses[0].shaderPasses[0].textures[0]->setHeight(height);
    renderPasses[0].shaderPasses[0].textures[0]->setPixelData(tileGrid.getTileLightInfo().data(), size);
}

}