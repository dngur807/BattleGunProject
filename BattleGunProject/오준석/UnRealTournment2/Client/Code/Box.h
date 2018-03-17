#pragma once
#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CBoxTex;
	class CTexture;
	class CVIBuffer;
}
class CBox :
	public CLandObject
{
private:
	explicit CBox(Engine::MYGDI* pMyGDI);
public:
	virtual ~CBox();

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CBox* Create(Engine::MYGDI* pMyGDI);

private:
	Engine::CResourceMgr* m_pResourceMgr;
	Engine::CTexture* m_pTexture;
	//Engine::CBoxTex* m_pBuffer;
	Engine::CVIBuffer*		m_pBuffer;
};

