#include "LightHelper.fx"

cbuffer cbPerFrame
{
	DirectionalLight gDirLights[3]; // 
	float3	gEyePosW;				// 광원 위치?
	float	gFogStart;				// 
	float	gFogRange;				//
	float4	gFogColor;				//
	float4x4	gView;
	float4x4	gProj;

	// Lights & num lights.
	Light	gLights[10];
	float	gNumLights;

	float3 gAccelW = { -1.0f, -9.8f, 0.0f };
	float gTimeStep;
	float gGameTime;
	float3 gEmitPosW;
	float4x4 gViewProj;

	float gPlayerAngleY;
};
Texture1D gRandomTex;
cbuffer cbPerObject
{
	float4x4	gWorld;
	float4x4	gPreWorld;			// Static Mesh의 Interpolation을 위한 이전 월드행렬
	float4x4	gWorldInvTranspose;
	float4x4	gTexTransform;
	float4x4 gShadowTransform;
	Material	gMaterial;
	Material gMaterial1;
	float gWeightFactor;			// 씬 전환 시 가중치
	bool gChangeScene;				// true == SceneChange
	bool gUseTexture;
	bool gUseNormalMap;
	bool gUseAnimation;
	bool gRenderingToShadowMap;
	bool gUseLighting;
};

cbuffer cbSkinned
{
	float4x4 gBoneTransforms[62];
	float4x4 gPreBoneTransforms[62];
};

// Nonnumeric values cannot be added to aa cbuffer
// : 수치가 아닌 객체는 cbuffer에 추가할 수 없다.
Texture2D gDiffuseMap;
Texture2D gTexture;
Texture2D gNormalMap;
Texture2D gShadowMap;

// 비등방 필터링
SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState samAnisotropic1
{
	Filter = MIN_MAG_MIP_LINEAR;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};


//! The sampler comparison states that is used when compering Z values between the shadow map and ShadowPosH.
SamplerComparisonState samShadow
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComparisonFunc = LESS;
};

//! The sampler state to use with the normal map.
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL		: POSITION;
	float3 NormalL	: NORMAL;
	float2 Tex		: TEXCOORD;
};

struct SkinnedVertexIn
{
	float3 PosL       : POSITION;
	float3 NormalL    : NORMAL;
	float2 Tex        : TEXCOORD;
	float4 TangentL   : TANGENT;
	float4 WeightsA    : WEIGHTSA;
	float4 WeightsB    : WEIGHTSB;
	uint4 BoneIndicesA : BONEINDICESA;
	uint4 BoneIndicesB : BONEINDICESB;
};


struct CubeVertexIn
{
	float3 PosL       : POSITION;
	float3 NormalL    : NORMAL;
	float3 Tex        : TEXCOORD;
};

struct ColorVertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
	//float3 NormalL : NORMAL;
};

cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

struct BoxIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

struct BoxOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
};

float3 RandVec3(float offset)
{
	// Use game time plus offset to sample random texture.
	float u = (gGameTime + offset);

	// coordinates in [-1,1]
	float3 v = gRandomTex.SampleLevel(samLinear, u, 0).xyz;

	return v;
}


BoxOut BOXVS(BoxIn vin)
{
	BoxOut vout;

	// Transform to homogeneous clip space.
	//vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	float4x4 matViewProj = mul(gView, gProj);
	vout.PosH = mul(float4(vin.PosL, 1.0f), matViewProj);

	// Just pass vertex color into the pixel shader.
	vout.Color = vin.Color;

	return vout;
}

float4 BOXPS(BoxOut pin) : SV_Target
{
	return pin.Color;
}

//cbuffer cbPerFrame1
//{
//	DirectionalLight gDirLight;
//	PointLight gPointLight;
//	SpotLight gSpotLight;
//	//float3 gEyePosW;
//};

//cbuffer cbPerObject1
//{
//	//float4x4 gWorld;
//	//float4x4 gWorldInvTranspose;
//	//float4x4 gWorldViewProj;
//	Material gMaterial1;
//};
struct WavesIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
};

struct WavesOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD;
};

