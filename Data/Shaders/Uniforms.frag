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
    vec2 u_renderTargetSize;
};

layout(std140) uniform u_postProcessParameters
{
    vec4 u_SAOParameters; //projScale, radius, bias, intensity
    vec2 u_postRenderTargetSize;
};

uniform int u_shadowOcclusionParameterIndex;
uniform int u_materialParameterIndex;