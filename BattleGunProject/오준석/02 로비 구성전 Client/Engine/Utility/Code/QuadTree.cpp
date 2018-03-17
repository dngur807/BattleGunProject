#include "QuadTree.h"

#include "Export_Function.h"
#include "Effects.h"

IMPLEMENT_SINGLETON(Engine::CQuadTree)

Engine::CQuadTree::CQuadTree(void)
	: m_iCenter(0)
	, m_fRadius(0.f)
	, m_isIn(false)
{
	ZeroMemory(m_pChild, sizeof(CQuadTree*) * CHILD_END);
	ZeroMemory(m_wCorner, sizeof(WORD) * CORNER_END);
	ZeroMemory(m_pNeighbor, sizeof(CQuadTree*) * NEIGHBOR_END);
}

Engine::CQuadTree::~CQuadTree(void)
{
	Release();
}

HRESULT Engine::CQuadTree::InitQuadTree(const VTXTEX* pTerrainVtx, 
	const WORD& wCntX, const WORD& wCntZ)
{
	m_wCorner[CORNER_LT] = 0;
	m_wCorner[CORNER_RT] = wCntX - 1;
	m_wCorner[CORNER_LB] = wCntX * (wCntZ - 1);
	m_wCorner[CORNER_RB] = wCntX * wCntZ - 1;

	for (int i = 0; i < CORNER_END; ++i)
		m_iCenter += m_wCorner[i];

	m_iCenter = m_iCenter >> 2;

	SetChild(pTerrainVtx);
	return S_OK;
}

void Engine::CQuadTree::SetChild(const VTXTEX* pTerrainVtx)
{
	XMStoreFloat(&m_fRadius, XMVector3Length(XMLoadFloat3(&(pTerrainVtx[m_wCorner[CORNER_LT]].vPos)) -
		XMLoadFloat3(&pTerrainVtx[m_iCenter].vPos)));

	for (int i = 0; i < CHILD_END; ++i)
	{
		m_pChild[i] = MakeChild(CHILD(i));
		if (m_pChild[i] != NULL)
			m_pChild[i]->SetChild(pTerrainVtx);
	}
}

Engine::CQuadTree* Engine::CQuadTree::MakeChild(CHILD eChildID)
{
	if (m_wCorner[CORNER_RT] - m_wCorner[CORNER_LT] == 1)
	{
		m_fRadius = 0.f;
		return NULL;
	}

	int		iLC, iTC, iRC, iBC;
	iLC = (m_wCorner[CORNER_LT] + m_wCorner[CORNER_LB]) >> 1;
	iTC = (m_wCorner[CORNER_LT] + m_wCorner[CORNER_RT]) >> 1;
	iRC = (m_wCorner[CORNER_RT] + m_wCorner[CORNER_RB]) >> 1;
	iBC = (m_wCorner[CORNER_LB] + m_wCorner[CORNER_RB]) >> 1;

	CQuadTree*		pQuadTree = new CQuadTree;
	switch (eChildID)
	{
	case CHILD_LT:
		pQuadTree->m_wCorner[CORNER_LT] = m_wCorner[CORNER_LT];
		pQuadTree->m_wCorner[CORNER_RT] = iTC;
		pQuadTree->m_wCorner[CORNER_LB] = iLC;
		pQuadTree->m_wCorner[CORNER_RB] = m_iCenter;
		break;

	case CHILD_RT:
		pQuadTree->m_wCorner[CORNER_LT] = iTC;
		pQuadTree->m_wCorner[CORNER_RT] = m_wCorner[CORNER_RT];
		pQuadTree->m_wCorner[CORNER_LB] = m_iCenter;
		pQuadTree->m_wCorner[CORNER_RB] = iRC;
		break;

	case CHILD_LB:
		pQuadTree->m_wCorner[CORNER_LT] = iLC;
		pQuadTree->m_wCorner[CORNER_RT] = m_iCenter;
		pQuadTree->m_wCorner[CORNER_LB] = m_wCorner[CORNER_LB];
		pQuadTree->m_wCorner[CORNER_RB] = iBC;
		break;

	case CHILD_RB:
		pQuadTree->m_wCorner[CORNER_LT] = m_iCenter;
		pQuadTree->m_wCorner[CORNER_RT] = iRC;
		pQuadTree->m_wCorner[CORNER_LB] = iBC;
		pQuadTree->m_wCorner[CORNER_RB] = m_wCorner[CORNER_RB];
		break;
	}

	for (int i = 0; i < CORNER_END; ++i)
		pQuadTree->m_iCenter += pQuadTree->m_wCorner[i];

	pQuadTree->m_iCenter = pQuadTree->m_iCenter >> 2;
	return pQuadTree;
}