WavesOut WAVESVS(WavesIn vin)
{
	WavesOut vout;

	// Transform to world space space.
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	// Transform to homogeneous clip space.

	//vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	//vin.PosL.x -= 70;
	//vin.PosL.z -= 100;


	float4x4 matWorldView = mul(gWorld, gView);
	float4x4 matViewProj = mul(matWorldView, gProj);
	vout.PosH = mul(float4(vin.PosL, 1.0f), matViewProj);

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;


	return vout;
}

float4 WAVESPS(WavesOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform bool gAlphaClip, uniform bool gFogEnabled) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

float3 toEyeW = normalize(gEyePosW - pin.PosW);

float distToEye = length(gEyePosW - pin.PosW);

float4 texColor = float4(1, 1, 1, 1);
if (gUseTexure)
{
	// Sample texture.
	texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
	if (gAlphaClip)
	{
		// Discard pixel if texture alpha < 0.1.  Note that we do this
		// test as soon as possible so that we can potentially exit the shader 
		// early, thereby skipping the rest of the shader code.
		clip(texColor.a - 0.1f);
	}
}
float4 litColor = texColor;

// Start with a sum of zero. 
float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

// Sum the light contribution from each light source.
float4 A, D, S;

ComputeDirectionalLight(gMaterial1, gDirLights[0], pin.NormalW, toEyeW, A, D, S);
ambient += A;
diffuse += D;
spec += S;


if (gFogEnabled)
{
	float fogLerp = saturate((distToEye - gFogStart) / gFogRange);

	// Blend the fog color and the lit color.
	litColor = lerp(litColor, gFogColor, fogLerp);
}

//litColor = texColor*(ambient + diffuse) + spec;
litColor.a = gMaterial.Diffuse.a * texColor.a;

return litColor;
}


//! Output by the vertex shader -> input for the pixel shader.
struct VertexOut
{
	float4 PosH		: SV_POSITION;
	float3 PosW		: POSITION;
	float3 NormalW	: NORMAL;
	float4 TangentW : TANGENT;
	float2 Tex		: TEXCOORD;
	float4 ShadowPosH : TEXCOORD1;
};

struct CubeVertexOut
{
	float4 PosH		: SV_POSITION;
	float3 PosW		: POSITION;
	float3 NormalW	: NORMAL;
	float3 Tex		: TEXCOORD;
};

struct ColorVertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
	float3 NormalW : NORMAL;
};

//! Output for the shadow map vertex shader.
struct VertexOutShadowMap
{
	float4 PosH : SV_POSITION;
};

#define PT_EMITTER 0
#define PT_FLARE 1

struct Particle
{
	float3 InitialPosW : POSITION;
	float3 InitialVelW : VELOCITY;
	float2 SizeW       : SIZE;
	float Age : AGE;
	uint Type          : TYPE;
};

// Array of textures for texturing the particles.
Texture2DArray gTexArray;
struct VertexOutR
{
	float3 PosW  : POSITION;
	uint   Type  : TYPE;
};
struct GeoOut
{
	float4 PosH  : SV_Position;
	float2 Tex   : TEXCOORD;
};

VertexOutR DrawVS(Particle vin)
{
	VertexOutR vout;

	float t = vin.Age;

	// constant acceleration equation
	vout.PosW = 0.5f*t*t*gAccelW + t*vin.InitialVelW + vin.InitialPosW;

	vout.Type = vin.Type;

	return vout;
}

float4 DrawPS(GeoOut pin) : SV_TARGET
{
	return gTexArray.Sample(samLinear, float3(pin.Tex, 0));
}



DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

DepthStencilState NoDepthWrites
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

// The draw GS just expands points into lines.
[maxvertexcount(2)]
void DrawGS(point VertexOutR gin[1],
	inout LineStream<GeoOut> lineStream)
{

	float4x4 matWorldView = mul(gWorld, gView);
	float4x4 matViewProj = mul(matWorldView, gProj);
	//vout.PosH = mul(float4(vin.PosL, 1.0f), matViewProj);

	// Output vertex attributes for interpolation across triangle.
	//vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	// do not draw emitter particles.
	if (gin[0].Type != PT_EMITTER)
	{
		// Slant line in acceleration direction.
		float3 p0 = gin[0].PosW;
		float3 p1 = gin[0].PosW + 0.07f*gAccelW;

		GeoOut v0;
		v0.PosH = mul(float4(p0, 1.0f), matWorldView);
		v0.Tex = float2(0.0f, 0.0f);
		lineStream.Append(v0);

		GeoOut v1;
		v1.PosH = mul(float4(p1, 1.0f), matWorldView);
		v1.Tex = float2(1.0f, 1.0f);
		lineStream.Append(v1);
	}
}


