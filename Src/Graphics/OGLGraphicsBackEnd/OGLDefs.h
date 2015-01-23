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

#ifndef OGLDefs_H
#define OGLDefs_H

#include "Graphics/OGLGraphicsBackEnd/OGLHeaders.h"

namespace Huurre3D
{

static const GLenum glWrapMode[] =
{
    GL_REPEAT,
    GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_BORDER,
};

static const GLenum glMinFilter[] =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_LINEAR
};

static const GLenum glMinMipMapFilter[] =
{
    GL_NEAREST_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_LINEAR
};

static const GLenum glMaxFilter[] =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_LINEAR
};

static const GLenum glPixelFormat[] =
{
    GL_RGB,
    GL_RGBA,
    GL_RGBA,
    GL_RGBA,
    GL_RGBA_INTEGER,
    GL_RED,
    GL_RED,
    GL_RED_INTEGER,
    GL_RED_INTEGER,
    GL_DEPTH_COMPONENT,
    GL_DEPTH_COMPONENT
};

static const GLenum glPixelInternalFormat[] =
{
    GL_RGB8,
    GL_RGBA8,
    GL_RGBA32F,
    GL_RGBA16F,
    GL_RGBA32I,
    GL_R16F,
    GL_R32F,
    GL_R16I,
    GL_R32I,
    GL_DEPTH_COMPONENT,
    GL_DEPTH_COMPONENT24,
    GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
    GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
    GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
};

static const GLenum glPixelType[] =
{
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_FLOAT,
    GL_HALF_FLOAT,
    GL_INT,
    GL_HALF_FLOAT,
    GL_FLOAT,
    GL_SHORT,
    GL_INT,
    GL_UNSIGNED_BYTE,
    GL_FLOAT
};

static const GLenum glTextureTarget[] =
{
    GL_TEXTURE_2D,
    GL_TEXTURE_2D_ARRAY,
    GL_TEXTURE_3D,
    GL_TEXTURE_CUBE_MAP
};

static const GLenum glCubeMapFaces[] =
{
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

static const GLenum glAttributeType[] =
{
    GL_BYTE,
    GL_SHORT,
    GL_INT,
    GL_FLOAT
};

static const GLenum glPrimitiveType[] =
{
    GL_POINTS,
    GL_LINES,
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP
};

static const GLenum glIndexType[] =
{
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT
};

static const GLsizei glIndexSize[] =
{
    sizeof(GLushort),
    sizeof(GLuint)
};

static const GLenum glBlendSourceFactor[] =
{
    GL_ZERO,
    GL_ONE,
    GL_SRC_ALPHA,
    GL_SRC_ALPHA,
    GL_DST_COLOR
};

static const GLenum glBlendDestFactor[] =
{
    GL_ONE,
    GL_ONE,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_ONE,
    GL_ZERO
};

static const GLenum glCompareMode[] =
{
    GL_ALWAYS,
    GL_EQUAL,
    GL_NOTEQUAL,
    GL_LESS,
    GL_LEQUAL,
    GL_GREATER,
    GL_GEQUAL,
    GL_NEVER
};

static const GLenum glCullMode[] =
{
    GL_FRONT,
    GL_BACK,
    GL_FRONT_AND_BACK
};

static const GLchar* glAttributeName[] =
{
    "i_position",
    "i_normal",
    "i_tangent",
    "i_bitangent",
    "i_jointIndices",
    "i_jointWeights",
    "i_texcoord0",
    "i_texcoord1",
    "i_texcoord2",
    "i_texcoord3"
};

static const GLenum glUniformType[] =
{
    GL_FLOAT,
    GL_FLOAT_VEC2,
    GL_FLOAT_VEC3,
    GL_FLOAT_VEC4,
    GL_INT,
    GL_INT_VEC2,
    GL_INT_VEC3,
    GL_INT_VEC4,
    GL_FLOAT_MAT2,
    GL_FLOAT_MAT3,
    GL_FLOAT_MAT4,
};

static const GLchar* glTextureSlots[] =
{
    "u_depth",
    "u_diffuseTexture",
    "u_specularTexture",
    "u_normalTexture",
    "u_alphaTexture",
    "u_diffuseBuffer",
    "u_specularBuffer",
    "u_normalBuffer",
    "u_tileLightInfo",
    "u_shadowDepth",
    "u_shadowOcclusion",
    "u_ssao",
    "u_lightingTexture",
    "u_skyBox"
};

static const GLenum shaderTypes[] =
{
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
};

}
#endif