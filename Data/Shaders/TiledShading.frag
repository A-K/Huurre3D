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
#include "Filter.frag"
#include "Brdf.frag"

float calcAttenuation(in float lightRadius, in float fallOffExponent, in float distToLight)
{
    return pow(max(0.0, 1.0 - (distToLight / lightRadius)), fallOffExponent);
}

//Calculates the compined lighting and shadowing for all the lights affecting this fragment.
vec4 processFragment(in vec2 texCoord, in vec3 position, in vec3 normal, in vec3 diffuse, in vec3 specular, in float roughness, in float reflectance)
{ 
    int lightIndex = 0;
    ivec2 lightIndexSampler = ivec2(0,0);

    //Get the tile index which includes this fragment
    int tileIndex = (int(gl_FragCoord.y) / u_gridDimensions.y) * u_gridDimensions.z  + (int(gl_FragCoord.x) / u_gridDimensions.x);
    lightIndexSampler.y = tileIndex;
    lightIndexSampler.x = 0;

    vec3 color = vec3(0.0f, 0.0f, 0.0f);
    int numLights = int(u_numLightsAndGlobalAmbientLight.x);

    //Get the shadow occlusion values for this fragment.
    ivec4 shadowOcllusionValues = readAndDecodeShadowOcclusion2x2(texCoord, u_renderTargetSize.zw, u_shadowOcclusion);

    while(lightIndexSampler.x < numLights)
    {
        //Get light index affecting this tile
        lightIndex = texelFetch(u_tileLightInfo, lightIndexSampler, 0).r;

        //No more lights affecting this tile, move to next tie.
        if(lightIndex == -1)
            break;

        float shadowOcclusion = 1.0f;
        Light light = u_lights[lightIndex];

        //Does the light cast shadows.
        if(light.u_innerOuterAngles.z > 0)
        {
            int mask = int(light.u_innerOuterAngles.w);
            shadowOcclusion = PCF4(shadowOcllusionValues, mask);
        }

        vec3 lightDir;
        float attenuation = 1.0f;

        //0: Directional light,
        //1: Spot light
        //2: Point light.
        int lightType = int(light.u_direction.w);
        if(lightType == 0)
        {
            lightDir = -light.u_direction.xyz;
        }
        else if(lightType == 1)
        {
            vec3 spotDir = -light.u_direction.xyz;
            lightDir = light.u_position.xyz - position;
            float dist = length(lightDir);

            //Calculate the spot effect.
            float angle = dot(normalize(lightDir), spotDir);
            float spotEffect = smoothstep(light.u_innerOuterAngles.y, light.u_innerOuterAngles.x, angle);
            attenuation = spotEffect * calcAttenuation(light.u_position.w, light.u_color.w, dist);
        }
        else if(lightType == 2)
        {
            //color += light.u_color.rgb * 0.1; //visualize the bounding rect
            lightDir = light.u_position.xyz - position;
            float dist = length(lightDir);
            attenuation = calcAttenuation(light.u_position.w, light.u_color.w, dist);
        }

        lightDir = normalize(lightDir);
        vec3 viewDir = -normalize(position);
        color += shadowOcclusion * attenuation * calculateBrdf(light.u_color.xyz, diffuse, specular, normal, viewDir, lightDir, roughness, reflectance);
		
        //Move to the next light affecting this tile.
        lightIndexSampler.x += 1;
    }

    return vec4(color, 1.0);
}