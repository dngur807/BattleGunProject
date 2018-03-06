/*!
* \file SelectMenu6.h
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
#ifndef SelectMenu6_h__
#define SelectMenu6_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CSelectMenu6
	: public Engine::CGameObject
{
private:
	CSelectMenu6(Engine::MYGDI* pMyGDI);

public:
	virtual ~CSelectMenu6(void);

private:
	HRESULT Initialize(void);
	void	Release(void);

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CSelectMenu6* Create(Engine::MYGDI* pMyGDI);

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

#endif // SelectMenu6_h__