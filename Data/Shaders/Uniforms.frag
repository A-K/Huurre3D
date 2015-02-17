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

#ifdef MAX_NUM_MATERIALS
//Material properties uniform buffer.
struct MaterialProperties
{
    vec4 u_diffuseColor; //Diffuse color + roughness
    vec4 u_specularColor; //Specular color + specularPower
    vec4 u_emissiveColor; //Emissive color + reflectance
    vec4 u_ambientColor; //Ambient color + alpha
};

layout(std140) uniform u_materialProperties
{
    MaterialProperties u_materials[MAX_NUM_MATERIALS];
};
#endif

#ifdef MAX_SHADOW_LIGHTS
struct PerLightParameters
{
    mat4 u_viewToLightViewProj[6];
    vec4 u_farDistances; //When light type is point light, this contains world space position.
    int u_lightType;
    int u_shadowOcclusionMask;
    float shadowBias;
    float padding;
};

layout(std140) uniform u_shadowOcclusionParameters  
{
    PerLightParameters u_occlusionParameters[MAX_SHADOW_LIGHTS];
};
#endif

#ifdef MAX_NUM_LIGHTS
struct Light
{
    vec4 u_position; //Light Position in view space + radius.
    vec4 u_color;    //Light color + falloff exponent.
    vec4 u_direction; //Light direction in view space + light type.
    vec4 u_innerOuterAngles; //Cosines of the spot light inner and outer angles. + shadow parameters: light castShadow(0: dont cast shadows)
                             //and shadow occlusion mask (bit flag mask for shadow occlusion texture).
};

//Buffer includes all lights currently in camera's frustum.
layout(std140) uniform u_lightParameters
{
    vec4 u_numLightsAndGlobalAmbientLight;
    Light u_lights[MAX_NUM_LIGHTS];
};
#endif

layout(std140) uniform u_lightGridParameters
{
    ivec4 u_gridDimensions; //Tile width, tile height, width resolution, height resolution.
};

layout(std140) uniform u_SSAOParameters
{
    vec4 u_SAOParameters; //projScale, radius, bias, intensity 
};

layout(std140) uniform u_renderTargetParameters
{
    vec4 u_renderTargetSize; //width, height, inverse width and inverse height
};

uniform int u_shadowOcclusionParameterIndex;
uniform int u_materialParameterIndex;