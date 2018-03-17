#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMapTile :
	public CVIBuffer
{
private:
	explicit CMapTile(MYGDI* pMyGDI);
	//explicit CMapTile(const CMapTile& rhs);
public:
	virtual ~CMapTile();

public:
	virtual HRESULT CreateBuffer(const float& fWidth, const float& fDepth, 
		const UINT& iCntX, const UINT& iCntZ);

public:
	VTXTEX* GetVertex(void) { return m_pVertex; }
	float CMapTile::GetHillHeight(float x, float z)const;
	DirectX::XMFLOAT3 CMapTile::GetHillNormal(float x, float z)const;

public:
	virtual CResources* CloneResource(void);

public:
	static CMapTile* Create(MYGDI* pMyGDI, const float& fWidth, 
		const float& fDepth, const UINT& iCntX, const UINT& iCntZ);

private:
	VTXTEX*		m_pVertex;
};

END	