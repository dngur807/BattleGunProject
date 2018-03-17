/*!
* \file PlayerUI3.h
* \date 2015/04/05 8:25
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
#ifndef PlayerUI3_h__
#define PlayerUI3_h__

#include "GameObject.h"
#include "Include.h"
#include "UI.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CPlayerUI3
	: public CUI
{
private:
	CPlayerUI3(Engine::MYGDI* pMyGDI);

public:
	virtual ~CPlayerUI3(void);

private:
	HRESULT Initialize(void);
	void	Release(void);

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CPlayerUI3* Create(Engine::MYGDI* pMyGDI, Engine::CGameObject* pCamera, const Engine::CTransform* pGameObj);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

private:
	Material		m_tMtrl;
	ID3D11RasterizerState*      m_pWire;
	DirectionalLight			m_DirLight;
	Engine::CGameObject* m_pCamera;
	const Engine::CTransform*	m_pTargetInfo;
	bool m_bMenu;
	int m_iMenu;

};

#endif // PlayerUI3_h__