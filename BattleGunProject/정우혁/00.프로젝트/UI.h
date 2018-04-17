#pragma once

#include "GameObject.h"

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
	virtual ~CUI();
public:
	virtual HRESULT Initialize();
	virtual int Update();
	virtual void Render();
public:
	static CUI* Create(Engine::MYGDI* pMyGDI);
protected:
	void    SetUV(Engine::CVIBuffer* pBuffer, float x1, float y1, float x2, float y2, float SizeX, float SizeY);

protected:
	Engine::CResourceMgr*		m_pResourceMgr;
	float				m_fX, m_fY;
	float				m_fSizeX, m_fSizeY;
	XMFLOAT4X4			m_matView, m_matProj, m_matWorld;
	Engine::VTXTEX*				m_pVtxTex;

};