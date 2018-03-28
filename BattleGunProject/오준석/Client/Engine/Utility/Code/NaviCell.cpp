#include "NaviCell.h"
#include "Line2D.h"


Engine::CNaviCell::CNaviCell(const XMFLOAT3 * pPointA,
	const XMFLOAT3 * pPointB, const XMFLOAT3 * pPointC,
	const XMFLOAT4& pPlane)
	:m_iIndex(0)
{

	ZeroMemory(m_pNeighbor, sizeof(Engine::CNaviCell*) * NEIGHBOR_END);

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_pPlane.x = 0;
	m_pPlane.y = 0;
	m_pPlane.z = 0;
	m_pPlane.w = 0;
	m_pPlane = pPlane;
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
	//D3DXMATRIX		matView, matProj;
	//m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	//m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	//D3DXVECTOR3		vPoint[4];
	//vPoint[0] = m_vPoint[0];
	//vPoint[1] = m_vPoint[1];
	//vPoint[2] = m_vPoint[2];
	//vPoint[3] = m_vPoint[0];

	//for (int i = 0; i < 4; ++i)
	//{
	//	D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
	//	if (vPoint[i].z < 0.f)
	//		vPoint[i].z = 0.f;
	//	D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	//}

	//D3DXMATRIX		matIdentity;
	//D3DXMatrixIdentity(&matIdentity);

	//m_pLine->SetWidth(2.f);
	//m_pLine->Begin();
	//m_pLine->DrawTransform(vPoint, POINT_END + 1
	//	, &matIdentity, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	//m_pLine->End();
	//


}

bool Engine::CNaviCell::CheckPass(const XMFLOAT3 * pPos, const XMFLOAT3 * pDir, NEIGHBOR * pNeighbor)
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_pLine2D[i]->CheckLinePass(&XMFLOAT2(pPos->x + pDir->x
			, pPos->z + pDir->z)))
		{
			*pNeighbor = NEIGHBOR(i);
			return true;
		}
	}
	return false;
}

bool Engine::CNaviCell::ComparePoint(const XMFLOAT3 * pFirstPoint, const XMFLOAT3 * pSecondPoint, CNaviCell * pNeighbor)
{
	//XMVector3Equal(XMLoadFloat3(pFirstPoint), XMLoadFloat3(&m_vPoint[POINT_A]))
	if (XMVector3Equal(XMLoadFloat3(pFirstPoint), XMLoadFloat3(&m_vPoint[POINT_A])))
	{
		//if (*pSecondPoint == m_vPoint[POINT_B])
		if (XMVector3Equal(XMLoadFloat3(pSecondPoint), XMLoadFloat3(&m_vPoint[POINT_B])))
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else //if (*pSecondPoint == m_vPoint[POINT_C])
			if (XMVector3Equal(XMLoadFloat3(pSecondPoint), XMLoadFloat3(&m_vPoint[POINT_C])))
			{
				m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
				return true;
			}
	}

	//if (*pFirstPoint == m_vPoqint[POINT_B])
	if (XMVector3Equal(XMLoadFloat3(pFirstPoint), XMLoadFloat3(&m_vPoint[POINT_B])))
	{
		//if (*pSecondPoint == m_vPoint[POINT_A])
		if (XMVector3Equal(XMLoadFloat3(pSecondPoint), XMLoadFloat3(&m_vPoint[POINT_A])))
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else //if (*pSecondPoint == m_vPoint[POINT_C])
			if (XMVector3Equal(XMLoadFloat3(pSecondPoint), XMLoadFloat3(&m_vPoint[POINT_C])))
			{
				m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
				return true;
			}
	}

	//if (*pFirstPoint == m_vPoint[POINT_C])
	if (XMVector3Equal(XMLoadFloat3(pFirstPoint), XMLoadFloat3(&m_vPoint[POINT_C])))
	{
		//if (*pSecondPoint == m_vPoint[POINT_A])
		if (XMVector3Equal(XMLoadFloat3(pSecondPoint), XMLoadFloat3(&m_vPoint[POINT_A])))
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
		else //if (*pSecondPoint == m_vPoint[POINT_B])
			if (XMVector3Equal(XMLoadFloat3(pSecondPoint), XMLoadFloat3(&m_vPoint[POINT_B])))
			{
				m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
				return true;
			}
	}
	return false;
}

void Engine::CNaviCell::Release(void)
{
	for (int i = 0; i < LINE_END; ++i)
		Safe_Delete(m_pLine2D[i]);
}

Engine::CNaviCell * Engine::CNaviCell::Create(MYGDI * pMyGDI,
	const XMFLOAT3 * pPointA, const XMFLOAT3 * pPointB, const XMFLOAT3 * pPointC,
	const UINT & iIndex, const XMFLOAT4& pPlane)
{
	CNaviCell*	pCell = new CNaviCell(pPointA, pPointB, pPointC, pPlane);
	if (FAILED(pCell->InitCell(pMyGDI, iIndex)))
		Safe_Delete(pCell);

	return pCell;
}