Particle StreamOutVS(Particle vin)
{
	return vin;
}

[maxvertexcount(6)]
void StreamOutGS(point Particle gin[1],
	inout PointStream<Particle> ptStream)
{
	gin[0].Age += gTimeStep;

	if (gin[0].Type == PT_EMITTER)
	{
		// time to emit a new particle?
		if (gin[0].Age > 0.002f)
		{
			for (int i = 0; i < 5; ++i)
			{
				// Spread rain drops out above the camera.
				float3 vRandom = 35.0f*RandVec3((float)i / 5.0f);
				vRandom.y = 20.0f;

				Particle p;
				p.InitialPosW = gEmitPosW.xyz + vRandom;
				p.InitialVelW = float3(0.0f, 0.0f, 0.0f);
				p.SizeW = float2(1.0f, 1.0f);
				p.Age = 0.0f;
				p.Type = PT_FLARE;

				ptStream.Append(p);
			}

			// reset the time to emit
			gin[0].Age = 0.0f;
		}

		// always keep emitters
		ptStream.Append(gin[0]);
	}
	else
	{
		// Specify conditions to keep particle; this may vary from system to system.
		if (gin[0].Age <= 3.0f)
			ptStream.Append(gin[0]);
	}
}

GeometryShader gsStreamOut = ConstructGSWithSO(
	CompileShader(gs_5_0, StreamOutGS()),
	"POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x");



VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	//Transform to homogeneous clip space.
	float4x4 matViewProj = mul(gView, gProj);

	//float3 PosL		: POSITION;
	//float3 NormalL	: NORMAL;
	//float2 Tex		: TEXCOORD;

	if (gChangeScene)
	{	// Static Mesh Interpolation
		float3 posW = float3(0.0f, 0.0f, 0.0f);
		float3 normalW = float3(0.0f, 0.0f, 0.0f);
		float3 PreposW = float3(0.0f, 0.0f, 0.0f);
		float3 PrenormalW = float3(0.0f, 0.0f, 0.0f);

		//Transform to world space.
		posW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
		PreposW = mul(float4(vin.PosL, 1.0f), gPreWorld).xyz;
		posW *= gWeightFactor;
		PreposW *= (1 - gWeightFactor);
		vout.PosW = posW = posW + PreposW;
		vout.PosH = mul(float4(posW, 1.0f), matViewProj);
		//vout.PosH = mul(float4(posW, 1.0f), gWorldViewProj);

		normalW = mul(float4(vin.NormalL, 1.0f), gWorld).xyz;
		PrenormalW = mul(float4(vin.NormalL, 1.0f), gPreWorld).xyz;
		normalW *= gWeightFactor;
		PrenormalW *= (1 - gWeightFactor);
		vout.NormalW = normalW = normalW + PrenormalW;
	}
	else
	{
		float3 posW = float3(0.0f, 0.0f, 0.0f);
		//Transform to world space.
		vout.PosW = posW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
		vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
		//vout.TangentW = mul(vin.TangentL, gWorld);


		vout.PosH = mul(float4(posW, 1.0f), matViewProj);

		//vout.PosH = float4(posW, 1.0f);
	}



	//Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}

CubeVertexOut CubeVS(CubeVertexIn vin)
{
	CubeVertexOut vout;

	//Transform to world space.
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	//vout.TangentW = mul(vin.TangentL, gWorld);

	//Transform to homogeneous clip space.
	float4x4 matWorldView = mul(gWorld, gView);
	float4x4 matWorldViewProj = mul(matWorldView, gProj);
	vout.PosH = mul(float4(vin.PosL, 1.0f), matWorldViewProj);

	//Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 1.0f), gTexTransform).xyz;

	return vout;
}


