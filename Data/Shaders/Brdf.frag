//#version 330
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

#define PI 3.1415926535897932384626433832795

vec3 diffuseLambert(in vec3 diffuseColor)
{
    return vec3(diffuseColor / PI);
}

float distributionBeckmann(in float normalDotHalf, in float roughness)
{
    float roughnessSq = roughness * roughness;
    float normalDotHalfSq = normalDotHalf * normalDotHalf;
    float roughness1 = 1.0f / (PI * roughnessSq * normalDotHalfSq * normalDotHalfSq);
    float roughness2 = exp((normalDotHalfSq - 1.0f) / (roughnessSq * normalDotHalfSq));
    return roughness1 * roughness2;
}

float geometryCookTorrance(in float normalDotHalf, in float normalDotView, in float normalDotLight, in float viewDotHalf)
{
    float normalDotHalf2 = 2.0f * normalDotHalf;
    float geo1 = (normalDotHalf2 * normalDotView) / viewDotHalf;
    float geo2 = (normalDotHalf2 * normalDotLight) / viewDotHalf;
    return min(1.0f, min(geo1, geo2));
}

float fresnellSchlick(in float viewDotHalf, in float reflectance)
{
    return reflectance + (1.0f - reflectance) * pow(1.0f - viewDotHalf, 5.0f);
}

float cookTorrance(in float normalDotHalf, in float viewDotHalf, in float normalDotView, in float normalDotLight, in float surfaceRoughness, in float fresnelReflectance)
{
    float geo = geometryCookTorrance(normalDotHalf, normalDotView, normalDotLight, viewDotHalf);
    float distribution = distributionBeckmann(normalDotHalf, surfaceRoughness);
    float fresnel = fresnellSchlick(viewDotHalf, fresnelReflectance);

    //Compute the final term
    return max(0.0f, ((distribution * fresnel *geo) / (4 * (normalDotLight * normalDotView))));
}

vec3 calculateBrdf(in vec3 lightColor, in vec3 matDiffuse, in vec3 matSpecular, in vec3 normal, in vec3 viewDir, in vec3 lightDir, in float surfaceRoughness, in float fresnelReflectance)
{
    //Compute intermediary values
    vec3  halfVec = normalize(lightDir + viewDir);
    float normalDotHalf = clamp(dot(normal, halfVec), 0.0f, 1.0f);
    float viewDotHalf = clamp(dot(viewDir, halfVec), 0.0f, 1.0f);
    float normalDotView = clamp(dot(normal, viewDir), 0.0f, 1.0f); 
    float normalDotLight = clamp(dot(normal, lightDir), 0.0f, 1.0f);

    vec3 specular = matSpecular * cookTorrance(normalDotHalf, viewDotHalf, normalDotView, normalDotLight, surfaceRoughness, fresnelReflectance);
    return lightColor * (max(0.0f, normalDotLight ) * (/*diffuseLambert(matDiffuse)*/matDiffuse + specular));
}