/*!
* \file UI.h
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
#ifndef UI_h__
#define UI_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CUI
	: public Engine::CGameObject
{
protected:
	CUI(Engine::MYGDI* pMyGDI);

public:
	virtual ~CUI(void);

protected:
	HRESULT Initialize(void);
	void	Release(void);

public:
	virtual int Update(void);
	virtual void Render(void);

protected:
	virtual HRESULT AddComponent(void);

public:
	static CUI* Create(Engine::MYGDI* pMyGDI, Engine::CGameObject* pCamera);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

private:
	Material		m_tMtrl;
	ID3D11RasterizerState*      m_pWire;
	//Material					m_tMtrl;
	DirectionalLight			m_DirLight;
	Engine::CGameObject* m_pCamera;

};

#endif // UI_h__