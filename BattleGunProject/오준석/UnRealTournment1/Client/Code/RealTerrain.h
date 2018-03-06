/*!
* \file RealTerrain.h
* \date 2017/05/31 15:17
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

#ifndef RealTerrain_h__
#define RealTerrain_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;
	class CStaticMesh;
	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CRealTerrain
	: public Engine::CGameObject
{
private:
	CRealTerrain(Engine::MYGDI* pMyGDI);

public:
	virtual ~CRealTerrain(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CRealTerrain* Create(Engine::MYGDI* pMyGDI);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);
	void MapMeshLoad();
	HRESULT AddMapMesh();

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	vector<Engine::CStaticMesh*>		m_VecMeshMap;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	ID3D11RasterizerState*      m_pWire;

private:
	Material				m_tMtrl;

private:
	Engine::VTXTEX*			m_pRealTerrainVtx;
	UINT*					m_pIndex;
	UINT					m_iIndexCnt;
};

#endif // RealTerrain_h__