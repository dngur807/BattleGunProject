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
		const XMFLOAT3* pPointB, const XMFLOAT3* pPointC, const XMFLOAT4& pPlane);
	void CreateBuffer(void);
	void Render_NaviMesh(void);
	CNaviCell* FindLocation(const XMFLOAT3* _vPos);
	DWORD FindIndex(const XMFLOAT3* _vPos);
	float FindfTop(const XMFLOAT3* _vPos);
	void FindNeighbor(void);
	DWORD MoveOnNaviMesh(XMFLOAT3* pPos
		, const XMFLOAT3* pDir
		, const DWORD& dwCurrentIdx
		, float& pTop);
	void LinkCell(void);
	bool DeleteCell();

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
private:
	DWORD		m_dwIdxCnt;
	ID3D11RasterizerState*      m_pWire;
};

END