/*!
* \file SelectBack.h
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
#ifndef SelectBack_h__
#define SelectBack_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CSelectBack
	: public Engine::CGameObject
{
private:
	CSelectBack(Engine::MYGDI* pMyGDI);

public:
	virtual ~CSelectBack(void);

private:
	HRESULT Initialize(void);
	void	Release(void);

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CSelectBack* Create(Engine::MYGDI* pMyGDI);

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
	int m_iTex;
	bool m_bRight;

};

#endif // SelectBack_h__