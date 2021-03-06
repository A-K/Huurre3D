#version 330
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

#include "../../Data/Shaders/Uniforms.frag"
#include "../../Data/Shaders/Samplers.frag"
#include "../../Data/Shaders/TiledShading.frag"
#include "../../Data/Shaders/ToneMap.frag"

layout(std140) uniform u_showSSAOParameters
{
    ivec4 u_ssaoOn; //x: use ssao, y: show ssao texture.
};

in vec2 f_texCoord0;
in vec3 f_frustumCorner;

out vec4  o_color;

void main()
{
    vec4 normal = texture(u_normalBuffer, f_texCoord0);
    vec3 position = f_frustumCorner * -normal.a;

    vec4 diffuse = texture(u_diffuseBuffer, f_texCoord0);
    vec4 specular = texture(u_specularBuffer, f_texCoord0);
    int materialIndex = int(diffuse.a * 255.0f);
    MaterialProperties materialProperties = u_materials[materialIndex];

    diffuse.rgb *= materialProperties.u_diffuseColor.rgb;
    specular.rgb *= materialProperties.u_specularColor.rgb;
    float roughness = materialProperties.u_diffuseColor.a;
    float reflectance = materialProperties.u_emissiveColor.a;

    vec3 globalAmbient = u_numLightsAndGlobalAmbientLight.yzw;
    vec4 ambient = vec4(materialProperties.u_ambientColor.rgb * diffuse.rgb * globalAmbient, 1.0f);
    vec4 lighting = processFragment(f_texCoord0, position, normal.rgb, diffuse.rgb, specular.rgb, roughness, reflectance);

    vec4 finalColor;

    if(u_ssaoOn.x == 1)
    {
        float ambientOcclusion = texture(u_ssao, vec3(f_texCoord0, 0.0f)).r;

        if(u_ssaoOn.y == 1)
            finalColor = vec4(ambientOcclusion, ambientOcclusion, ambientOcclusion, 1.0f);
        else
            finalColor = lighting + ambient * ambientOcclusion + vec4(materialProperties.u_emissiveColor.rgb, 1.0f);
    }
    else
        finalColor = lighting + ambient + vec4(materialProperties.u_emissiveColor.rgb, 1.0f);

    //Gamma correction.
    finalColor.rgb = toSrgb(finalColor.rgb);

    //Calculate Luma for FXAA.
    finalColor.a = dot(finalColor.rgb, vec3(0.299f, 0.587f, 0.114f));
    o_color = finalColor;
}