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

#ifndef OGLGraphicSystemBackEnd_H
#define OGLGraphicSystemBackEnd_H

#include "Graphics/GraphicSystemBackEnd.h"

namespace Huurre3D
{

class Shader;
class IndexBuffer;
class AttributeBuffer;
class VertexStream;

class OGLGraphicSystemBackEnd : public GraphicSystemBackEnd
{
public:
    OGLGraphicSystemBackEnd() = default;
    ~OGLGraphicSystemBackEnd() = default;

    void setViewPort(const ViewPort& viewPort);
    void clear(unsigned int flags, const Vector4& color);
    //Generates a buffer object for attribute, index and uniform buffers.
    unsigned int createBuffer();
    //Generates a vertex array object, that is used to encapsulate the vertex data state.
    unsigned int createVertexData();
    //Generates a texture object.
    unsigned int createTexture();
    //Generates a shader program object.
    unsigned int createShaderProgram();
    //Generates a frameBuffer object.
    unsigned int createRenderTarget(int width, int height, bool depthBuffer, int numBuffers, int numLayers);
    void removeBuffer(unsigned int bufferId);
    void removeVertexData(unsigned int vertexDataId);
    void removeTexture(unsigned int textureId);
    void removeShaderProgram(unsigned int shaderProgramId);
    void removeRenderTarget(unsigned int renderTargetId);
    //Sets the vertex data as the currently active vertex data state.
    void setVertexData(VertexData* vertexData);
    //Sets the shader program as the currently active shader program.
    void setShaderProgram(ShaderProgram* program);
    //Sets the uniform (shader parameter) to the currently active shader program.
    void setShaderParameter(ShaderParameterDescription* description, const ShaderParameter& shaderParameter);
    //binds the uniform buffer (shader parameter block) to the currently active shader program.
    void setShaderParameterBlock(ShaderParameterBlockDescription* description, ShaderParameterBlock* block);
    //Sets the texture as active texture and binds it.
    void setTexture(Texture* texture);
    void setDepthWrite(bool enable);
    void setColorWrite(bool enable);
    //sets the cull, depth compare and blend modes
    void setRasterState(const RasterState& state);
    //Binds the framebuffer (renderTarget).   
    void setOffLineRenderTarget(RenderTarget* renderTarget);
    //Binds the window framebuffer (main renderTarget).
    void setMainRenderTarget();
    //Draws the currently active vertex buffer. 
    void draw(int numVertices, int vertexOffset);
    //Draws the currently active vertex  and index buffer. 
    void drawIndexed(int numIndices, int indexOffset);
    void drawInstanced(int numIndices, int indexOffset, int instancesCount);

private:
    void setBlendMode(BlendMode mode);
    void setDepthCompareMode(CompareMode mode);
    void setCullMode(CullMode mode);
    void updateTexture(Texture* texture);
    void updateShaderProgram(ShaderProgram* program);
    void updateRenderTarget(RenderTarget* renderTarget);
    void updateShaderParameterBlock(ShaderParameterBlock* block);
    void updateVertexStream(VertexStream* stream);
    void updateInterleavedVertexStream(VertexStream* stream);
    void updateIndexBuffer(IndexBuffer* buffer);
    //Builds a shader program from vertex and fragment shader sources.
    void loadShaderProgram(Shader* vertexShader, Shader* fragmentShader);
    void compileShader(Shader* shader);
    //Fetches the needed information of the uniform blocks.
    void fetchUniformBlocks(ShaderProgram* program);
    //Fetches the needed information of single uniforms, when uniform buffers are not supported.
    void fetchUniforms(ShaderProgram* program);
    void enableAttributes(VertexStream* vertexStream);
    void enableInterleavedAttributes(VertexStream* vertexStream);
    void disableAttributes(VertexStream* vertexStream);
    //Sets the texture parameters to texture object.
    void updateTextureParameters(Texture *texture);
    //Loads the the pixel data to texture object.
    void updateTextureData(Texture* texture);

    int currentlyActiveTextureSlot = 0;
    Texture* texturesInUse[static_cast<int>(TextureSlotIndex::NumSlots)];
    BlendMode currentBlendMode = BlendMode::Off;
    CompareMode currentCompareMode = CompareMode::Never;
    CullMode currentCullMode = CullMode::None;
    unsigned int currentShaderProgramId = 0;
    unsigned int currentFBOId = 0;
    unsigned int currentBindedShaderBlockId = 0;
    PrimitiveType currentPrimitiveType = PrimitiveType::Triangles;
    IndexType currentIndexType = IndexType::Short;
};

}

#endif