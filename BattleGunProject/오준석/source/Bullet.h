#pragma once
#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CVIBuffer;
	class CTexture;

}
class CBolt :
	public CLandObject
{
private:
	explicit CBolt(Engine::MYGDI* pMyGDI);
public:
	virtual ~CBolt();

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CBolt* Create(Engine::MYGDI* pMyGDI);

private:
	Engine::CResourceMgr* m_pResourceMgr;
	Engine::CTexture* m_pTexture;
	Engine::CVIBuffer* m_pBuffer;
	UINT mBoltFrameIndex = 0.f;
};