void Engine::CQuadTree::Release(void)
{
	for (int i = 0; i < CHILD_END; ++i)
	{
		Safe_Delete(m_pChild[i]);
	}
}

HRESULT Engine::CQuadTree::InitNeighbor(const VTXTEX * pTerrainVtx, CQuadTree * pParentQuad, const WORD & wCntX, const WORD & wCntZ)
{
	int iLT, iRT, iLB, iRB;

	for (int i = 0; i < NEIGHBOR_END; ++i)
	{
		if (MakeNeighbor((NEIGHBOR)i, iLT, iRT, iLB, iRB, wCntX, wCntZ))
		{
			int iCenter = (iLT + iRT + iLB + iRB) >> 2;
			m_pNeighbor[i] = pParentQuad->FindNeighbor(pTerrainVtx, iLT, iRT, iLB, iRB, iCenter);
		}
		else
			m_pNeighbor[i] = NULL;
	}

	for (int i = 0; i < CHILD_END; ++i)
	{
		if (m_pChild[i])
			m_pChild[i]->InitNeighbor(pTerrainVtx, pParentQuad, wCntX, wCntZ);
	}

	return S_OK;
}

void Engine::CQuadTree::Culling_LOD(const VTXTEX * pTerrainVtx, UINT * pIndices, UINT * pIdxCnt)
{
	if (m_isIn)
	{
		if (LevelofDetail(pTerrainVtx))
		{
			bool isRender[4] = { true, true, true, true };

			for (int i = 0; i < NEIGHBOR_END; ++i)
			{
				if (m_pNeighbor[i])
					isRender[i] = m_pNeighbor[i]->LevelofDetail(pTerrainVtx);
			}

			if (isRender[NEIGHBOR_LEFT] && isRender[NEIGHBOR_TOP] &&
				isRender[NEIGHBOR_RIGHT] && isRender[NEIGHBOR_BOT])
			{
				pIndices[*pIdxCnt] = m_wCorner[CORNER_LB];
				pIndices[*pIdxCnt + 1] = m_wCorner[CORNER_RB];
				pIndices[*pIdxCnt + 2] = m_wCorner[CORNER_RT];

				pIndices[*pIdxCnt + 3] = m_wCorner[CORNER_LB];
				pIndices[*pIdxCnt + 4] = m_wCorner[CORNER_RT];
				pIndices[*pIdxCnt + 5] = m_wCorner[CORNER_LT];

				*pIdxCnt += 6;
				return;
			}

			if (!isRender[NEIGHBOR_LEFT])
			{
				int iLC = (m_wCorner[CORNER_LT] + m_wCorner[CORNER_LB]) >> 1;
				pIndices[*pIdxCnt] = m_wCorner[CORNER_LB];
				pIndices[*pIdxCnt + 1] = m_iCenter;
				pIndices[*pIdxCnt + 2] = iLC;

				pIndices[*pIdxCnt + 3] = iLC;
				pIndices[*pIdxCnt + 4] = m_iCenter;
				pIndices[*pIdxCnt + 5] = m_wCorner[CORNER_LT];

				*pIdxCnt += 6;
			}
			else
			{
				pIndices[*pIdxCnt] = m_wCorner[CORNER_LB];
				pIndices[*pIdxCnt + 1] = m_iCenter;
				pIndices[*pIdxCnt + 2] = m_wCorner[CORNER_LT];

				*pIdxCnt += 3;
			}

			if (!isRender[NEIGHBOR_TOP])
			{
				int iTC = (m_wCorner[CORNER_LT] + m_wCorner[CORNER_RT]) >> 1;
				pIndices[*pIdxCnt] = m_wCorner[CORNER_LT];
				pIndices[*pIdxCnt + 1] = m_iCenter;
				pIndices[*pIdxCnt + 2] = iTC;

				pIndices[*pIdxCnt + 3] = iTC;
				pIndices[*pIdxCnt + 4] = m_iCenter;
				pIndices[*pIdxCnt + 5] = m_wCorner[CORNER_RT];

				*pIdxCnt += 6;
			}
			else
			{
				pIndices[*pIdxCnt] = m_wCorner[CORNER_LT];
				pIndices[*pIdxCnt + 1] = m_iCenter;
				pIndices[*pIdxCnt + 2] = m_wCorner[CORNER_RT];

				*pIdxCnt += 3;
			}

			if (!isRender[NEIGHBOR_RIGHT])
			{
				int iRC = (m_wCorner[CORNER_RT] + m_wCorner[CORNER_RB]) >> 1;
				pIndices[*pIdxCnt] = m_iCenter;
				pIndices[*pIdxCnt + 1] = m_wCorner[CORNER_RB];
				pIndices[*pIdxCnt + 2] = iRC;

				pIndices[*pIdxCnt + 3] = iRC;
				pIndices[*pIdxCnt + 4] = m_wCorner[CORNER_RT];
				pIndices[*pIdxCnt + 5] = m_iCenter;

				*pIdxCnt += 6;
			}
			else
			{
				pIndices[*pIdxCnt] = m_iCenter;
				pIndices[*pIdxCnt + 1] = m_wCorner[CORNER_RB];
				pIndices[*pIdxCnt + 2] = m_wCorner[CORNER_RT];

				*pIdxCnt += 3;
			}

			if (!isRender[NEIGHBOR_BOT])
			{
				int iBC = (m_wCorner[CORNER_LB] + m_wCorner[CORNER_RB]) >> 1;
				pIndices[*pIdxCnt] = m_wCorner[CORNER_LB];
				pIndices[*pIdxCnt + 1] = iBC;
				pIndices[*pIdxCnt + 2] = m_iCenter;

				pIndices[*pIdxCnt + 3] = m_iCenter;
				pIndices[*pIdxCnt + 4] = iBC;
				pIndices[*pIdxCnt + 5] = m_wCorner[CORNER_RB];

				*pIdxCnt += 6;
			}
			else
			{
				pIndices[*pIdxCnt] = m_iCenter;
				pIndices[*pIdxCnt + 1] = m_wCorner[CORNER_LB];
				pIndices[*pIdxCnt + 2] = m_wCorner[CORNER_RB];

				*pIdxCnt += 3;
			}
			return;
		}
		for (int i = 0; i < CHILD_END; ++i)
		{
			if (m_pChild[i])
			{
				m_pChild[i]->Culling_LOD(pTerrainVtx, pIndices, pIdxCnt);
			}
		}
	}
}

