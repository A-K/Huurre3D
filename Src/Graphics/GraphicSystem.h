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

#ifndef GraphicSystem_H
#define GraphicSystem_H

#include "Graphics/Texture.h"
#include "Graphics/VertexData.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/ShaderParameterBlock.h"
#include "Graphics/ShaderParameters.h"
#include "Graphics/Rasterization.h"
#include "Graphics/RenderTarget.h"
#include "Math/Rect.h"

#ifdef USE_OGL
#include "Graphics/OGLGraphicsBackEnd/OGLGraphicSystemBackEnd.h"
#define GraphicSystemBackEnd OGLGraphicSystemBackEnd
#endif

namespace Huurre3D
{

class GraphicSystem
{
public:
    GraphicSystem();
    ~GraphicSystem();

    VertexData* createVertexData(PrimitiveType primitiveType, int numVertices);
    AttributeBuffer* createAttributeBuffer(AttributeType type, AttributeSemantic semantic, int numComponentsPerVertex, bool normalized, bool dynamic);
    VertexStream* createVertexStream(int numVertices, bool interleaved);
    IndexBuffer* createIndexBuffer(IndexType indexType, int numIndices, bool dynamic);
    Shader* createShader(ShaderType shaderType, const std::string& sourceFileName);
    ShaderProgram* createShaderProgram(Shader* vertexShader, Shader* fragmentShader);
    Texture* createTexture(TextureTargetMode targetMode, TextureWrapMode wrapMode, TextureFilterMode filterMode, TexturePixelFormat pixelFormat, int width, int height);
    RenderTarget* createRenderTarget(int width, int height, bool depthBuffer, int numBuffers = 1, int numLayers = 1);
    ShaderParameterBlock* createShaderParameterBlock(const std::string& name);
    void setVertexData(VertexData* vertexData);
    void setShaderProgram(ShaderProgram* program);
    void setShaderParameter(const ShaderParameter& shaderParameter);
    void setShaderParameterBlock(ShaderParameterBlock* block);
    void setTexture(Texture* texture);
    void setDepthWrite(bool enable);
    void setColorWrite(bool enable);
    void setOffLineRenderTarget(RenderTarget* renderTarget);
    void setRasterState(const RasterState& state);
    void setMainRenderTarget();
    void setViewPort(const ViewPort& viewPort);
    void clear(unsigned int flags, const Vector4& color);
    void draw(int numVertices, int vertexOffset);
    void drawIndexed(int numIndices, int indexOffset);
    void drawInstanced(int numIndices, int indexOffset, int instancesCount);
    void removeVertexData(VertexData* vertexData);
    void removeAttributeBuffer(AttributeBuffer* attributeBuffer);
    void removeVertexStream(VertexStream* vertexStream);
    void removeIndexBuffer(IndexBuffer* indexBuffer);
    void removeShaderProgram(ShaderProgram* program);
    void removeShader(Shader* shader);
    void removeTexture(Texture* texture);
    void removeRenderTarget(RenderTarget* renderTarget);
    void removeShaderParameterBlock(ShaderParameterBlock* shaderParameterBlock);
    void clearVertexDatas();
    void clearAttributeBuffers();
    void clearVertexStreams();
    void clearIndexBuffers();
    void clearShaderPrograms();
    void clearShaders();
    void clearTextures();
    void clearRenderTargets();
    void clearShaderParameterBlocks();
    ShaderProgram* getShaderCombination(unsigned int shaderCombinationTag);
    ShaderProgram* getShaderCombination(const Vector<std::string>& shaderFileNames, const Vector<ShaderDefine>& shaderDefines);
    Texture* getTextureBySlotIndex(TextureSlotIndex index);
    ShaderParameterBlock* getShaderParameterBlockByName(const std::string& name);
    RenderTarget* getRenderTargetByName(const std::string& name);

private:
    unsigned int generateShaderCombinationTag(const Vector<Shader*>& shaders);
    unsigned int generateShaderCombinationTag(const Vector<std::string>& shaderFileNames, const Vector<ShaderDefine>& shaderDefines);
    
    GraphicSystemBackEnd* graphicSystemBackEnd;
    Vector<AttributeBuffer*> attributeBuffers;
    Vector<VertexStream*> vertexStreams;
    Vector<IndexBuffer*> indexBuffers;
    Vector<VertexData*> vertexDataComponents;
    Vector<ShaderProgram*> shaderPrograms;
    Vector<Shader*> shaders;
    Vector<Texture*> textures;
    Vector<RenderTarget*> renderTargets;
    Vector<ShaderParameterBlock*> shaderParameterBlocks;

    VertexData* currentVertexData = nullptr;
    ShaderProgram* currentShaderProgram = nullptr;
    ShaderParameterBlock* currentShaderParameterBlock = nullptr;
    ViewPort currentViewPort;
    RasterState currentRasterState;
    bool depthWriteEnabled = true;
    bool colorWriteEnabled = true;

#ifdef USE_OGL
    GraphicSystemBackEnd* CreateGraphicSystemBackEnd() const {return new OGLGraphicSystemBackEnd();}
#endif

};

}

#endif