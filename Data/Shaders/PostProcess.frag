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

in vec2 f_texCoord0;
in vec3 f_frustumCorner;
in vec3 f_frustumFarPlaneLeftBottomCorner;
in vec2 f_frustumFarPlaneSize;

#include "FXAA.frag"

#ifdef SAO
#include "SAO.frag"
#endif

out vec4 o_color;

//TODO: Move the SAO out of post processing.
//SAO should be calculated and rendered into a texture before lighting calculations,
//where it should contribute only to the ambient lighting term.
void main()
{
    vec3 color = vec3(0.0f, 0.0f, 0.0f);
    vec2 texOffset = vec2(1.0f / u_postRenderTargetSize.x, 1.0f / u_postRenderTargetSize.y);
    float ambientOcclusion = 1.0f;

#ifdef SAO
    vec4 normal = texture(u_normalBuffer, f_texCoord0);
    ivec2 screenSpacePosition = ivec2(gl_FragCoord.xy);
    float projScale = 500.0f;//u_SAOParameters.x;
    float radius = 0.6f;// u_SAOParameters.y;
    float bias = 0.026f;//u_SAOParameters.z;
    float intensity = 5.0;// u_SAOParameters.w;
    ambientOcclusion = calculateSAO(screenSpacePosition, normal, projScale, radius, bias, texOffset);
    ambientOcclusion = clamp(pow(ambientOcclusion, 1.0 + intensity), 0.0, 1.0);
#endif

    float fxaaQualitySubpix = 0.75;
    float fxaaQualityEdgeThreshold = 0.166;
    float fxaaQualityEdgeThresholdMin = 0.0833;
    color = fxaaPixelShader(f_texCoord0, u_lightingTexture, texOffset, fxaaQualitySubpix, fxaaQualityEdgeThreshold, fxaaQualityEdgeThresholdMin).rgb;

    o_color = vec4(color /** ambientOcclusion*/, 1.0f);
}