float4 CubePS(CubeVertexOut pin, uniform int gLightCount) : SV_Target
{
	//Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

// The toEye vertor is used in lighting.
float3 toEye = gEyePosW - pin.PosW;

// Cache the distance to the eye from this surface point
// : 시점과 이 표면 점 사이의 거리를 보관해둔다.
float distToEye = length(toEye);

//Normalize toEye
toEye /= distToEye;

// Default to multiplicative iendtity.
// : 기본 텍스처 색상은 곱셈의 항등원??
float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

//Sample texture.
texColor = gDiffuseMap.Sample(samAnisotropic1, pin.Tex);


// Discard pixel if texture alpha < 0.1.  Note that we do this
// test as soon as possible so that we can potentially exit the shader 
// early, thereby skipping the rest of the shader code.
//clip(texColor.a - 0.1f);


//## Lighting

float4 litColor = texColor;
//if (gLightCount > 0)
{
	// Start with a sum of zero.
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	[unroll]
	for (int i = 0; i < gLightCount; ++i)
	{
		float4 A, D, S;
		ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye,
			A, D, S);

		ambient += A;
		diffuse += D;
		spec += S;
	}
	// Modulate with late add. : 변조 후 가산
	litColor = texColor * (ambient + diffuse) + spec;
}

//Common to take alpha from diffuse material and texture.
litColor.a = gMaterial.Diffuse.a * texColor.a;

return litColor;
}

ColorVertexOut ColorVS(ColorVertexIn vin)
{
	ColorVertexOut vout;

	//Transform to homogeneous clip space.
	float4x4 matViewProj = mul(gView, gProj);

	if (gChangeScene)
	{	// Static Mesh Interpolation
		float3 posW = float3(0.0f, 0.0f, 0.0f);
		float3 PreposW = float3(0.0f, 0.0f, 0.0f);

		//Transform to world space.
		posW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
		PreposW = mul(float4(vin.PosL, 1.0f), gPreWorld).xyz;
		posW *= gWeightFactor;
		PreposW *= (1 - gWeightFactor);
		posW = posW + PreposW;

		vout.PosH = mul(float4(posW, 1.0f), matViewProj);
	}
	else
	{
		float3 posW = float3(0.0f, 0.0f, 0.0f);
		//Transform to world space.
		posW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
		vout.PosH = mul(float4(posW, 1.0f), matViewProj);
	}

	// Just pass vertex color into the pixel shader.
	vout.Color = vin.Color;

	return vout;
}

