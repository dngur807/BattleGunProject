/*!
* \file SelectMenu2.h
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
#ifndef SelectMenu2_h__
#define SelectMenu2_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CSelectMenu2
	: public Engine::CGameObject
{
private:
	CSelectMenu2(Engine::MYGDI* pMyGDI);

public:
	virtual ~CSelectMenu2(void);

private:
	HRESULT Initialize(void);
	void	Release(void);

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CSelectMenu2* Create(Engine::MYGDI* pMyGDI);

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
	float m_fTime;
	bool m_bRight;
	bool m_bMouseOn;
	POINT	ptMouse;

};

#endif // SelectMenu2_h__