void Engine::CQuadTree::IsinFrustum(const VTXTEX* pTerrainVtx)
{
	if (m_pChild[CHILD_LT] == NULL)
	{
		m_isIn = true;
		return;
	}

	m_isIn = Engine::Get_Frustum()->SphereInFrustum(XMLoadFloat3(&pTerrainVtx[m_iCenter].vPos)
		, m_fRadius);

	if (m_isIn == true)
	{
		for (int i = 0; i < CHILD_END; ++i)
		{
			if (m_pChild[i] != NULL)
				m_pChild[i]->IsinFrustum(pTerrainVtx);
		}
	}

}

void Engine::CQuadTree::CullingToQuadTree(const VTXTEX* pTerrainVtx, UINT* pIndex, UINT* pIndexCnt)
{
	if (m_isIn == true)
	{
		if (m_pChild[CORNER_LT] == NULL)
		{
			bool	isIn[4] = { false };

			isIn[0] = Engine::Get_Frustum()->VertexInFrustum(XMLoadFloat3(&pTerrainVtx[m_wCorner[CORNER_LB]].vPos));
			isIn[1] = Engine::Get_Frustum()->VertexInFrustum(XMLoadFloat3(&pTerrainVtx[m_wCorner[CORNER_RB]].vPos));
			isIn[2] = Engine::Get_Frustum()->VertexInFrustum(XMLoadFloat3(&pTerrainVtx[m_wCorner[CORNER_RT]].vPos));
			isIn[3] = Engine::Get_Frustum()->VertexInFrustum(XMLoadFloat3(&pTerrainVtx[m_wCorner[CORNER_LT]].vPos));

			if (isIn[0] || isIn[1] || isIn[2])
			{
				pIndex[*pIndexCnt] = m_wCorner[CORNER_LB];	++(*pIndexCnt);
				pIndex[*pIndexCnt] = m_wCorner[CORNER_RB];	++(*pIndexCnt);
				pIndex[*pIndexCnt] = m_wCorner[CORNER_RT];	++(*pIndexCnt);
			}

			if (isIn[0] || isIn[2] || isIn[3])
			{
				pIndex[*pIndexCnt] = m_wCorner[CORNER_LB];	++(*pIndexCnt);
				pIndex[*pIndexCnt] = m_wCorner[CORNER_RT];	++(*pIndexCnt);
				pIndex[*pIndexCnt] = m_wCorner[CORNER_LT];	++(*pIndexCnt);
			}
			return;
		}
		for (int i = 0; i < CHILD_END; ++i)
		{
			if (m_pChild[i] != NULL)
			{
				m_pChild[i]->CullingToQuadTree(pTerrainVtx, pIndex, pIndexCnt);
			}
		}
	}
}

