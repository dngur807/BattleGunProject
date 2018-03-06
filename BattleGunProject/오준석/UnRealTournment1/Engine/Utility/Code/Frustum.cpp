#include "Frustum.h"
#include "Effects.h"

IMPLEMENT_SINGLETON(Engine::CFrustum)

Engine::CFrustum::CFrustum(void)
{
	ZeroMemory(m_vPoint, sizeof(XMFLOAT3) * 8);
	ZeroMemory(m_vConPoint, sizeof(XMFLOAT3) * 8);
	ZeroMemory(m_Plane, sizeof(XMFLOAT4) * 6);
}

Engine::CFrustum::~CFrustum(void)
{

}

HRESULT Engine::CFrustum::InitFrustum(MYGDI* pMyGDI)
{
	m_pMyGDI = pMyGDI;
	NULL_CHECK_RETURN(m_pMyGDI, E_FAIL);

	XMVECTOR vPoint[8];
	//¾Õ¸é
	vPoint[0] = XMVectorSet(-1.f, 1.f, 0.f, 0.f);
	vPoint[1] = XMVectorSet(1.f, 1.f, 0.f, 0.f);
	vPoint[2] = XMVectorSet(1.f, -1.f, 0.f, 0.f);
	vPoint[3] = XMVectorSet(-1.f, -1.f, 0.f, 0.f);
	//µÞ¸é
	vPoint[4] = XMVectorSet(-1.f, 1.f, 1.f, 0.f);
	vPoint[5] = XMVectorSet(1.f, 1.f, 1.f, 0.f);
	vPoint[6] = XMVectorSet(1.f, -1.f, 1.f, 0.f);
	vPoint[7] = XMVectorSet(-1.f, -1.f, 1.f, 0.f);

	XMMATRIX matProj;
	m_pMyGDI->pEffect->Proj->GetMatrix(reinterpret_cast<float*>(&matProj));
	matProj = XMMatrixInverse(nullptr, matProj);
	
	for (int i = 0; i < 8; ++i)
	{
		vPoint[i] = XMVector3TransformCoord(vPoint[i], matProj);
		XMStoreFloat3(&m_vPoint[i], vPoint[i]);
	}
	return S_OK;
}

void Engine::CFrustum::Transform_Frustum_ToWorld(void)
{
	XMMATRIX matView;
	m_pMyGDI->pEffect->View->GetMatrix(reinterpret_cast<float*>(&matView));
	matView = XMMatrixInverse(nullptr, matView);

	XMVECTOR vPoint[8];
	for (int i = 0; i < 8; ++i)
	{
		vPoint[i] = XMLoadFloat3(&m_vPoint[i]);
		vPoint[i] = XMVector3TransformCoord(vPoint[i], matView);
		XMStoreFloat3(&m_vConPoint[i], vPoint[i]);
	}
}

void Engine::CFrustum::Transform_Frustum_Local(const XMMATRIX& rWorld)
{
	XMMATRIX matWorldInv = XMMatrixInverse(nullptr, rWorld);

	XMVECTOR vConPoint[8];
	for (int i = 0; i < 8; ++i)
	{
		vConPoint[i] = XMLoadFloat3(&m_vConPoint[i]);
		vConPoint[i] = XMVector3TransformCoord(vConPoint[i], matWorldInv);
		XMStoreFloat3(&m_vConPoint[i], vConPoint[i]);
	}

	Make_Plane();
}

void Engine::CFrustum::CullingForTerrain(UINT* pIndex, UINT* pIndexCnt
	, const VTXTEX* pTerrainVtx, const UINT& wCntX, const UINT& wCntZ)
{
	*pIndexCnt = 0;

	WORD		wIndex = 0;
	bool		isIn[4] = { false };

	for (WORD z = 0; z < wCntZ - 1; ++z)
	{
		for (WORD x = 0; x < wCntX - 1; ++x)
		{
			wIndex = z * wCntX + x;
			isIn[0] = VertexInFrustum(XMLoadFloat3(&pTerrainVtx[wIndex + wCntX].vPos));
			isIn[1] = VertexInFrustum(XMLoadFloat3(&pTerrainVtx[wIndex + wCntX + 1].vPos));
			isIn[2] = VertexInFrustum(XMLoadFloat3(&pTerrainVtx[wIndex + 1].vPos));
			isIn[3] = VertexInFrustum(XMLoadFloat3(&pTerrainVtx[wIndex].vPos));

			if (isIn[0] || isIn[1] || isIn[2])
			{
				pIndex[*pIndexCnt] = wIndex + wCntX;		++(*pIndexCnt);
				pIndex[*pIndexCnt] = wIndex + wCntX + 1;	++(*pIndexCnt);
				pIndex[*pIndexCnt] = wIndex + 1;			++(*pIndexCnt);
			}

			if (isIn[0] || isIn[2] || isIn[3])
			{
				pIndex[*pIndexCnt] = wIndex + wCntX;		++(*pIndexCnt);
				pIndex[*pIndexCnt] = wIndex + 1;			++(*pIndexCnt);
				pIndex[*pIndexCnt] = wIndex;				++(*pIndexCnt);
			}
		}
	}
}

void Engine::CFrustum::Make_Plane(void)
{
	XMVECTOR Plane[6];

	Plane[0] = XMPlaneFromPoints(XMLoadFloat3(&m_vConPoint[1]), 
		XMLoadFloat3(&m_vConPoint[5]), XMLoadFloat3(&m_vConPoint[6])); // x+
	Plane[1] = XMPlaneFromPoints(XMLoadFloat3(&m_vConPoint[4]),
		XMLoadFloat3(&m_vConPoint[0]), XMLoadFloat3(&m_vConPoint[3])); // x-
	Plane[2] = XMPlaneFromPoints(XMLoadFloat3(&m_vConPoint[4]),
		XMLoadFloat3(&m_vConPoint[5]), XMLoadFloat3(&m_vConPoint[1])); // y+
	Plane[3] = XMPlaneFromPoints(XMLoadFloat3(&m_vConPoint[3]),
		XMLoadFloat3(&m_vConPoint[2]), XMLoadFloat3(&m_vConPoint[6])); // y-
	Plane[4] = XMPlaneFromPoints(XMLoadFloat3(&m_vConPoint[7]),
		XMLoadFloat3(&m_vConPoint[6]), XMLoadFloat3(&m_vConPoint[5])); // z+
	Plane[5] = XMPlaneFromPoints(XMLoadFloat3(&m_vConPoint[0]),
		XMLoadFloat3(&m_vConPoint[1]), XMLoadFloat3(&m_vConPoint[2])); // z-

	for(int i = 0; i < 6; ++i)
		XMStoreFloat4(&m_Plane[i], Plane[i]);
}

bool Engine::CFrustum::VertexInFrustum(const XMVECTOR& rPoint)
{
	float fDistance = 0.f;
	for (int i = 0; i < 6; ++i)
	{
		XMStoreFloat(&fDistance, (XMPlaneDotCoord(XMLoadFloat4(&m_Plane[i]), rPoint)));
		if (fDistance > -1.f)
			return false;
	}
	return true;
}

bool Engine::CFrustum::SphereInFrustum(const XMVECTOR& rCenter, const float& fRadius)
{
	float fDistance = 0.f;
	for (int i = 0; i < 6; ++i)
	{
		XMStoreFloat(&fDistance, (XMPlaneDotCoord(XMLoadFloat4(&m_Plane[i]), rCenter)));
		if (fDistance > fRadius)
			return false;
	}
	return true;
}
