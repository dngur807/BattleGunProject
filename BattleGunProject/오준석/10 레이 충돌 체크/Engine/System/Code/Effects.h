#ifndef Effects_h__
#define Effects_h__

#include "Engine_Include.h"
#include "LightHelper.h"

#pragma region Effect
BEGIN(Engine)

enum EffectTech
{
	STANDARD_TECH,	// Standard tech.
	NMAP_TECH,		// Normal map tech.
	SMAP_TECH		// Shadow map tech.
};

class ENGINE_DLL CEffect
{
public:
	CEffect(MYGDI* pMyGDI, ID3DX11Effect** ppFX,
		const std::wstring& wstrFileName);
	virtual ~CEffect(void);

private:
	CEffect(const CEffect& rhs);
	CEffect& operator=(const CEffect& rhs);

public:
	//! Applies the effect to the render pipeline.
	void Apply(ID3D11DeviceContext* pContext, EffectTech tech);

protected:
	MYGDI*			m_pMyGDI;
	ID3DX11Effect* m_pFX;
};

END
#pragma endregion

#pragma region BasicEffect

BEGIN(Engine)

class ENGINE_DLL CBasicEffect : public CEffect
{
public:
	CBasicEffect(MYGDI* pMyGDI, ID3DX11Effect** ppFX, const std::wstring& wstrFileName);
	~CBasicEffect(void);

	XMFLOAT4X4 GetWorld() { return m_matWorld; }
	XMFLOAT4X4 GetView() { return m_matView; }
	XMFLOAT4X4 GetProj() { return m_matProj; }

	void SetWorld(DirectX::CXMMATRIX M);
	void SetView(DirectX::CXMMATRIX M);
	void SetProj(DirectX::CXMMATRIX M);
	void SetWorldInvTranspose(DirectX::CXMMATRIX M);
	void SetBoneTransforms(const XMFLOAT4X4* M, int cnt);
	void SetTexTransform(DirectX::CXMMATRIX M);
	void SetEyePosW(const DirectX::XMFLOAT3& v);
	void SetDirLights(const DirectionalLight* lights);
	void SetMaterial(const Material& mat);
	void SetDiffuseMap(ID3D11ShaderResourceView* tex);



	//// 다이나믹 매쉬 관련 추가사항들
	void SetUseLighting(bool use) { mfxUseLighting->SetBool(use); }
	void SetUseTexture(bool use) { UseTexture->SetBool(use); }
	void SetUseAnimation(bool use) { mfxUseAnimation->SetBool(use); }
	void SetUseNormalMap(bool use);
	void SetTexture(TEX2D* texture);
	void SetNormalMap(TEX2D* texture);
	//
	//Interplation to Change Animation Scene
	void SetWeightFactor(float fWeight) { mfxWeightFactor->SetFloat(fWeight); }
	void SetChangeScene(bool bChange) { mfxChangeScene->SetBool(bChange); }
	void SetPreBoneTransforms(const XMFLOAT4X4* M, int cnt);
	void SetPreWorld(const CXMMATRIX M);
	////

	void Apply(ID3D11DeviceContext* pContext, EffectTech tech);
	ID3DX11EffectTechnique* GetSkinnedTech(void);
	ID3DX11EffectTechnique* GetCubeTech(void);
	ID3DX11EffectTechnique* GetColorTech(void);
	////
	
	ID3DX11EffectTechnique* mfxCubeTech;
	ID3DX11EffectTechnique* mfxColorTech;

	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;

	ID3DX11EffectTechnique* Light0TexTech;
	ID3DX11EffectTechnique* Light1TexTech;
	ID3DX11EffectTechnique* Light2TexTech;
	ID3DX11EffectTechnique* Light3TexTech;


	// 내가 추가 
	XMFLOAT4X4					m_matWorld;
	XMFLOAT4X4					m_matView;
	XMFLOAT4X4					m_matProj;

	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* View;
	ID3DX11EffectMatrixVariable* Proj;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* BoneTransforms;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectScalarVariable* UseTexture; 
	ID3DX11EffectScalarVariable* mfxUseNormalMap;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectMatrixVariable* mfxTexTransform;
	ID3DX11EffectMatrixVariable* mfxShadowTransform;

	ID3DX11EffectVariable* mfxLights;
	ID3DX11EffectVariable* mfxMaterial;
	ID3DX11EffectVariable* mfxNumLights;
	ID3DX11EffectScalarVariable* mfxUseLighting;
	ID3DX11EffectShaderResourceVariable* mfxTexture;
	ID3DX11EffectShaderResourceVariable* mfxNormalMap;
	ID3DX11EffectShaderResourceVariable* mfxShadowMap;
	ID3DX11EffectScalarVariable* mfxRenderingToShadowMap;

	// Fog.
	ID3DX11EffectScalarVariable* mfxFogStart;
	ID3DX11EffectScalarVariable* mfxFogRange;
	ID3DX11EffectVectorVariable* mfxFogColor;

	// Skinning.
	ID3DX11EffectTechnique*		 mfxSkinnedTech;
	ID3DX11EffectMatrixVariable* mfxBoneTransforms;
	ID3DX11EffectScalarVariable* mfxUseAnimation;
	//Interplation to Change Animation Scene
	ID3DX11EffectMatrixVariable* mfxPreWorld;
	ID3DX11EffectScalarVariable* mfxWeightFactor;
	ID3DX11EffectScalarVariable* mfxChangeScene;
	ID3DX11EffectMatrixVariable* mfxPreBoneTransforms;

	ID3DX11EffectTechnique*		 mfxNormalMapTech;
	ID3DX11EffectTechnique*		 mfxShadowMapTech;
};
END
#pragma endregion

#pragma region Effects

BEGIN(Engine)

class ENGINE_DLL CEffects
{
	DECLARE_SINGLETON(CEffects)

private:
	CEffects();
public:
	~CEffects();

public:
	void InitAll(MYGDI* pMyGDI, ID3DX11Effect** ppFX);
	CBasicEffect* GetBasicFX(void) { return BasicFX; }
	//void DestroyAll();

	CBasicEffect* BasicFX;
};

END

#endif