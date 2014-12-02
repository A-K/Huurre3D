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

#ifdef SAO
#include "SAO.frag"
#endif

out float o_ambientOcclusion;

void main()
{
    vec2 texOffset = vec2(1.0f / u_SSAORenderTargetSize.x, 1.0f / u_SSAORenderTargetSize.y);
    float ambientOcclusion = 1.0f;
    vec4 normal = texture(u_normalBuffer, f_texCoord0);

#ifdef SAO
    ivec2 screenSpacePosition = ivec2(gl_FragCoord.xy);
    float projScale = u_SAOParameters.x;
    float radius = u_SAOParameters.y;
    float bias = u_SAOParameters.z;
    float intensity = u_SAOParameters.w;
    ambientOcclusion = calculateSAO(screenSpacePosition, normal, projScale, radius, bias, texOffset);
    ambientOcclusion = clamp(pow(ambientOcclusion, 1.0 + intensity), 0.0, 1.0);
#endif

    o_ambientOcclusion = ambientOcclusion;
}
