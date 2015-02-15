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

#ifndef GraphicSystemBackEnd_H
#define GraphicSystemBackEnd_H

#include "Graphics/Rasterization.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexData.h"
#include "Graphics/ShaderParameter.h"
#include "Graphics/ShaderProgram.h"
#include "Util/Vector.h"

namespace Huurre3D
{

class ShaderParameterBlock;
class RenderTarget;

class GraphicSystemBackEnd
{
public:
    GraphicSystemBackEnd(){}
    virtual ~GraphicSystemBackEnd() {}
	
    void setViewPort(const ViewPort& viewPort) {}
    void clear(unsigned int flags, const Vector4& color) {}
    unsigned int createVertexStream() {return graphicResourceId++;}
    unsigned int createIndexBuffer() {return graphicResourceId++;}
    unsigned int createShaderParameterBlock(const std::string& name)  { return graphicResourceId++; }
    unsigned int createVertexData() {return graphicResourceId++;}
    unsigned int createTexture() {return graphicResourceId++;}
    unsigned int createShaderProgram() {return graphicResourceId++;}
    unsigned int createRenderTarget(int width, int height, int numBuffers, int numLayers) { return graphicResourceId++; }
    void removeBuffer(unsigned int bufferId) {}
    void removeVertexData(unsigned int vertexDataId) {}
    void removeTexture(unsigned int textureId) {}
    void removeShaderProgram(unsigned int shaderProgramId) {}
    void removeRenderTarget(unsigned int renderTargetId) {}
    void setVertexData(VertexData *vertexData) {}
    void setShaderProgram(ShaderProgram* program) {} 
    void setShaderParameter(ShaderParameterDescription* description, const ShaderParameter& shaderParameter) {}
    void setShaderParameterBlock(ShaderParameterBlockDescription* description, ShaderParameterBlock* block) {}
    void setTexture(Texture* texture) {}
    void setDepthWrite(bool enable) {}
    void setColorWrite(bool enable) {}
    void setRasterState(const RasterState& state) {}
    void setOffLineRenderTarget(RenderTarget* renderTarget) {}
    void setMainRenderTarget() {}
    void draw(int numVertices, int vertexOffset) {} 
    void drawIndexed(int numIndices, int indexOffset) {}
    void drawInstanced(int numIndices, int indexOffset, int instancesCount) {}

private:
    unsigned int graphicResourceId = 0;
};

}

#endif