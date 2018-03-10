#pragma once
#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CSkyBoxTex;
	class CTexture;
	class CVIBuffer;
}
class CSkyBox :
	public CLandObject
{
private:
	explicit CSkyBox(Engine::MYGDI* pMyGDI);
public:
	virtual ~CSkyBox();

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CSkyBox* Create(Engine::MYGDI* pMyGDI, int iSelect);

private:
	Engine::CResourceMgr* m_pResourceMgr;
	Engine::CTexture* m_pTexture;
	//Engine::CSkyBoxTex* m_pBuffer;
	Engine::CVIBuffer*		m_pBuffer;
	Material		m_tMtrl;
	DirectionalLight			m_DirLight;
	int m_iSelect;
	int m_iTextureNum;
};

