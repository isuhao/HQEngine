#ifndef COMMON_SHADER_CODE_H
#define COMMON_SHADER_CODE_H

#define PI 3.141592654f

#ifdef HQEXT_CG
#pragma pack_matrix(column_major)

struct vPNIn{
	float3 iPos : VPOSITION;
	float3 iNormal : VNORMAL;
};

//world transformation of position
float3 worldTransformCoords(float3 posL, float4x3 worldMatrix){
	return mul(float4(posL, 1.0), worldMatrix);
}

//world transformation of normal
float3 worldTransformNormal(float3 normalL, float4x3 worldMatrix){
	float3 normalW = mul(float4(normalL, 0.0), worldMatrix);
	return normalize(normalW);
}
//clip space transformation
float4 clipTransformCoords(float3 posW, float4x4 viewMatrix, float4x4 projMatrix)
{
	float4 posV = mul(float4(posW, 1.0), viewMatrix);
	return mul(posV, projMatrix);
}

//clip space transformation
void transformWVP(float3 posL, 
	float4x3 worldMatrix, 
	float4x4 viewMatrix, 
	float4x4 projMatrix,
	out float3 posW,
	out float4 posH)
{

	posW = worldTransformCoords(posL, worldMatrix);
	posH = clipTransformCoords(posW, viewMatrix, projMatrix);
}


//get normalized depth in range [0..1]
float getNormDepth(float2 clipDepth){
	return clipDepth.x / clipDepth.y;
}

//get normalized depth in range [0..1]. OpenGL version 
glslf float getNormDepth(float2 clipDepth){
	return 0.5 * (clipDepth.x / clipDepth.y) + 0.5;
}

//get normalized depth in range [0..1]
float getNormDepth(float4 clipPos){
	return getNormDepth(clipPos.zw);
}

//scale normalized coordinate to a texcoord value between [0..1]
float2 scaleToTexcoord(float2 normalizeCoords){
	float2 scaled;

	scaled.x = normalizeCoords.x * 0.5 + 0.5;
	scaled.y = -normalizeCoords.y * 0.5 + 0.5;

	return scaled;
}

//scale normalized coordinate to a texcoord value between [0..1]. OpenGL version
glslv glslf float2 scaleToTexcoord(float2 normalizeCoords){
	float2 scaled;

	scaled.x = normalizeCoords.x * 0.5 + 0.5;
	scaled.y = normalizeCoords.y * 0.5 + 0.5;

	return scaled;
}

//compute shadow factor
float computeShadowFactor(
	sampler2D depthMap, 
	int depthMapSize,
	float2 depthMapTexcoords, 
	float fragmentDepth,
	float depthBias)
{
	float2 texelPos = depthMapTexcoords * depthMapSize;
	const float dx = 1.0 / depthMapSize;

	//filtering between 4 nearest neighbors
	float2 lerpFactor = frac(texelPos);

	float s0 = (tex2D(depthMap, depthMapTexcoords).r + depthBias) < fragmentDepth ? 0.0 : 1.0;
	float s1 = (tex2D(depthMap, depthMapTexcoords + float2(dx, 0.0)).r + depthBias) < fragmentDepth ? 0.0 : 1.0;
	float s2 = (tex2D(depthMap, depthMapTexcoords + float2(0.0, dx)).r + depthBias) < fragmentDepth ? 0.0 : 1.0;
	float s3 = (tex2D(depthMap, depthMapTexcoords + float2(dx, dx)).r + depthBias) < fragmentDepth ? 0.0 : 1.0;

	float shadowFactor = lerp(
		lerp(s0, s1, lerpFactor.x),
		lerp(s2, s3, lerpFactor.x),
		lerpFactor.y);


	return shadowFactor;
}


//{lightToVert} must be normalized
float calculateSpotLightFactor(float3 lightToVert, float3 lightDirection, float3 lightFalloff_cosHalfAngle_cosHalfTheta)
{
	float cosLD = dot(lightToVert, lightDirection);
	float spot = (max(cosLD, 0) - lightFalloff_cosHalfAngle_cosHalfTheta.y)
		/ (lightFalloff_cosHalfAngle_cosHalfTheta.z - lightFalloff_cosHalfAngle_cosHalfTheta.y);

	return pow(max(spot, 0.0), lightFalloff_cosHalfAngle_cosHalfTheta.x);
}

#elif !defined HQEXT_GLSL && !defined HQEXT_GLSL_ES //D3D
#pragma pack_matrix(column_major)

struct vPNIn{
	float3 iPos : VPOSITION;
	float3 iNormal : VNORMAL;
};

//world transformation of position
float3 worldTransformCoords(float3 posL, float4x3 worldMatrix){
	return mul(float4(posL, 1.0), worldMatrix);
}

//world transformation of normal
float3 worldTransformNormal(float3 normalL, float4x3 worldMatrix){
	float3 normalW = mul(float4(normalL, 0.0), worldMatrix);
	return normalize(normalW);
}
//clip space transformation
float4 clipTransformCoords(float3 posW, float4x4 viewMatrix, float4x4 projMatrix)
{
	float4 posV = mul(float4(posW, 1.0), viewMatrix);
	return mul(posV, projMatrix);
}

//clip space transformation
void transformWVP(float3 posL,
	float4x3 worldMatrix,
	float4x4 viewMatrix,
	float4x4 projMatrix,
	out float3 posW,
	out float4 posH)
{

	posW = worldTransformCoords(posL, worldMatrix);
	posH = clipTransformCoords(posW, viewMatrix, projMatrix);
}


#	if defined USE_GL_WAY
//get normalized depth in range [0..1]. OpenGL version 
float getNormDepth(float2 clipDepth){
	return 0.5 * (clipDepth.x / clipDepth.y) + 0.5;
}
#	else

