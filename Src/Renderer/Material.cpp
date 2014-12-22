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

#include "Renderer/Material.h"
#include "Math/MathFunctions.h"
#include <iostream>

namespace Huurre3D
{

Material::Material():
Material(Matrix4x4(Vector4(DefaultColor, DefaultRoughness), Vector4(DefaultColor, DefaultSpecularPower), Vector4(Vector3::ZERO, DefaultReflectance), Vector4(DefaultColor, DefaultAlpha)), 
         RasterState(BlendState(false, BlendFunction::Replace), CompareState(true, CompareFunction::Less), CullState(true, CullFace::Back)))
{
}

Material::Material(const Matrix4x4& parameters, const RasterState& rasterState):
parameters(parameters),
rasterState(rasterState)
{
}

void Material::setAmbientColor(const Vector3& ambient)
{
    setAmbientColor(ambient.x, ambient.y, ambient.z);
}

void Material::setSpecularColor(const Vector3& specular)
{
    setSpecularColor(specular.x, specular.y, specular.z);
}

void Material::setDiffuseColor(const Vector3& diffuse)
{
    setDiffuseColor(diffuse.x, diffuse.y, diffuse.z);
}

void Material::setEmissiveColor(const Vector3& emissive)
{
    setEmissiveColor(emissive.x, emissive.y, emissive.z);
}

void Material::setAmbientColor(float r, float g, float b)
{
    parameters[3].x = r;
    parameters[3].y = g;
    parameters[3].z = b;
    parametersDirty = true;
}

void Material::setSpecularColor(float r, float g, float b)
{
    parameters[1].x = r;
    parameters[1].y = g;
    parameters[1].z = b;
    parametersDirty = true;
}

void Material::setDiffuseColor(float r, float g, float b)
{
    parameters[0].x = r;
    parameters[0].y = g;
    parameters[0].z = b;
    parametersDirty = true;
}

void Material::setEmissiveColor(float r, float g, float b)
{
    parameters[2].x = r;
    parameters[2].y = g;
    parameters[2].z = b;
    parametersDirty = true;
}

void Material::setAlpha(float alpha)
{
    parameters[3].w = alpha;
    parametersDirty = true;
}

void Material::setSpecularPower(float specularPower)
{
    parameters[1].w = specularPower;
    parametersDirty = true;
}

void Material::setRoughness(float roughness)
{
    parameters[0].w = roughness;
    parametersDirty = true;
}

void Material::setReflectance(float reflectance)
{
    parameters[2].w = reflectance;
    parametersDirty = true;
}

void Material::setDiffuseTexture(Texture* texture)
{
    diffuseTexture = texture;
    shaderDefines.pushBack(ShaderDefine(sd_diffuseTexture, ""));
}

void Material::setSpecularTexture(Texture* texture)
{
    specularTexture = texture;
    shaderDefines.pushBack(ShaderDefine(sd_specularTexture, ""));
}

void Material::setNormalMap(Texture* texture)
{
    normalMap = texture;
    shaderDefines.pushBack(ShaderDefine(sd_normalTexture, ""));
}

void Material::setAlphaTexture(Texture* texture)
{
    alphaTexture = texture;
    shaderDefines.pushBack(ShaderDefine(sd_alphaMask, ""));
}

void Material::setCurrentShaderCombinationTag(unsigned int shaderCombinationTag)
{
    currentShaderCombinationTag = shaderCombinationTag;
}

void Material::setRasterState(const RasterState& state)
{
    rasterState = state;
}

void Material::getTextures(Vector<Texture*>& texturesOut)
{
    if(diffuseTexture)
        texturesOut.pushBack(diffuseTexture);
    if(specularTexture)
        texturesOut.pushBack(specularTexture);
    if(normalMap)
        texturesOut.pushBack(normalMap);
    if(alphaTexture)
        texturesOut.pushBack(alphaTexture);
}

unsigned int Material::getParameterId()
{
    return !parametersDirty ? parameterId : generateParameterId();
}

unsigned int Material::generateParameterId()
{
    parameterId = generateHash((unsigned char*)parameters.toArray(), 64);
    return parameterId;
}

}