bool Engine::CQuadTree::LevelofDetail(const VTXTEX* pTerrainVtx)
{
	XMMATRIX matView;

	Engine::Get_Effects()->BasicFX->View->GetMatrix(reinterpret_cast<float*>(&matView));
	matView = XMMatrixInverse(nullptr, matView);

	XMVECTOR vCamPos = matView.r[3];
	float fDistance = 0.f;

	XMStoreFloat(&fDistance, XMVector3Length(vCamPos - XMLoadFloat3(&pTerrainVtx[m_iCenter].vPos)));

	float fWidth = pTerrainVtx[m_wCorner[CORNER_RT]].vPos.x
		- pTerrainVtx[m_wCorner[CORNER_LT]].vPos.x;

	if (fDistance * 0.1f > fWidth)
		return true;

	return false;
}

bool Engine::CQuadTree::MakeNeighbor(NEIGHBOR eNeighborID, int & iLT, int & iRT, int & iLB, int & iRB, const WORD & wCntX, const WORD & wCntZ)
{
	int iSizeX = m_wCorner[CORNER_RT] - m_wCorner[CORNER_LT];
	int iSizeZ = m_wCorner[CORNER_LB] - m_wCorner[CORNER_LT];

	switch (eNeighborID)
	{
	case NEIGHBOR_LEFT:
		if (m_wCorner[CORNER_LT] % wCntX == 0)
			return false;
		iLT = m_wCorner[CORNER_LT] - iSizeX;
		iRT = m_wCorner[CORNER_LT];
		iLB = m_wCorner[CORNER_LB] - iSizeX;
		iRB = m_wCorner[CORNER_LB];
		break;
	case NEIGHBOR_TOP:
		if (m_wCorner[CORNER_RT] < wCntX)
			return false;
		iLT = m_wCorner[CORNER_LT] - iSizeZ;
		iRT = m_wCorner[CORNER_RT] - iSizeZ;
		iLB = m_wCorner[CORNER_LT];
		iRB = m_wCorner[CORNER_RT];
		break;
	case NEIGHBOR_RIGHT:
		if (m_wCorner[CORNER_RT] % wCntX == wCntX - 1)
			return false;
		iLT = m_wCorner[CORNER_RT];
		iRT = m_wCorner[CORNER_RT] + iSizeX;
		iLB = m_wCorner[CORNER_RB];
		iRB = m_wCorner[CORNER_RB] + iSizeX;
		break;
	case NEIGHBOR_BOT:
		if (m_wCorner[CORNER_LB] >= wCntX * (wCntZ - 1))
			return false;
		iLT = m_wCorner[CORNER_LB];
		iRT = m_wCorner[CORNER_RB];
		iLB = m_wCorner[CORNER_LB] + iSizeZ;
		iRB = m_wCorner[CORNER_RB] + iSizeZ;
		break;
	}

	return true;
}

Engine::CQuadTree * Engine::CQuadTree::FindNeighbor(const VTXTEX * pTerrainVtx, 
	const int & iLT, const int & iRT, const int & iLB, const int & iRB, const int & iCenter)
{
	if (m_wCorner[CORNER_LT] == iLT && m_wCorner[CORNER_RT] == iRT &&
		m_wCorner[CORNER_LB] == iLB && m_wCorner[CORNER_RB] == iRB)
		return this;

	RECT rcQuad;
	POINT ptCenter;

	for (int i = 0; i < CHILD_END; ++i)
	{
		if (m_pChild[i])
		{
			ZeroMemory(&rcQuad, sizeof(RECT));
			ZeroMemory(&ptCenter, sizeof(POINT));

			SetRect(&rcQuad, int(pTerrainVtx[m_pChild[i]->m_wCorner[CORNER_LT]].vPos.x),
				int(pTerrainVtx[m_pChild[i]->m_wCorner[CORNER_LT]].vPos.z),
				int(pTerrainVtx[m_pChild[i]->m_wCorner[CORNER_RB]].vPos.x),
				int(pTerrainVtx[m_pChild[i]->m_wCorner[CORNER_RB]].vPos.z));

			ptCenter.x = (LONG)pTerrainVtx[iCenter].vPos.x;
			ptCenter.y = (LONG)pTerrainVtx[iCenter].vPos.z;

			if (PtInRect(&rcQuad, ptCenter))
				return m_pChild[i]->FindNeighbor(pTerrainVtx, iLT, iRT, iLB, iRB, iCenter);
		}
	}

	return nullptr;
}

