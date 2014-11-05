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

#ifndef Material_H
#define Material_H

#include "Graphics/Texture.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderParameterBlock.h"
#include "Graphics/Rasterization.h"
#include "Math/Matrix4x4.h"

namespace Huurre3D
{

const Vector3 DefaultColor = Vector3(0.5880f, 0.5880f, 0.5880f);
const float DefaultRoughness = 0.1f;
const float DefaultReflectance = 0.25f;
const float DefaultSpecularPower = 0.0f;
const float DefaultAlpha = 1.0f;

class Material
{
public:
    Material();
    Material(const Matrix4x4& parameters, const RasterState& rasterState);
    ~Material() = default;
	
    void setAmbientColor(const Vector3& ambient);
    void setSpecularColor(const Vector3& specular);
    void setDiffuseColor(const Vector3& diffuse);
    void setEmissiveColor(const Vector3& emissive);
    void setAmbientColor(float r, float g, float b);
    void setSpecularColor(float r, float g, float b);
    void setDiffuseColor(float r, float g, float b);
    void setEmissiveColor(float r, float g, float b);
    void setAlpha(float alpha);
    void setSpecularPower(float specularPower);
    void setRoughness(float roughness);
    void setReflectance(float refiectance);
    void setCompareMode(CompareMode compareMode);
    void setCullMode(CullMode cullMode);
    void setBlendMode(BlendMode blendMode);
    void setDiffuseTexture(Texture* texture);
    void setSpecularTexture(Texture* texture);
    void setNormalMap(Texture* texture);
    void setAlphaTexture(Texture* texture);
    void setCurrentShaderCombinationTag(unsigned int shaderCombinationTag);
    void getTextures(Vector<Texture*>& texturesOut);
    unsigned int getParameterId();
    Vector3 getAmbientColor() const {return parameters[3].xyz();}
    Vector3 getSpecularColor() const {return parameters[1].xyz();}
    Vector3 getDiffuseColor() const {return parameters[0].xyz();}
    Vector3 getEmissive() const {return parameters[2].xyz();}
    float getRoughness() const {return parameters[0].w;}
    float getReflectance() const {return parameters[2].w;}
    unsigned int getCurrentShaderCombinationTag() const {return currentShaderCombinationTag;}
    CompareMode getCompareMode() const {return rasterState.compareMode;}
    CullMode getCullMode() const {return rasterState.cullMode;}
    BlendMode getBlendMode() const {return rasterState.blendMode;}
    const Vector<ShaderDefine>& getShaderDefines() const {return shaderDefines;}
    RasterState getRasterState() const {return rasterState;}
    const Matrix4x4& getParameters() const {return parameters;}
    bool isTransparent() const {return parameters[3].w < 1.0f;}

private:
    unsigned int generateParameterId();
    //col1: diffuse + roughness
    //col2: specular + specularPower
    //col3: emissive + reflectance
    //col4: ambient + alpha
    Matrix4x4 parameters; 
    RasterState rasterState;
    unsigned int currentShaderCombinationTag = 0;
    Texture* diffuseTexture = nullptr;
    Texture* specularTexture = nullptr;
    Texture* normalMap = nullptr;
    Texture* alphaTexture = nullptr;
    Vector<ShaderDefine> shaderDefines;
    bool parametersDirty = true;
    unsigned int parameterId = 0;
};

}

#endif