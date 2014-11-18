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

#include "Graphics/OGLGraphicsBackEnd/OGLDefs.h"
namespace Huurre3D
{

const GLenum glWrapMode[] =
{
    GL_REPEAT,
    GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_BORDER,
};

const GLenum glMinFilter[] =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_LINEAR
};

const GLenum glMinMipMapFilter[] =
{
    GL_NEAREST_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_LINEAR
};

const GLenum glMaxFilter[] =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_LINEAR
};

const GLenum glPixelFormat[] =
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

const GLenum glPixelInternalFormat[] =
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

const GLenum glPixelType[] =
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

const GLenum glTextureTarget[] =
{
    GL_TEXTURE_2D,
    GL_TEXTURE_2D_ARRAY,
    GL_TEXTURE_3D,
    GL_TEXTURE_CUBE_MAP
};

const GLenum glCubeMapFaces[] =
{
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

const GLenum glAttributeType[] =
{
    GL_BYTE,
    GL_SHORT,
    GL_INT,
    GL_FLOAT
};

const GLenum glPrimitiveType[] =
{
    GL_POINTS,
    GL_LINES,
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP
};

const GLenum glIndexType[] =
{
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT
};

const GLsizei glIndexSize[] =
{
    sizeof(GLushort),
    sizeof(GLuint)
};

const GLenum glBlendSourceFactor[] =
{
    GL_ZERO,
    GL_ZERO,
    GL_ONE,
    GL_SRC_ALPHA,
    GL_SRC_ALPHA,
    GL_DST_COLOR
};

const GLenum glBlendDestFactor[] =
{
    GL_ZERO,
    GL_ONE,
    GL_ONE,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_ONE,
    GL_ZERO
};

const GLenum glCompareMode[] =
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

const GLenum glCullMode[] =
{
    GL_ZERO,
    GL_FRONT,
    GL_BACK,
    GL_FRONT_AND_BACK
};

const GLchar* glAttributeName[] =
{
    "i_position",
    "i_normal",
    "i_color",
    "i_tangent",
    "i_bitangent",
    "i_texcoord0",
    "i_texcoord1",
    "i_texcoord2",
    "i_texcoord3"
};

const GLenum glUniformType[] =
{
    GL_FLOAT,
    GL_FLOAT_VEC2,
    GL_FLOAT_VEC3,
    GL_FLOAT_VEC4,
    GL_INT,
    GL_INT_VEC2,
    GL_INT_VEC3,
    GL_INT_VEC4,
    GL_BOOL,
    GL_BOOL_VEC2,
    GL_BOOL_VEC3,
    GL_BOOL_VEC4,
    GL_FLOAT_MAT2,
    GL_FLOAT_MAT3,
    GL_FLOAT_MAT4,
};

const GLchar* glShaderDefines[] =
{
    "#define DIFFUSE_TEXTURE",
    "#define SPECULAR_TEXTURE",
    "#define ALPHA_MASK",
    "#define NORMAL_TEXTURE",
    "#define ENVIRONMENT_MAP",
    "#define MAX_NUM_MATERIALS",
    "#define MAX_NUM_LIGHTS",
    "#define MAX_SHADOW_LIGHTS",
    "#define USE_WORLD_PARAMETERS",
    "#define FXAA_QUALITY__PRESET",
    "#define HDR",
    "#define SAO",
    "#define NUM_SAO_SAMPLES",
    "#define NUM_SAO_SPIRAL_TURNS"
};


const GLchar* glShaderParameters[] = 
{
    "u_worldTransform",
    "u_cameraParameters",
    "u_lightParameters",
    "u_lightGridParameters",
    "u_materialProperties",
    "u_materialParameterIndex",
    "u_lightViewProjectionMatrix",
    "u_shadowOcclusionParameters",
    "u_shadowOcclusionParameterIndex",
    "u_postProcessParameters"
};

const GLchar* glTextureSlots[] = 
{
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
    "u_lightingTexture",
    "u_skyBox"
};

const GLenum shaderTypes[] =
{
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
};

}