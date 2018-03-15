#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };

class CLine2D;

class ENGINE_DLL CNaviCell
{
private:
	CNaviCell(const XMFLOAT3* pPointA, 
		const XMFLOAT3* pPointB, const XMFLOAT3* pPointC);
public:
	~CNaviCell();

public:
	HRESULT InitCell(MYGDI* pMyGDI, const UINT& iIndex);
	void Render(void);

public:
	const XMFLOAT3* GetPoint(void) { return m_vPoint; }
	CLine2D* GetLine2D(int iIndex) { return m_pLine2D[iIndex]; }
	std::vector<CNaviCell*>* GetvecNeighbor(void) { return &m_vecNeighbor; }

private:
	void Release(void);

public:
	static CNaviCell* Create(MYGDI* pMyGDI,
		const XMFLOAT3* pPointA,
		const XMFLOAT3* pPointB,
		const XMFLOAT3* pPointC,
		const UINT& iIndex);

private:
	std::vector<CNaviCell*>	m_vecNeighbor;
private:
	MYGDI*		m_pMyGDI;
	CLine2D*	m_pLine2D[LINE_END];
private:
	XMFLOAT3	m_vPoint[POINT_END];
	UINT		m_iIndex;
};

END