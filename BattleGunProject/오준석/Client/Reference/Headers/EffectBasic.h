#pragma once
#include "Component.h"
#include "Engine_Include.h"
#include "LightHelper.h"

BEGIN(Engine)

//enum EffectTech
//{
//	STANDARD_TECH,	// Standard tech.
//	NMAP_TECH,		// Normal map tech.
//	SMAP_TECH		// Shadow map tech.
//};


class ENGINE_DLL CEffectBasic :
	public CComponent
{
protected:
	explicit CEffectBasic(MYGDI* pMyGDI, const std::wstring& wstrFileName);
	explicit CEffectBasic(const CEffectBasic& rhs);
	virtual ~CEffectBasic(void) = default;

protected:
	void CreateFX(const wstring& wstrFileName);
	void InitEffect(void);


public:
	virtual HRESULT Ready_Effect(void) PURE;
	void SetWorld(CXMMATRIX M);
	void SetView(CXMMATRIX M);
	void SetProj(CXMMATRIX M);
	void SetLastWorld(CXMMATRIX M);
	void SetTexTransform(CXMMATRIX M);
	void SetWorldInvTranspose(DirectX::CXMMATRIX M);
	void SetEyePosW(const XMFLOAT3& v);
	void SetMaterial(const Material& mat);
	void SetGrapping(const FLOAT& v);

	void SetTextureMap(ID3D11ShaderResourceView* texture);
	void SetNormalMap(ID3D11ShaderResourceView* normal);
	void SetSpecularMap(ID3D11ShaderResourceView* specular);
	void SetShadowMap(ID3D11ShaderResourceView* shadow);
	void SetDepthMap(ID3D11ShaderResourceView* depth);
	void SetLastMap(ID3D11ShaderResourceView* lastmap);

	void SetSpotLights(const SpotLight* lights);
	void SetFogColor(const FXMVECTOR v) { mfxFogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f) { mfxFogStart->SetFloat(f); }
	void SetFogRange(float f) { mfxFogRange->SetFloat(f); }

	void SetDirLights(const DirectionalLight* lights);
	void SetPlayerAngleY(float fWeight) { m_fxPlayerAngleY->SetFloat(fWeight); }
	void SetPlayerAngleX(float fWeight) { m_fxPlayerAngleX->SetFloat(fWeight); }
	ID3DX11EffectTechnique* GetTech(void);
	void Apply();
	void SetWorldViewProj(CXMMATRIX M) { m_fxWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }

public:
	virtual CEffectBasic* Clone(void) PURE;
protected:
	MYGDI*						m_pMyGDI = nullptr;
	ID3DX11Effect*				m_pFX = nullptr;
	ID3DX11EffectTechnique*		m_fxTech = nullptr;

	ID3DX11EffectScalarVariable* m_fxGrappling = nullptr;

	ID3DX11EffectVectorVariable* m_fxVecEyePosW = nullptr;

	ID3DX11EffectMatrixVariable* m_fxMatWorld = nullptr;
	ID3DX11EffectMatrixVariable* m_fxMatView = nullptr;
	ID3DX11EffectMatrixVariable* m_fxMatProj = nullptr;
	ID3DX11EffectMatrixVariable* m_fxLastWorld = nullptr;
	ID3DX11EffectMatrixVariable* m_fxMatTextureTransform = nullptr;
	ID3DX11EffectMatrixVariable* m_fxMatShadowTransform = nullptr;
	ID3DX11EffectMatrixVariable* m_fxMatWorldInvTranspose = nullptr;

	ID3DX11EffectShaderResourceVariable* m_fxTextureMap = nullptr;
	ID3DX11EffectShaderResourceVariable* m_fxNormalMap = nullptr;
	ID3DX11EffectShaderResourceVariable* m_fxSpecularMap = nullptr;
	ID3DX11EffectShaderResourceVariable* m_fxShadowMap = nullptr;
	ID3DX11EffectShaderResourceVariable* m_fxDepthMap = nullptr;
	ID3DX11EffectShaderResourceVariable* m_fxLastMap = nullptr;

	ID3DX11EffectVariable* m_fxMaterial = nullptr;
	ID3DX11EffectVariable* m_fxDirLights = nullptr;
	ID3DX11EffectVariable* m_fxSpotLights = nullptr;
	ID3DX11EffectScalarVariable* m_fxPlayerAngleY = nullptr;
	ID3DX11EffectScalarVariable* m_fxPlayerAngleX = nullptr;
	ID3DX11EffectMatrixVariable* m_fxWorldViewProj;

	// Fog.
	ID3DX11EffectScalarVariable* mfxFogStart;
	ID3DX11EffectScalarVariable* mfxFogRange;
	ID3DX11EffectVectorVariable* mfxFogColor;


protected:
	DWORD Release(void);



};

END