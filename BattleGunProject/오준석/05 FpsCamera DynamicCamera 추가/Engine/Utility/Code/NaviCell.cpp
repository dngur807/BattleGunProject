#include "NaviCell.h"
#include "Line2D.h"


Engine::CNaviCell::CNaviCell(const XMFLOAT3 * pPointA, 
	const XMFLOAT3 * pPointB, const XMFLOAT3 * pPointC)
{
	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;
}

Engine::CNaviCell::~CNaviCell()
{
	Release();
}

HRESULT Engine::CNaviCell::InitCell(MYGDI * pMyGDI, const UINT & iIndex)
{
	m_pMyGDI = pMyGDI;
	m_iIndex = iIndex;

	m_pLine2D[LINE_AB] = CLine2D::Create(&m_vPoint[POINT_A], &m_vPoint[POINT_B]);
	m_pLine2D[LINE_BC] = CLine2D::Create(&m_vPoint[POINT_B], &m_vPoint[POINT_C]);
	m_pLine2D[LINE_CA] = CLine2D::Create(&m_vPoint[POINT_C], &m_vPoint[POINT_A]);

	return S_OK;
}

void Engine::CNaviCell::Render(void)
{
}

void Engine::CNaviCell::Release(void)
{
	for (int i = 0; i < LINE_END; ++i)
		Safe_Delete(m_pLine2D[i]);
}

Engine::CNaviCell * Engine::CNaviCell::Create(MYGDI * pMyGDI, 
	const XMFLOAT3 * pPointA, const XMFLOAT3 * pPointB, const XMFLOAT3 * pPointC, 
	const UINT & iIndex)
{
	CNaviCell*	pCell = new CNaviCell(pPointA, pPointB, pPointC);
	if (FAILED(pCell->InitCell(pMyGDI, iIndex)))
		Safe_Delete(pCell);

	return pCell;
}
