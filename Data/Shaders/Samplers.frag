//Material Textures.
uniform sampler2D u_diffuseTexture;
uniform sampler2D u_specularTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_alphaTexture;

//G-buffer
uniform sampler2D u_diffuseBuffer;
uniform sampler2D u_specularBuffer;
uniform sampler2D u_normalBuffer;

//
uniform sampler2DArray u_shadowDepth;

//Texture which includes light id's for each tile.
uniform isampler2D u_tileLightInfo;

//Texture which includes the shadow occlusion for 32 lights is accessed usign the light's shadow occlusion mask. 
uniform sampler2D u_shadowOcclusion;

uniform sampler2DArray u_ssao;

//Texture which includes the result from lighting calculations.
uniform sampler2D u_lightingTexture;

uniform samplerCube u_skyBox;