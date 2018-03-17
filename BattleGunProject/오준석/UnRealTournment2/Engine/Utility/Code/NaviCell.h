#pragma once

#include "Engine_Include.h"
BEGIN(Engine)

enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
class CLine2D;

class ENGINE_DLL CNaviCell
{
private:
	CNaviCell(const XMFLOAT3* pPointA,
		const XMFLOAT3* pPointB, const XMFLOAT3* pPointC,
		const XMFLOAT4& pPlane);
public:
	~CNaviCell();

public:
	//const XMFLOAT3* GetPoint(POINT ePointID) { return &m_vPoint[ePointID]; }
	CNaviCell* GetNeighbor(NEIGHBOR eNeighborID) { return m_pNeighbor[eNeighborID]; }
	UINT GetIndex(void) { return m_iIndex; }

public:
	void SetNeighbor(NEIGHBOR eNeighborID, CNaviCell* pNeighbor)
	{
		m_pNeighbor[eNeighborID] = pNeighbor;
	}
public:
	HRESULT InitCell(MYGDI* pMyGDI, const UINT& iIndex);
	void Render(void);
	bool CheckPass(const XMFLOAT3* pPos, const XMFLOAT3* pDir, NEIGHBOR* pNeighbor);
	bool ComparePoint(const XMFLOAT3* pFirstPoint, const XMFLOAT3* pSecondPoint
		, CNaviCell* pNeighbor);
public:
	const XMFLOAT3* GetPoint(void) { return m_vPoint; }
	const XMFLOAT3* GetPoint(POINT ePointID) { return &m_vPoint[ePointID]; }
	CLine2D* GetLine2D(int iIndex) { return m_pLine2D[iIndex]; }
	std::vector<CNaviCell*>* GetvecNeighbor(void) { return &m_vecNeighbor; }

private:
	void Release(void);

public:
	static CNaviCell* Create(MYGDI* pMyGDI,
		const XMFLOAT3* pPointA,
		const XMFLOAT3* pPointB,
		const XMFLOAT3* pPointC,
		const UINT& iIndex,
		const XMFLOAT4& pPlane);


public:
	XMFLOAT4				m_pPlane;
private:
	std::vector<CNaviCell*>	m_vecNeighbor;
	CNaviCell*				m_pNeighbor[NEIGHBOR_END];
private:
	MYGDI*		m_pMyGDI;
	CLine2D*	m_pLine2D[LINE_END];
private:
	XMFLOAT3	m_vPoint[POINT_END];
	UINT		m_iIndex;
};

END