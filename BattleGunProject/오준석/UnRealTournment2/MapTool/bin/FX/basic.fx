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
};

cbuffer cbPerObject
{
	float4x4	gWorld;
	float4x4	gPreWorld;			// Static Mesh의 Interpolation을 위한 이전 월드행렬
	float4x4	gWorldInvTranspose;
	float4x4	gTexTransform;
	float4x4 gShadowTransform;
	Material	gMaterial;
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
	MaxAnisotropy = 1;

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

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	//Transform to homogeneous clip space.
	float4x4 matViewProj = mul(gView, gProj);

	if (gChangeScene)
	{	// Static Mesh Interpolation
		float3 posW		= float3(0.0f, 0.0f, 0.0f);
		float3 normalW = float3(0.0f, 0.0f, 0.0f);
		float3 PreposW	= float3(0.0f, 0.0f, 0.0f);
		float3 PrenormalW = float3(0.0f, 0.0f, 0.0f);

		//Transform to world space.
		posW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
		PreposW = mul(float4(vin.PosL, 1.0f), gPreWorld).xyz;
		posW *= gWeightFactor;
		PreposW *= (1 - gWeightFactor);
		vout.PosW = posW = posW + PreposW;
		vout.PosH = mul(float4(posW, 1.0f), matViewProj);

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

if (gLightCount == 1)
{
	if (gUseTexture)
	{
		//Sample texture.
		texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
		//texColor = float4(1.0f, 1.0f, 0.f, 0.f);
		//texColor = float4(1.0f, 0.f, 0.f, 0.f);
	}
}
else if (gLightCount == 2)
{
	if (gUseTexture)
	{
		//Sample texture.
		texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
		texColor = float4(1.0f, 0.0f, 0.f, 0.f);
	}
}


//## Lighting

	float4 litColor = texColor;
	if (gLightCount ==1)
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
		texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);


	//## Lighting

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

	//Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

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

technique11 CubeTech
{
	pass P0
	{
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
		SetPixelShader(CompileShader(ps_5_0, PS(2, true)));
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
