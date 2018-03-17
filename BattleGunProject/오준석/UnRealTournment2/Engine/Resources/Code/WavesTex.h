#pragma once
#include "VIBuffer.h"
#include "Waves.h"
#include "ParticleSystem.h"
BEGIN(Engine)

class ENGINE_DLL CWavesTex :
	public CVIBuffer
{
public:
	explicit CWavesTex(MYGDI* pMyGDI);
	explicit CWavesTex(const CWavesTex& rhs);
public:
	virtual ~CWavesTex();

public:
	virtual HRESULT CreateBuffer(void);

public:
	virtual CResources* CloneResource(void);

public:
	static CWavesTex* Create(MYGDI* pMyGDI);
	Waves mWaves;
	float GetHeight(float x, float z)const;
	float mTheta;
	float mPhi;
	float mRadius;
	void DrawWaves(XMFLOAT4X4& rmatWorld, float dt, ID3D11ShaderResourceView** pTexture=NULL);
	void RainUpdate(float dt, float gameTime);

	ID3DX11EffectVariable* mfxMaterial;
	Material mWavesMat;
	DirectionalLight mDirLight;
	ID3DX11EffectVariable* mfxDirLight;
	ID3DX11Effect*	m_pFX;
	XMFLOAT3 mEyePosW;
	PointLight mPointLight;
	SpotLight mSpotLight;

	///
	XMFLOAT4X4 mWaterTexTransform;
	XMFLOAT2 mWaterTexOffset;
	ID3D11ShaderResourceView* mWavesMapSRV;
	//안개
	RenderOptions mRenderOptions;

	//비 파티클
	void DrawRain(XMFLOAT4X4& rmatWorld, float dt, ID3D11ShaderResourceView** pTexture);
	CParticleSystem mRain;
	float mGameTime;
	float mTimeStep;
	float mAge;
	static ID3D11InputLayout* Particle;

};

END

