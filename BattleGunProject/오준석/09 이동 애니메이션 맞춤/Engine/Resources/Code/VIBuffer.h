#pragma once
#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer :
	public CResources
{
public:
	enum BUFFERTYPE {
		BUFFER_RCTEX, BUFFER_TERRAIN, BUFFER_TILEMAP, BUFFER_CUBETEX, BUFFER_CUBECOL, BUFFER_BOX, BUFFER_WAVES,
		BUFFER_SKYBOX
	};

protected:
	explicit CVIBuffer(MYGDI* pMyGDI);
	//explicit CVIBuffer(const CVIBuffer& rhs);
public:
	virtual ~CVIBuffer();

public:
	virtual DWORD Release(void);

public:
	virtual void Render(XMFLOAT4X4& rmatWorld);

public:
	void GetVtxInfo(void* pVtxInfo);

public:
	void SetVtxInfo(void* pVtxInfo);
	void SetIdxInfo(void* pIdxInfo, const UINT* pIndexCnt);

public:
	virtual HRESULT CreateBuffer(void);

public:
	virtual CResources* CloneResource(void) PURE;

protected:
	void CreateGrid(float fWidth, float fDepth,
		UINT m, UINT n, MESHINFO& MeshInfo);


protected:
	ID3D11InputLayout*	m_pInputLayout;
	ID3DX11EffectTechnique* m_pTech;
	//ID3DX11EffectMatrixVariable* m_pFXWorldViewProj;

protected:
	ID3D11Buffer* m_pVB;
	UINT	m_dwVtxSize;
	DWORD	m_dwVtxCnt;
	DWORD m_dwVtxOffset;

protected:
	ID3D11Buffer* m_pIB;
	DWORD	m_dwIdxSize;
	DWORD	m_dwIndexCnt;
	DWORD	m_dwIdxOffset;

protected:
	float m_fPhi;
	float m_fTheta;
	float m_fRad;

protected:
	DirectX::XMFLOAT4X4 mWorld;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;
};

END