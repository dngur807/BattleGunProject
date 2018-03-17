#pragma once
#include "engine_include.h"

BEGIN(Engine)

class CNaviCell;

class ENGINE_DLL CNaviMgr
{
public:
	DECLARE_SINGLETON(CNaviMgr)

private:
	CNaviMgr();
	~CNaviMgr();

public:
	void ReserveCellSize(MYGDI* pMyGDI, const UINT& iSize);
	HRESULT AddCell(const XMFLOAT3* pPointA,
		const XMFLOAT3* pPointB, const XMFLOAT3* pPointC);
	void CreateBuffer(void);
	void Render_NaviMesh(void);
	CNaviCell* FindLocation(const XMFLOAT3* _vPos);
	void FindNeighbor(void);

private:
	void Release(void);

private:
	MYGDI*	m_pMyGDI;
	UINT	m_iReservedSize;
	UINT	m_iIdxCnt;

private:
	typedef std::vector<CNaviCell*>	VECCELL;
	VECCELL		m_vecNaviMesh;

private:
	ID3D11InputLayout*	m_pInputLayout;
	ID3D11Buffer* m_pVB;
	ID3D11Buffer* m_pIB;

	typedef std::vector<VTXTEX> VECTEX;
	VECTEX	m_vecVertices;
	typedef std::vector<UINT> VECINDEX;
	VECINDEX m_vecIndices;
};

END	