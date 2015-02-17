#version 330
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

#include "Uniforms.frag"
#include "Samplers.frag"

in vec2 f_texCoord0;
in vec3 f_frustumCorner;

out float o_blur;

const float bilateralScale = 1.3f;
const float centerWeight = 0.3162162162f;
const vec4 gaussian = vec4(0.0702702703f, 0.2270270270, 0.2270270270, 0.0702702703f);
const vec4 offsets = vec4(-3.2307692308f, -1.3846153846f, 1.3846153846f, 3.2307692308f);

//5-tap bilateral linear gaussian blur.
void main()
{

#ifdef VERTICAL_BLUR
    vec2 dir = vec2(0.0f, 1.0f);
    float layer = 1.0f;
#else
    vec2 dir = vec2(1.0f, 0.0f);
    float layer = 0.0f;
#endif

    float centerDepth = texture(u_normalBuffer, f_texCoord0).a;

    if(centerDepth == 1.0f)
    {
        o_blur = 1.0f;
        return;
    }

    vec2 texOffset = u_renderTargetSize.zw;
    vec4 weights = gaussian;
    vec3 samplePositions[4] = vec3[4](vec3(f_texCoord0, layer) + vec3(texOffset * offsets.x * dir, 0.0f),
                                      vec3(f_texCoord0, layer) + vec3(texOffset * offsets.y * dir, 0.0f),
                                      vec3(f_texCoord0, layer) + vec3(texOffset * offsets.z * dir, 0.0f),
                                      vec3(f_texCoord0, layer) + vec3(texOffset * offsets.w * dir, 0.0f));

    float centerAO = texture(u_ssao, vec3(f_texCoord0, layer)).r;
    float result = centerAO * centerWeight;
    float totalWeight = centerWeight;

    vec4 depthSamples = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 aoSamples = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    depthSamples.x = texture(u_normalBuffer, samplePositions[0].xy).a;
    depthSamples.y = texture(u_normalBuffer, samplePositions[1].xy).a;
    depthSamples.z = texture(u_normalBuffer, samplePositions[2].xy).a;
    depthSamples.w = texture(u_normalBuffer, samplePositions[3].xy).a;

    aoSamples.x = texture(u_ssao, samplePositions[0]).r;
    aoSamples.y = texture(u_ssao, samplePositions[1]).r;
    aoSamples.z = texture(u_ssao, samplePositions[2]).r;
    aoSamples.w = texture(u_ssao, samplePositions[3]).r;

    vec4 depthDiffs = abs(vec4(centerDepth) - depthSamples);
    
    //As depth difference increases the bilateral weight decreases.
    weights *= max(vec4(0.0f), vec4(1.0f) - (20.0f * bilateralScale) * depthDiffs);

    result += dot(weights, aoSamples);
    totalWeight += dot(weights, vec4(1.0f));
    
    o_blur = result / totalWeight;
}