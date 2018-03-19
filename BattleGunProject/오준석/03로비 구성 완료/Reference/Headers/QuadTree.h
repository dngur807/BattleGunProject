

#ifndef QuadTree_h__
#define QuadTree_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CQuadTree
{
public:
	DECLARE_SINGLETON(CQuadTree)

public:
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_LB, CHILD_RB, CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB, CORNER_END };
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOT, NEIGHBOR_END };

private:
	CQuadTree(void);

public:
	~CQuadTree(void);

public:
	HRESULT InitQuadTree(const VTXTEX* pTerrainVtx
		, const WORD& wCntX, const WORD& wCntZ);
	void IsinFrustum(const VTXTEX* pTerrainVtx);
	void CullingToQuadTree(const VTXTEX* pTerrainVtx, UINT* pIndex, UINT* pIndexCnt);

private:
	void SetChild(const VTXTEX* pTerrainVtx);
	CQuadTree* MakeChild(CHILD eChildID);
	void Release(void);

public:
	HRESULT InitNeighbor(const VTXTEX* pTerrainVtx, CQuadTree* pParentQuad,
		const WORD& wCntX, const WORD& wCntZ);
	void Culling_LOD(const VTXTEX* pTerrainVtx, UINT* pIndices, UINT* pIdxCnt);

private:
	bool LevelofDetail(const VTXTEX* pTerrainVtx);
	bool MakeNeighbor(NEIGHBOR eNeighborID,
		int& iLT, int& iRT, int& iLB, int& iRB,
		const WORD& wCntX, const WORD& wCntZ);
	CQuadTree* FindNeighbor(const VTXTEX* pTerrainVtx,
		const int& iLT, const int& iRT, const int& iLB, const int& iRB, const int& iCenter);

private:
	CQuadTree*		m_pChild[CHILD_END];
	CQuadTree*		m_pNeighbor[NEIGHBOR_END];
	WORD			m_wCorner[CORNER_END];
	int				m_iCenter;
	float			m_fRadius;
	bool			m_isIn;
};

END

#endif // QuadTree_h__