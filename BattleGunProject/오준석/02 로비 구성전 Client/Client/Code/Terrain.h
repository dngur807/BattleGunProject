/*!
* \file Terrain.h
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

#ifndef Terrain_h__
#define Terrain_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CTerrain
	: public Engine::CGameObject
{
private:
	CTerrain(Engine::MYGDI* pMyGDI);

public:
	virtual ~CTerrain(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CTerrain* Create(Engine::MYGDI* pMyGDI);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

private:
	Material				m_tMtrl;

private:
	Engine::VTXTEX*			m_pTerrainVtx;
	UINT*					m_pIndex;
	UINT					m_iIndexCnt;
};

#endif // Terrain_h__