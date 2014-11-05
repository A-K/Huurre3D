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

#include "Renderer/RenderStage.h"
#include "Renderer/Renderer.h"
#include "Graphics/GraphicSystem.h"

namespace Huurre3D
{

RenderStage::RenderStage(Renderer* renderer):
renderer(renderer)
{
}

void RenderStage::drawRenderPasses(const Vector<RenderPass>& renderPasses) const
{
    GraphicSystem* graphicSystem = renderer->getGraphicSystem();

    for(unsigned int i = 0; i < renderPasses.size(); ++i)
    {
        RenderPass pass = renderPasses[i];

        if(pass.renderTarget)
        {
            pass.renderTarget->setRenderLayer(pass.renderTargetLayer); 
            graphicSystem->setOffLineRenderTarget(pass.renderTarget);
        }
        else
        {
            graphicSystem->setMainRenderTarget();
        }

        graphicSystem->setViewPort(pass.viewPort);
        graphicSystem->setDepthWrite(pass.depthWrite);
        graphicSystem->setColorWrite(pass.colorWrite);

        if(pass.flags != 0)
            graphicSystem->clear(pass.flags, pass.clearColor);

        for(unsigned int j = 0; j < renderPasses[i].shaderPasses.size(); ++j)
        {
            ShaderPass shaderPass = renderPasses[i].shaderPasses[j];
            graphicSystem->setVertexData(shaderPass.vertexData);
            graphicSystem->setRasterState(shaderPass.rasterState);
            graphicSystem->setShaderProgram(shaderPass.program);

            for(unsigned int k = 0; k < shaderPass.textures.size(); ++k)
                graphicSystem->setTexture(shaderPass.textures[k]);

            for(unsigned int n = 0; n < shaderPass.shaderParameterBlocks.size(); ++n)
                graphicSystem->setShaderParameterBlock(shaderPass.shaderParameterBlocks[n]);

            for(unsigned int m = 0; m < shaderPass.shaderParameters.size(); ++m)
                graphicSystem->setShaderParameter(shaderPass.shaderParameters[m]);

            shaderPass.vertexData->isIndexed() ? graphicSystem->drawIndexed(shaderPass.vertexData->getIndexBuffer()->getNumIndices(), 0) :
                graphicSystem->draw(shaderPass.vertexData->getNumVertices(), 0);
        }
    }
}

}