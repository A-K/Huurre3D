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

#ifndef GraphicDefs_H
#define GraphicDefs_H

#include <string>

namespace Huurre3D
{

enum  class PrimitiveType
{
    Points = 0,
    Lines,
    Triangles,
    TriangleStrip
};

enum class AttributeType
{
    Byte = 0,
    Short,
    Int,
    Float
};

enum class AttributeSemantic
{
    Position = 0,
    Normal,
    Color,
    Tangent,
    BiTanget,
    TexCoord0,
    TexCoord1,
    TexCoord2,
    TexCoord3,
    NumSemantics
};

static const int attributeSize[] =
{
    sizeof(unsigned char),
    sizeof(short),
    sizeof(int),
    sizeof(float)
};

enum class IndexType
{
    Short = 0,
    Int
};

static const int indexSize[] =
{
    sizeof(unsigned short),
    sizeof(unsigned int)
};

enum class BlendFunction
{
    Replace = 0,
    Add,
    Alpha,
    AddAlpha,
    Modulate
};

enum class CompareFunction
{
    Always = 0,
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Never
};

enum class CullFace
{
    Front = 0,
    Back,
    FrontAndBack
};

enum class ShaderType
{
    Vertex,
    Fragment
};


enum class ShaderParameterType
{
    Float = 0,
    FloatVector2,
    FloatVector3,
    FloatVector4,
    Int,
    IntVector2,
    IntVector3,
    IntVector4,
    Bool,
    BoolVector2,
    BoolVector3,
    BoolVector4,
    FloatMatrix2,
    FloatMatrix3,
    FloatMatrix4,
    NumTypes
};

enum class TextureSlotIndex
{
    Diffuse = 0,
    Specular,
    NormalMap,
    Alpha,
    DiffuseBuffer,
    SpecularBuffer,
    NormalBuffer,
    TileLightInfo,
    ShadowDepth,
    ShadowOcclusion,
    SSAO,
    Lighting,
    SkyBox,
    NumSlots
};

enum class TextureWrapMode
{
    Repeat = 0,
    Mirror,
    ClampEdge,
    ClampBorder
};

enum class TextureFilterMode
{
    Nearest = 0,
    Bilinear,
    Trilinear
};

enum class TexturePixelFormat
{
    Rgb8 = 0,
    Rgba8,
    Rgba32F,
    Rgba16F,
    Rgba32I,
    Red16F,
    Red32F,
    Red16I,
    Red32I,
    Depth,
    Depth24,
    DXT1,
    DXT3,
    DXT5,
};

static const int pixelFormatNumComponents[] =
{
    3, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 4, 4, 4
};

static const int pixelFormatSizeInBytes[] =
{
    3, 4, 16, 8, 16, 2, 4, 2, 4, 1, 3, 8, 16, 16
};

enum class TextureTargetMode
{
    Texture2D = 0,
    Texture2DArray,
    Texture3D,
    TextureCubeMap
};

enum class CubeMapFace
{
    PositiveX = 0,
    NegativeX,
    PositiveY,
    NegativeY,
    PositiveZ,
    NegativeZ,
};

static const unsigned int NumCubeMapFaces = 6;
static const int MaxRenderTargetBuffers = 10;

//Inbuilt engine parameters
static const std::string sp_worldTransform = "u_worldTransform";
static const std::string sp_cameraParameters = "u_cameraParameters";
static const std::string sp_lightParameters = "u_lightParameters";
static const std::string sp_lightGridParameters = "u_lightGridParameters";
static const std::string sp_materialProperties = "u_materialProperties";
static const std::string sp_materialParameterIndex = "u_materialParameterIndex";
static const std::string sp_lightViewProjectionMatrix = "u_lightViewProjectionMatrix";
static const std::string sp_shadowOcclusionParameters = "u_shadowOcclusionParameters";
static const std::string sp_shadowOcclusionParameterIndex = "u_shadowOcclusionParameterIndex";
static const std::string sp_SSAOParameters = "u_SSAOParameters";
static const std::string sp_renderTargetSize = "u_renderTargetParameters";

//Shader defines.
static const std::string sd_diffuseTexture = "#define DIFFUSE_TEXTURE";
static const std::string sd_specularTexture = "#define SPECULAR_TEXTURE";
static const std::string sd_alphaMask = "#define ALPHA_MASK";
static const std::string sd_normalTexture = "#define NORMAL_TEXTURE";
static const std::string sd_maxNumMaterials = "#define MAX_NUM_MATERIALS";
static const std::string sd_maxNumLights = "#define MAX_NUM_LIGHTS";
static const std::string sd_maxNumShadowLights = "#define MAX_SHADOW_LIGHTS";
static const std::string sd_useWorldSpaceParameters = "#define USE_WORLD_PARAMETERS";
static const std::string sd_fxaaQuality = "#define FXAA_QUALITY__PRESET";
static const std::string sd_HDR = "#define HDR";
static const std::string sd_SSAO = "#define SSAO";
static const std::string sd_SAO = "#define SAO";
static const std::string sd_numSAOSamples = "#define NUM_SAO_SAMPLES";
static const std::string sd_numSAOSpiralTurns = "#define NUM_SAO_SPIRAL_TURNS";
static const std::string sd_verticalBlur = "#define VERTICAL_BLUR";

}

#endif