#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex :
	public CVIBuffer
{
private:
	explicit CTerrainTex(MYGDI* pMyGDI);
	//explicit CTerrainTex(const CTerrainTex& rhs);
public:
	virtual ~CTerrainTex();

public:
	virtual HRESULT CreateBuffer(const float& fWidth, const float& fDepth, 
		const UINT& iCntX, const UINT& iCntZ);

public:
	VTXTEX* GetVertex(void) { return m_pVertex; }
	float CTerrainTex::GetHillHeight(float x, float z)const;
	DirectX::XMFLOAT3 CTerrainTex::GetHillNormal(float x, float z)const;

public:
	virtual CResources* CloneResource(void);

public:
	static CTerrainTex* Create(MYGDI* pMyGDI, const float& fWidth, 
		const float& fDepth, const UINT& iCntX, const UINT& iCntZ);

private:
	VTXTEX*		m_pVertex;
};

END	