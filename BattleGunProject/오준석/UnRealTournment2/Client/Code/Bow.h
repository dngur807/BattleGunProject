/*!
 * \file Bow.h
 * \date 2017/06/13 16:39
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

#ifndef Bow_h__
#define Bow_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;
	class CTexture;
	class CTransform;
	class CStaticMesh;
};

class CBow
	: public Engine::CGameObject
{
private:
	CBow(Engine::MYGDI* pDevice);

public:
	virtual ~CBow(void);

public:
	void SetMatrixInfo(const XMMATRIX& pPlayerInfo, const XMMATRIX& pWeaponRef, bool bSceneChange);

public:
	void SetWeight(float* pWeight) { m_pWeight = pWeight; }

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CBow* Create(Engine::MYGDI* pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CStaticMesh*	m_pMesh;
	Engine::CTransform*		m_pInfo;
	Engine::CTexture*		m_pTexture;

	XMFLOAT4X4		m_pmatWeaponRef;
	XMFLOAT4X4		m_pmatPlayerInfo;

	// 애니메이션 씬 전환 시 보간을 위한 변수들
	XMFLOAT4X4		m_matPreWorld;
	bool			m_bChangeScene;
	float*			m_pWeight;
};


#endif // Bow_h__