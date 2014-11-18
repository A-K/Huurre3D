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

const int attributeSize[] =
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

const int indexSize[] =
{
    sizeof(unsigned short),
    sizeof(unsigned int)
};

enum class BlendMode
{
    Off = 0,
    Replace,
    Add,
    Alpha,
    AddAlpha,
    Modulate
};

enum class CompareMode
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

enum class CullMode
{
    None = 0,
    Front,
    Back,
    FrontAndBack
};

enum class ShaderType
{
    Vertex,
    Fragment
};

//Defines to toggle features on.
enum class ShaderDefineType
{
    DiffuseTexture,
    SpecularTexture,
    AlphaMask,
    NormalTexture,
    EnvironmentMap,
    MaxNumMaterials,
    MaxNumLights,
    MaxNumShadowLights,
    UseWorldSpaceParameters,
    FxaaQuality,
    Hdr,
    SAO,
    NumSAOSamples,
    NumSAOSpiralTurns
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

const int pixelFormatNumComponents[] =
{
    3, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 4, 4, 4
};

const int pixelFormatSizeInBytes[] =
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

}

#endif