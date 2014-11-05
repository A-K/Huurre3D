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

const float epsilon = 0.01;

//Implementation is based on the reference code and paper https://research.nvidia.com/publication/scalable-ambient-obscurance
float calculateSAO(in ivec2 screenSpacePosition, in vec4 normal, in float projScale, in float radius, in float bias, in vec2 texOffset)
{
    float radius2 = radius * radius;
    vec3 position = f_frustumCorner * -normal.a;

    //Hash function used in the HPG12 AlchemyAO paper
    float randomPatternRotationAngle = (3 * screenSpacePosition.x ^ screenSpacePosition.y + screenSpacePosition.x * screenSpacePosition.y) * 10;
    float screenSpaceDiskRadius = projScale * radius / position.z;

    float ao = 0.0f;
    float sampleStep = 1.0f / float(NUM_SAO_SAMPLES);

    for(int i = 0; i < NUM_SAO_SAMPLES; ++i)
    {
        float alpha = (float(i) + 0.5) * sampleStep;
        float angle = alpha * (float(NUM_SAO_SPIRAL_TURNS) * 6.28f) + randomPatternRotationAngle;
        vec2 unitOffset = vec2(cos(angle), sin(angle));
        float screenSpaceRadius = alpha * screenSpaceDiskRadius;

        //Get the screen space position of the sample.
        ivec2 sampleScreenSpacePosition = ivec2(screenSpaceRadius * unitOffset) + screenSpacePosition.xy;
        //Interpolate a frustum vector for the sample. 
        vec3 frustumVector = f_frustumFarPlaneLeftBottomCorner;
        frustumVector.xy = frustumVector.xy + (vec2(sampleScreenSpacePosition + 0.5f) * texOffset) * f_frustumFarPlaneSize;
        //Use the interpolated frustum vector and depth to reconstruct the sample's view space position. 
        vec3 sampleViewSpacePosition = frustumVector * -texelFetch(u_normalBuffer, sampleScreenSpacePosition, 0).a;

        vec3 sampleRay = sampleViewSpacePosition - position;

        float sampleRaySquaredDist = dot(sampleRay, sampleRay);
        float sampleRayAngle = dot(sampleRay, normal.xyz);

        //falloff
        float f = max(radius2 - sampleRaySquaredDist, 0.0f);
        ao += f * f * f * max((sampleRayAngle - bias) / (epsilon + sampleRaySquaredDist), 0.0f);
    }

    return max(0.0f, 1.0f - ao * (4.0f / float(NUM_SAO_SAMPLES)));
}