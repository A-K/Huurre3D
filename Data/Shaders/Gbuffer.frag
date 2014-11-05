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

#include "Uniforms.frag"
#include "Samplers.frag"

in vec3 f_normal;
in vec3 f_tangent;
in vec3 f_bitangent;
in vec2 f_texCoord0;
in float f_linear_depth;

layout(location = 0) out vec4 o_diffuse; //Diffuse color + material index
layout(location = 1) out vec4 o_specular; //Specular color
layout(location = 2) out vec4 o_normal; //Normal + linear depth [0, 1]

void main()
{
#ifdef ALPHA_MASK
    if(texture(u_alphaTexture, f_texCoord0).r < 0.5)
        discard;
#endif

    vec3 normal = normalize(f_normal);
    vec3 tangent = normalize(f_tangent);
    vec3 bitangent = normalize(f_bitangent);
    vec3 normalSample = vec3(1.0, 1.0, 1.0);
    vec3 diffuse = vec3(1.0, 1.0, 1.0);
    vec3 specular = vec3(1.0, 1.0, 1.0);

#ifdef NORMAL_TEXTURE
    normalSample = texture(u_normalTexture, f_texCoord0).rgb * 2.0 - 1.0;
    mat3 tbn = mat3(tangent, bitangent, normal);
    o_normal = vec4(normalize(tbn * normalSample), f_linear_depth);
#else
    o_normal = vec4(normalize(normal), f_linear_depth);
#endif

#ifdef DIFFUSE_TEXTURE
    diffuse = texture(u_diffuseTexture, f_texCoord0).rgb;
#endif

    o_diffuse = vec4(diffuse, float(u_materialParameterIndex) / 256.0f);

#ifdef SPECULAR_TEXTURE
    specular = texture(u_specularTexture, f_texCoord0).rgb;
#endif

    o_specular = vec4(specular, 1.0f);
}