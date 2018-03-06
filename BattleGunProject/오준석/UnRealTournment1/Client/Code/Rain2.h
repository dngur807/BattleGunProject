/*!
* \file Rain2.h
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
#ifndef Rain2_h__
#define Rain2_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CRain2
	: public Engine::CGameObject
{
private:
	CRain2(Engine::MYGDI* pMyGDI);

public:
	virtual ~CRain2(void);

private:
	HRESULT Initialize(void);
	void	Release(void);

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CRain2* Create(Engine::MYGDI* pMyGDI, const Engine::CTransform* pGameObj, Engine::CGameObject* pCamera);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	//vector<Engine::CTransform*>		Vec_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

private:
	Material		m_tMtrl;
	ID3D11RasterizerState*      m_pWire;
	//Material					m_tMtrl;
	DirectionalLight			m_DirLight;
	ID3D11BlendState*           AlphaToCoverageBS = 0;
	Engine::CGameObject* m_pCamera;
	const Engine::CTransform*	m_pTargetInfo;
	float m_fSpeed;



};

#endif // Loading_h__