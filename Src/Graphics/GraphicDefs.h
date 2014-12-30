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

#include "Util/EnumClassDeclaration.h"
#include <string>

namespace Huurre3D
{

DECLARE_ENUM_CLASS(PrimitiveType, Points, Lines, Triangles, TriangleStrip);
DECLARE_ENUM_CLASS(AttributeType, Byte, Short, Int, Float);
DECLARE_ENUM_CLASS(AttributeSemantic, Position, Normal, Color, Tangent, BiTanget, TexCoord0, TexCoord1, TexCoord2, TexCoord3, NumSemantics);
DECLARE_ENUM_CLASS(IndexType, Short, Int);
DECLARE_ENUM_CLASS(BlendFunction, Replace, Add, Alpha, AddAlpha, Modulate);
DECLARE_ENUM_CLASS(CompareFunction, Always, Equal, NotEqual, Less, LessEqual, Greater, GreaterEqual, Never);
DECLARE_ENUM_CLASS(CullFace, Front, Back, FrontAndBack);
DECLARE_ENUM_CLASS(ShaderType, Vertex, Fragment);
DECLARE_ENUM_CLASS(ShaderParameterType, Float, FloatVector2, FloatVector3, FloatVector4, Int, IntVector2, IntVector3, IntVector4, FloatMatrix2, FloatMatrix3, FloatMatrix4, NumTypes);
DECLARE_ENUM_CLASS(TextureSlotIndex, Diffuse, Specular, NormalMap, Alpha, DiffuseBuffer, SpecularBuffer, NormalBuffer, TileLightInfo, ShadowDepth, ShadowOcclusion, SSAO, Lighting, SkyBox, NumSlots);
DECLARE_ENUM_CLASS(TextureWrapMode, Repeat, Mirror, ClampEdge, ClampBorder);
DECLARE_ENUM_CLASS(TextureFilterMode, Nearest, Bilinear, Trilinear);
DECLARE_ENUM_CLASS(TexturePixelFormat, Rgb8, Rgba8, Rgba32F, Rgba16F, Rgba32I, Red16F, Red32F, Red16I, Red32I, Depth, Depth24, DXT1, DXT3, DXT5);
DECLARE_ENUM_CLASS(TextureTargetMode, Texture2D, Texture2DArray, Texture3D, TextureCubeMap);
DECLARE_ENUM_CLASS(CubeMapFace, PositiveX, NegativeX, PositiveY, NegativeY, PositiveZ, NegativeZ);

static const int attributeSize[] =
{
    sizeof(unsigned char),
    sizeof(short),
    sizeof(int),
    sizeof(float)
};

static const int indexSize[] =
{
    sizeof(unsigned short),
    sizeof(unsigned int)
};

static const int pixelFormatNumComponents[] =
{
    3, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 4, 4, 4
};

static const int pixelFormatSizeInBytes[] =
{
    3, 4, 16, 8, 16, 2, 4, 2, 4, 1, 3, 8, 16, 16
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
static const std::string sd_diffuseTexture = "DIFFUSE_TEXTURE";
static const std::string sd_specularTexture = "SPECULAR_TEXTURE";
static const std::string sd_alphaMask = "ALPHA_MASK";
static const std::string sd_normalTexture = "NORMAL_TEXTURE";
static const std::string sd_maxNumMaterials = "MAX_NUM_MATERIALS";
static const std::string sd_maxNumLights = "MAX_NUM_LIGHTS";
static const std::string sd_maxNumShadowLights = "MAX_SHADOW_LIGHTS";
static const std::string sd_useWorldSpaceParameters = "USE_WORLD_PARAMETERS";
static const std::string sd_fxaaQuality = "FXAA_QUALITY__PRESET";
static const std::string sd_HDR = "HDR";
static const std::string sd_SSAO = "SSAO";
static const std::string sd_SAO = "SAO";
static const std::string sd_numSAOSamples = "NUM_SAO_SAMPLES";
static const std::string sd_numSAOSpiralTurns = "NUM_SAO_SPIRAL_TURNS";
static const std::string sd_verticalBlur = "VERTICAL_BLUR";

}

#endif