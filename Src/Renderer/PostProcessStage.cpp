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
#include "Renderer/PostProcessStage.h"
#include "Renderer/Renderer.h"

namespace Huurre3D
{

PostProcessStage::PostProcessStage(Renderer* renderer):
RenderStage(renderer)
{
}

void PostProcessStage::init()
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();

    //Create environment pass.
    RenderPass environmentPass;
    environmentPass.colorWrite = true;
    environmentPass.depthWrite = false;
    environmentPass.flags = 0;
    environmentPass.renderTargetLayer = 0;
    environmentPass.renderTarget = graphicSystem->getRenderTargetByName("lighting");
    environmentPass.viewPort = screenViewPort;

    ShaderPass environmentShaderPass;
    environmentShaderPass.rasterState = RasterState(BlendMode::Add, CompareMode::Never, CullMode::None);

    Shader* environmentVert = graphicSystem->createShader(ShaderType::Vertex, Engine::getShaderPath() + std::string("FullScreenQuad.vert"));
    Shader* environmentFrag = graphicSystem->createShader(ShaderType::Fragment, Engine::getShaderPath() + std::string("Environment.frag"));
    environmentVert->setDefine(ShaderDefineType::UseWorldSpaceParameters);
    environmentShaderPass.program = graphicSystem->createShaderProgram(environmentVert, environmentFrag);
    environmentShaderPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_cameraParameters));
    environmentShaderPass.vertexData = renderer->getFullScreenQuad();

    environmentPass.shaderPasses.pushBack(environmentShaderPass);
    renderPasses.pushBack(environmentPass);

    //Create the post process pass.
    RenderPass postProcessRenderPass;
    postProcessRenderPass.colorWrite = true;
    postProcessRenderPass.depthWrite = false;
    postProcessRenderPass.flags = CLEAR_COLOR;
    postProcessRenderPass.renderTargetLayer = 0;
    postProcessRenderPass.renderTarget = nullptr;
    postProcessRenderPass.viewPort = screenViewPort;
	
    ShaderPass postProcessPass;
    postProcessPass.rasterState = RasterState(BlendMode::Off, CompareMode::Never, CullMode::None);

    Shader* postProcessVert = graphicSystem->createShader(ShaderType::Vertex, Engine::getShaderPath() + std::string("FullScreenQuad.vert"));
    Shader* postProcessFrag = graphicSystem->createShader(ShaderType::Fragment, Engine::getShaderPath() + std::string("PostProcess.frag"));
    postProcessFrag->setDefine(ShaderDefineType::FxaaQuality, 12);
    //postProcessFrag->setDefine(ShaderDefineType::SAO);
    //postProcessFrag->setDefine(ShaderDefineType::NumSAOSamples, NumSAOSamples);
    //postProcessFrag->setDefine(ShaderDefineType::NumSAOSpiralTurns, NumSAOSpiralTurns);
    postProcessPass.program = graphicSystem->createShaderProgram(postProcessVert, postProcessFrag);
    postProcessPass.vertexData = renderer->getFullScreenQuad();

    PostProcessParameterValue postProcessParameterValue;
    postProcessParameterValue.SAOParameters = Vector4(500.0f, 0.5f, 0.016f, 1.0f);
    postProcessParameterValue.renderTargetSize = Vector2(static_cast<float>(screenViewPort.width), static_cast<float>(screenViewPort.height));
    postProcessPass.shaderParameterBlocks.pushBack(graphicSystem->createShaderParameterBlock(sp_PostProcessParameters));
    postProcessPass.shaderParameterBlocks[0]->setParameterData(&postProcessParameterValue, sizeof(postProcessParameterValue));
    postProcessPass.shaderParameterBlocks.pushBack(graphicSystem->getShaderParameterBlockByName(sp_cameraParameters));

    postProcessRenderPass.shaderPasses.pushBack(postProcessPass);
    renderPasses.pushBack(postProcessRenderPass);
}

void PostProcessStage::resizeResources()
{
    ViewPort screenViewPort = renderer->getScreenViewPort();
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();

    //Resize the environment pass.
    renderPasses[0].renderTarget = graphicSystem->getRenderTargetByName("lighting");
    renderPasses[0].viewPort = screenViewPort;

    //Resize the post process pass
    renderPasses[1].viewPort = screenViewPort;

    //Resize the post process shader parameter block.
    PostProcessParameterValue postProcessParameterValue;
    postProcessParameterValue.SAOParameters = Vector4(500.0f, 0.5f, 0.016f, 1.0f);
    postProcessParameterValue.renderTargetSize = Vector2(static_cast<float>(screenViewPort.width), static_cast<float>(screenViewPort.height));
    renderPasses[1].shaderPasses[0].shaderParameterBlocks[0]->setParameterData(&postProcessParameterValue, sizeof(postProcessParameterValue));
}

void PostProcessStage::setSkybox(SkyBox* skyBox)
{
    if(currentSkyBox != skyBox)
    {
        Texture* skyBoxTexture = skyBox->getTexture();
        if(!skyBoxTexture)
        {
            FixedArray<bool, 6> flipImages;
            flipImages.fill(false);
            TextureLoader loader = renderer->getTextureLoader();
            TextureLoadResult result = loader.loadCubeMapFromFile(skyBox->getTextureFiles(), flipImages);

            if(result.pixelData[0] && result.pixelData[1] && result.pixelData[2] && result.pixelData[3] && result.pixelData[4] && result.pixelData[5])
            {
                skyBoxTexture = renderer->getGraphicSystem()->createTexture(TextureTargetMode::TextureCubeMap, TextureWrapMode::ClampEdge, TextureFilterMode::Bilinear, result.format, result.width, result.height);
                skyBoxTexture->setData(result);
            }

            loader.releasedata(result);
        }

        if(skyBoxTexture)
        {
            renderPasses[0].shaderPasses[0].textures.clear();
            renderPasses[0].shaderPasses[0].textures.pushBack(skyBoxTexture);
            skyBoxTexture->setSlotIndex(TextureSlotIndex::SkyBox);
            skyBox->setTexture(skyBoxTexture);
            currentSkyBox = skyBox;
        }
    }
}

}
