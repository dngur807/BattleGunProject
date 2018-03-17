#pragma once
#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CCubeTex;
	class CTexture;
	class CVIBuffer;
}
class CCube :
	public CLandObject
{
private:
	explicit CCube(Engine::MYGDI* pMyGDI);
public:
	virtual ~CCube();

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CCube* Create(Engine::MYGDI* pMyGDI);

private:
	Engine::CResourceMgr* m_pResourceMgr;
	Engine::CTexture* m_pTexture;
	//Engine::CCubeTex* m_pBuffer;
	Engine::CVIBuffer*		m_pBuffer;
	Material		m_tMtrl;
};