//! Vertex shader that transforms coordinates and normals to the world and homogeneous space.
VertexOut SkinnedVS(SkinnedVertexIn vin)
{
	VertexOut vout;

	if (gUseAnimation)
	{
		// Init array or else we get strange warnings about SV_POSITION.
		float weightsA[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		weightsA[0] = vin.WeightsA.x;
		weightsA[1] = vin.WeightsA.y;
		weightsA[2] = vin.WeightsA.z;
		weightsA[3] = vin.WeightsA.w;

		float weightsB[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		weightsB[0] = vin.WeightsB.x;
		weightsB[1] = vin.WeightsB.y;
		weightsB[2] = vin.WeightsB.z;
		weightsB[3] = vin.WeightsB.w;

		float3 posL = float3(0.0f, 0.0f, 0.0f);
		float3 normalL = float3(0.0f, 0.0f, 0.0f);
		float3 tangentL = float3(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < 4; ++i)
		{
			// Assume no nonuniform scaling when transforming normals, so 
			// that we do not have to use the inverse-transpose.
			//posL += weights[i] * vin.PosL;
			posL += weightsA[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndicesA[i]]).xyz;
			normalL += weightsA[i] * mul(vin.NormalL, (float3x3)gBoneTransforms[vin.BoneIndicesA[i]]);
			tangentL += weightsA[i] * mul(vin.TangentL.xyz, (float3x3)gBoneTransforms[vin.BoneIndicesA[i]]);

			posL += weightsB[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndicesB[i]]).xyz;
			normalL += weightsB[i] * mul(vin.NormalL, (float3x3)gBoneTransforms[vin.BoneIndicesB[i]]);
			tangentL += weightsB[i] * mul(vin.TangentL.xyz, (float3x3)gBoneTransforms[vin.BoneIndicesB[i]]);
		}

		if (gChangeScene)
		{
			posL *= gWeightFactor;
			normalL *= gWeightFactor;
			tangentL *= gWeightFactor;

			float3 posLip = float3(0.0f, 0.0f, 0.0f);
			float3 normalLip = float3(0.0f, 0.0f, 0.0f);
			float3 tangentLip = float3(0.0f, 0.0f, 0.0f);

			for (int i = 0; i < 4; ++i)
			{
				// Assume no nonuniform scaling when transforming normals, so 
				// that we do not have to use the inverse-transpose.
				//posL += weights[i] * vin.PosL;
				posLip += weightsA[i] * mul(float4(vin.PosL, 1.0f), gPreBoneTransforms[vin.BoneIndicesA[i]]).xyz;
				normalLip += weightsA[i] * mul(vin.NormalL, (float3x3)gPreBoneTransforms[vin.BoneIndicesA[i]]);
				tangentLip += weightsA[i] * mul(vin.TangentL.xyz, (float3x3)gPreBoneTransforms[vin.BoneIndicesA[i]]);

				posLip += weightsB[i] * mul(float4(vin.PosL, 1.0f), gPreBoneTransforms[vin.BoneIndicesB[i]]).xyz;
				normalLip += weightsB[i] * mul(vin.NormalL, (float3x3)gPreBoneTransforms[vin.BoneIndicesB[i]]);
				tangentLip += weightsB[i] * mul(vin.TangentL.xyz, (float3x3)gPreBoneTransforms[vin.BoneIndicesB[i]]);
			}

			posLip *= (1 - gWeightFactor);
			normalLip *= (1 - gWeightFactor);
			tangentLip *= (1 - gWeightFactor);

			posL += posLip;
			normalL += normalLip;
			tangentL += tangentLip;
		}

		// Transform to world space space.
		vout.PosW = mul(float4(posL, 1.0f), gWorld).xyz;
		vout.NormalW = mul(normalL, (float3x3)gWorldInvTranspose);
		vout.TangentW = float4(mul(tangentL, (float3x3)gWorld), vin.TangentL.w);

		// Transform to homogeneous clip space.
		float4x4 matWorldView = mul(gWorld, gView);
		float4x4 matWorldViewProj = mul(matWorldView, gProj);
		vout.PosH = mul(float4(posL, 1.0f), matWorldViewProj);

		// Pass on the texture.
		vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

		// Generate projective tex-coords to project shadow map onto scene.
		vout.ShadowPosH = mul(float4(posL, 1.0f), gShadowTransform);

		return vout;
	}
	else
	{
		// Transform to world space space.
		vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
		vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
		vout.TangentW = mul(vin.TangentL, gWorld);

		// Transform to homogeneous clip space.
		float4x4 matWorldView = mul(gWorld, gView);
		float4x4 matWorldViewProj = mul(matWorldView, gProj);
		vout.PosH = mul(float4(vin.PosL, 1.0f), matWorldViewProj);

		// Pass on the texture.
		vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

		// Generate projective tex-coords to project shadow map onto scene.
		vout.ShadowPosH = mul(float4(vin.PosL, 1.0f), gShadowTransform);

		return vout;
	}
}

//! Vertex shader that transforms coordinates and normals to the world and homogeneous space.
VertexOutShadowMap ShadowVS(SkinnedVertexIn vin)
{
	VertexOutShadowMap vout;

	if (gUseAnimation)
	{
		// Init array or else we get strange warnings about SV_POSITION.
		float weightsA[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		weightsA[0] = vin.WeightsA.x;
		weightsA[1] = vin.WeightsA.y;
		weightsA[2] = vin.WeightsA.z;
		weightsA[3] = vin.WeightsA.w;

		float3 posL = float3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < 4; ++i)
			posL += weightsA[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndicesA[i]]).xyz;

		// Transform to homogeneous clip space.
		float4x4 matWorldView = mul(gWorld, gView);
		float4x4 matWorldViewProj = mul(matWorldView, gProj);
		vout.PosH = mul(float4(posL, 1.0f), matWorldViewProj);
	}
	else
	{
		float4x4 matWorldView = mul(gWorld, gView);
		float4x4 matWorldViewProj = mul(matWorldView, gProj);
		vout.PosH = mul(float4(vin.PosL, 1.0f), matWorldViewProj);
	}
	return vout;
}

