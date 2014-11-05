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
in vec3 f_worldSpaceViewRay;
in vec3 f_cameraWorldPosition;

out vec4 occlusionMask;

//Converts the int bit mask to a fixed rgba value.
vec4 encodeOcclusionMask(int mask)
{
    float r = (0xFF & (mask >> 24));
    float g = (0xFF & (mask >> 16));
    float b = (0xFF & (mask >> 8));
    float a = (0xFF & (mask));
    return vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

bool isOccluded(in mat4 viewToLightViewProjection, in vec3 position, in float arrayIndexZ, in float bias)
{
    //First transform the view-space position to light clip space and then to texture space( [-1.0 1.0] -> [0.0 1.0] ).
    vec4 lightClipSpacePos = viewToLightViewProjection * vec4(-position, 1.0f);
    
    lightClipSpacePos /= lightClipSpacePos.w;
    float lightClipSpacePosDepth = lightClipSpacePos.z * 0.5f + 0.5f;
  
    lightClipSpacePos.xy = lightClipSpacePos.xy * vec2(0.5f, 0.5f) + vec2(0.5f, 0.5f);
    lightClipSpacePos.z = arrayIndexZ;

    return texture(u_shadowDepth, lightClipSpacePos.xyz).r + bias < lightClipSpacePosDepth;
}

//The component with the largest magnitude determines the major axis and the sign of the component determines the direction.
int getPointLightFaceIndex(in vec3 ray)
{
    float absMagnitudeX = abs(ray.x);
    float absMagnitudeY = abs(ray.y);
    float absMagnitudeZ = abs(ray.z);

    int faceIndex;

    if((absMagnitudeX > absMagnitudeY) && (absMagnitudeX > absMagnitudeZ))
        faceIndex = (ray.x > 0.0f) ? 0 : 1;
    else if((absMagnitudeY > absMagnitudeX) && (absMagnitudeY > absMagnitudeZ))
        faceIndex = (ray.y > 0.0f) ? 2 : 3;
    else
        faceIndex = (ray.z > 0.0f) ? 4 : 5;

    return faceIndex;
}

void main()
{
    vec4 normal = texture(u_normalBuffer, f_texCoord0);

    //Negate the position after it is compared to the far distances which are positive, 
    vec3 position = f_frustumCorner * normal.a;	
    PerLightParameters parameters = u_occlusionParameters[u_shadowOcclusionParameterIndex];

    //0: Directional light,
    //1: Spot light
    //2: Point light.
    int index = 0;
    bool occluded = false;

    if(parameters.u_lightType == 0)
    {
        //Find the appropriate depth map to look up, based on view-space depth.
        index = 3;
        if(position.z < parameters.u_farDistances.x)
            index = 0;
        else if(position.z < parameters.u_farDistances.y)
            index = 1;
        else if(position.z < parameters.u_farDistances.z)
            index = 2;

        occluded = isOccluded(parameters.u_viewToLightViewProj[index], position, float(index), parameters.shadowBias);
    }
    else if(parameters.u_lightType == 1)
    {
        occluded = isOccluded(parameters.u_viewToLightViewProj[0], position, float(index), parameters.shadowBias);
    }
    else if(parameters.u_lightType == 2)
    {
        //Calculate a ray from the light position to the current pixel in world space and use it to determine which layer is sampled. 
        vec3 lightRay = (f_cameraWorldPosition + f_worldSpaceViewRay * -normal.a) - parameters.u_farDistances.xyz;
        index = getPointLightFaceIndex(lightRay);
        occluded = isOccluded(parameters.u_viewToLightViewProj[index], position, float(index), parameters.shadowBias);
    }

    occlusionMask = occluded ? encodeOcclusionMask(parameters.u_shadowOcclusionMask) : vec4(0.0f, 0.0f, 0.0f, 0.0f);
}