//get normalized depth in range [0..1]
float getNormDepth(float2 clipDepth){
	return clipDepth.x / clipDepth.y;
}
#	endif


//get normalized depth in range [0..1]
float getNormDepth(float4 clipPos){
	return getNormDepth(clipPos.zw);
}

#	ifdef USE_GL_WAY
//scale normalized coordinate to a texcoord value between [0..1]. OpenGL version
float2 scaleToTexcoord(float2 normalizeCoords){
	float2 scaled;

	scaled.x = normalizeCoords.x * 0.5 + 0.5;
	scaled.y = normalizeCoords.y * 0.5 + 0.5;

	return scaled;
}
#	else
//scale normalized coordinate to a texcoord value between [0..1]
float2 scaleToTexcoord(float2 normalizeCoords){
	float2 scaled;

	scaled.x = normalizeCoords.x * 0.5 + 0.5;
	scaled.y = -normalizeCoords.y * 0.5 + 0.5;

	return scaled;
}
#	endif


//compute shadow factor
float computeShadowFactor(
	Texture2D<float4> depthMap,
	SamplerState samplerState,
	int depthMapSize,
	float2 depthMapTexcoords,
	float fragmentDepth,
	float depthBias)
{
	float2 texelPos = depthMapTexcoords * depthMapSize;
	const float dx = 1.0 / depthMapSize;

	//filtering between 4 nearest neighbors
	float2 lerpFactor = frac(texelPos);

	float s0 = (depthMap.Sample(samplerState, depthMapTexcoords).r + depthBias) < fragmentDepth ? 0.0 : 1.0;
	float s1 = (depthMap.Sample(samplerState, depthMapTexcoords + float2(dx, 0.0)).r + depthBias) < fragmentDepth ? 0.0 : 1.0;
	float s2 = (depthMap.Sample(samplerState, depthMapTexcoords + float2(0.0, dx)).r + depthBias) < fragmentDepth ? 0.0 : 1.0;
	float s3 = (depthMap.Sample(samplerState, depthMapTexcoords + float2(dx, dx)).r + depthBias) < fragmentDepth ? 0.0 : 1.0;

	float shadowFactor = lerp(
		lerp(s0, s1, lerpFactor.x),
		lerp(s2, s3, lerpFactor.x),
		lerpFactor.y);


	return shadowFactor;
}

//compute shadow factor from first layer in depth map
float computeShadowFactor(
	Texture2DArray<float4> depthMap,
	SamplerState samplerState,
	int depthMapSize,
	float2 depthMapTexcoords,
	float fragmentDepth,
	float depthBias)
{
	float2 texelPos = depthMapTexcoords * depthMapSize;
	const float dx = 1.0 / depthMapSize;

	//filtering between 4 nearest neighbors
	float2 lerpFactor = frac(texelPos);

	float s0 = (depthMap.Sample(samplerState, float3(depthMapTexcoords, 0.0)).r + depthBias) < fragmentDepth ? 0.0 : 1.0;
	float s1 = (depthMap.Sample(samplerState, float3(depthMapTexcoords + float2(dx, 0.0), 0.0)).r + depthBias) < fragmentDepth ? 0.0 : 1.0;
	float s2 = (depthMap.Sample(samplerState, float3(depthMapTexcoords + float2(0.0, dx), 0.0)).r + depthBias) < fragmentDepth ? 0.0 : 1.0;
	float s3 = (depthMap.Sample(samplerState, float3(depthMapTexcoords + float2(dx, dx), 0.0)).r + depthBias) < fragmentDepth ? 0.0 : 1.0;

	float shadowFactor = lerp(
		lerp(s0, s1, lerpFactor.x),
		lerp(s2, s3, lerpFactor.x),
		lerpFactor.y);


	return shadowFactor;
}


//{lightToVert} must be normalized
float calculateSpotLightFactor(float3 lightToVert, float3 lightDirection, float3 lightFalloff_cosHalfAngle_cosHalfTheta)
{
	float cosLD = dot(lightToVert, lightDirection);
	if (cosLD > lightFalloff_cosHalfAngle_cosHalfTheta.z)//inside innder cone
		return 1.0;
	float spot = (max(cosLD, 0) - lightFalloff_cosHalfAngle_cosHalfTheta.y)
		/ (lightFalloff_cosHalfAngle_cosHalfTheta.z - lightFalloff_cosHalfAngle_cosHalfTheta.y);

	return pow(max(spot, 0.0), lightFalloff_cosHalfAngle_cosHalfTheta.x);
}

#else //GLSL

#define float4 vec4
#define float4x4 mat4

//TO DO
#endif//#ifdef HQEXT_CG


/*---------specular material -----------------------*/
#if 1
#define SpecularMaterial float4x4

//work around cross platform array of struct in uniform buffer
#	if defined HQEXT_GLSL || defined HQEXT_GLSL_ES
#		define Material_Ambient(_material) ((_material)[0])
#		define Material_Diffuse(_material) ((_material)[1])
#		define Material_Specular(_material) ((_material)[2])
#		define Material_SpecularExp(_material) ((_material)[3].x)
#	else
#		define Material_Ambient(_material) ((_material)._11_21_31_41)
#		define Material_Diffuse(_material) ((_material)._12_22_32_42)
#		define Material_Specular(_material) ((_material)._13_23_33_43)
#		define Material_SpecularExp(_material) ((_material)._14)
#	endif

#else
//can also view SpecularMaterial as

struct SpecularMaterial {
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float specPower;
};

#define Material_Ambient(_material) (_material.ambient)
#define Material_Diffuse(_material) (_material.diffuse)
#define Material_Specular(_material) (_material.specular)
#define Material_SpecularExp(_material) (_material.specPower)

#endif


#endif