float4 ColorPS(ColorVertexOut pin) : SV_Target
{
	return pin.Color;
}

float4 PS(VertexOut pin, uniform int gLightCount, uniform bool gUseTexture) : SV_Target
{
	//Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

// The toEye vertor is used in lighting.
float3 toEye = gEyePosW - pin.PosW;

// Cache the distance to the eye from this surface point
// : 시점과 이 표면 점 사이의 거리를 보관해둔다.
float distToEye = length(toEye);

//Normalize toEye
toEye /= distToEye;

// Default to multiplicative iendtity.
// : 기본 텍스처 색상은 곱셈의 항등원??
float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
if (gUseTexture)
{
	//Sample texture.
	texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
}

//## Lighting
clip(texColor.a - 0.05f);

float4 litColor = texColor;
if (gLightCount > 0)
{
	// Start with a sum of zero.
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	[unroll]
	for (int i = 0; i < gLightCount; ++i)
	{
		float4 A, D, S;
		ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye,
			A, D, S);

		ambient += A; //조명광
		diffuse += D; //난반사
		spec += S; //정반사
	}
	// Modulate with late add. : 변조 후 가산
	//if (gLightCount != 2)
	litColor = texColor * (ambient + diffuse) + spec;
}

//Common to take alpha from diffuse material and texture.
//if (gLightCount != 2)
litColor.a = gMaterial.Diffuse.a * texColor.a;


//if (gLightCount == 2)
//{
//	litColor.r = 0.f;
//	litColor.g = 0.f;
//	litColor.b = 0.f;
//}

//if (gLightCount == 2)
//{
//	//if (litColor.r + litColor.g + litColor.b <= 0.1)
//	litColor.a = litColor.r;
//}



return litColor;
}


float4 SkinnedPS(VertexOut pin) : SV_Target
{
	/*if (gRenderingToShadowMap)
	return float4(1.0f, 1.0f, 1.0f, 1.0f);*/

	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

float3 toEyeW = normalize(gEyePosW - pin.PosW);

// Identity as default.
float4 texColor = float4(1, 1, 1, 1);
if (gUseTexture)
texColor = gTexture.Sample(samAnisotropic, pin.Tex);

// Get the shadow factor.
float shadow = 1.0f;
shadow = CalcShadowFactor(samShadow, gShadowMap, pin.ShadowPosH);

// Apply lighting.
float4 litColor;
if (gUseLighting)
ApplyLighting(gNumLights, gLights, gMaterial, pin.PosW, pin.NormalW, toEyeW, texColor, shadow, litColor);
else
litColor = texColor*(gMaterial.Ambient + gMaterial.Diffuse) + gMaterial.Specular;

////! Apply fogging.
//float distToEye = length(gEyePosW - pin.PosW);
//float fogLerp = saturate((distToEye - gFogStart) / gFogRange);

//// Blend the fog color and the lit color.
//litColor = lerp(litColor, gFogColor, fogLerp);

// Common to take alpha from diffuse material.
litColor.a = gMaterial.Diffuse.a * texColor.a;

return litColor;
}

//## fx에서 Render State 설정 가능
RasterizerState WireframeRS
{
	FillMode = Wireframe;
	CullMode = None;
	FrontCounterClockWise = false;
	//## 명시하지 않은 값들은 default값 사용
};

RasterizerState SolidRS
{
	FillMode = Solid;
	CullMode = Back;
	FrontCounterClockWise = false;
	//## 명시하지 않은 값들은 default값 사용
};

