#pragma once
#include "Resources.h"

BEGIN(Engine)

class CBasicEffect;

class ENGINE_DLL CTexture :
	public CResources
{
private:
	explicit CTexture(MYGDI* pMyGDI);
public:
	virtual ~CTexture(void);

private:
	DWORD Release(void);

public:
	void Render(const DWORD& dwStage, const DWORD& iIndex);
	ID3D11ShaderResourceView** GetTextureView(const DWORD& iIndex);

public:
	virtual CResources* CloneResource(void);
	HRESULT LoadTexture(TEXTURETYPE eTextureType, const std::wstring& wstrFilePath,
		const WORD& wCnt);

public:
	static CTexture* Create(MYGDI* pMyGDI, TEXTURETYPE eTextureType,
		const std::wstring& wstrFilePath, const WORD& wCnt);

private:
	typedef std::vector<ID3D11ShaderResourceView*> VECSRV;
	VECSRV	m_vecSRV;

private:
	DWORD	m_dwContainerSize;

private:
	CBasicEffect*	m_pEffect;
};

END