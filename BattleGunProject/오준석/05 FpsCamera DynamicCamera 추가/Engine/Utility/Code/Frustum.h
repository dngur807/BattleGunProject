/*!
* \file Frustum.h
* \date 2017/06/01 16:20
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef Frustum_h__
#define Frustum_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum
{
public:
	DECLARE_SINGLETON(CFrustum)

private:
	CFrustum(void);
	~CFrustum(void);

public:
	HRESULT InitFrustum(MYGDI* pMyGDI);
	void Transform_Frustum_ToWorld(void);
	void Transform_Frustum_Local(const XMMATRIX& rWorld);
	void CullingForTerrain(UINT* pIndex, UINT* pIndexCnt
		, const VTXTEX* pTerrainVtx, const UINT& wCntX, const UINT& wCntZ);
	bool VertexInFrustum(const XMVECTOR& rPoint);
	bool SphereInFrustum(const XMVECTOR& rCenter, const float& fRadius);

private:
	void Make_Plane(void);

private:
	MYGDI*				m_pMyGDI;

private:
	XMFLOAT3			m_vPoint[8];
	XMFLOAT3			m_vConPoint[8];

	XMFLOAT4			m_Plane[6];
};

END

#endif // Frustum_h__