VertexOut VSUI(VertexIn vin)
{
	VertexOut vout;

	float3 posW = float3(0.0f, 0.0f, 0.0f);
	vout.PosW = posW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.PosH = float4(posW, 1.0f);


	//Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	//vout.Tex = mul(float4(vin.Tex.x, vin.Tex.y , 0.0f , 1.0f), gTexTransform).xy;

	return vout;
}


VertexOut VSUINAVI(VertexIn vin)
{
	VertexOut vout;

	float3 posW = float3(0.0f, 0.0f, 0.0f);

	vout.PosW = posW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.PosH = float4(posW, 1.0f);


	//Output vertex attributes for interpolation across triangle.
	//vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	vout.Tex = mul(float4(vin.Tex.x + gPlayerAngleY, vin.Tex.y, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}



float4 PSUI(VertexOut pin, uniform int gLightCount, uniform bool gUseTexture) : SV_Target
{
	//Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

// The toEye vertor is used in lighting.
float3 toEye = gEyePosW - pin.PosW;

// Cache the distance to the eye from this surface point
// : 시점과 이 표면 점 사이의 거리를 보관해둔다.
float distToEye = length(toEye);

//Normalize toEye
toEye /= distToEye;

// Default to multiplicative iendtity.
// : 기본 텍스처 색상은 곱셈의 항등원??
float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
if (gUseTexture)
{
	//Sample texture.
	//if(pin.Tex.x>0.3 && pin.Tex.x<0.7f)
	texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
}

//## Lighting
clip(texColor.a - 0.01f);

float4 litColor = texColor;
if (gLightCount > 0)
{
	// Start with a sum of zero.
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	[unroll]
	for (int i = 0; i < gLightCount; ++i)
	{
		float4 A, D, S;
		ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye,
			A, D, S);

		ambient += A;
		diffuse += D;
		spec += S;
	}
	// Modulate with late add. : 변조 후 가산
	litColor = texColor * (ambient + diffuse) + spec;
}

litColor.a = gMaterial.Diffuse.a * texColor.a;



return litColor;
}

technique11 ColorTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, ColorVS()));
		SetGeometryShader(NULL);
		SetRasterizerState(WireframeRS);
		SetPixelShader(CompileShader(ps_5_0, ColorPS()));
		SetRasterizerState(SolidRS);
	}
}
//! Skinned mesh technique.
technique11 SkinnedTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, true)));
	}
}

technique11 StreamOutTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, StreamOutVS()));
		SetGeometryShader(gsStreamOut);

		// disable pixel shader for stream-out only
		SetPixelShader(NULL);

		// we must also disable the depth buffer for stream-out only
		SetDepthStencilState(DisableDepth, 0);
	}
}


technique11 RainTech
{
	pass P0
	{
		//SetVertexShader(CompileShader(vs_5_0, RainTechVS()));
		//SetGeometryShader(NULL);
		//SetPixelShader(CompileShader(ps_5_0, RainTechPS(1, true, true, true)));

		SetVertexShader(CompileShader(vs_5_0, DrawVS()));
		SetGeometryShader(CompileShader(gs_5_0, DrawGS()));
		SetPixelShader(CompileShader(ps_5_0, DrawPS()));

		SetDepthStencilState(NoDepthWrites, 0);
	}
}


technique11 WavesTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, WAVESVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, WAVESPS(1, true, true, true)));
	}
}

technique11 BoxTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, BOXVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, BOXPS()));
	}
}

technique11 CubeTech
{
	pass P0
	{
		//cullmode = none;
		SetVertexShader(CompileShader(vs_5_0, CubeVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, CubePS(1)));
	}
}

technique11 Light1
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, false)));
	}
}

technique11 Light2
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(2, false)));
	}
}

technique11 Light3
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(3, false)));
	}
}

technique11 Light0Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(0, true)));
	}
}

technique11 Light1Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, true)));
	}
}

technique11 Light2Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(2, true)));
	}
}

technique11 Light3Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(3, true)));
	}
}

technique11 UITech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSUI()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSUI(0, true)));
		SetDepthStencilState(DisableDepth, 0);//Z버퍼 끄기   NoDepthWrites 키기
	}
}

technique11 UINaviTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSUINAVI()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSUI(0, true)));
	}
}


technique11 UIWeaponTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSUI()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSUI(1, true)));
	}
}


