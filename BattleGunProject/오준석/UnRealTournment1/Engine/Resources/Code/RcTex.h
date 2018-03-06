#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex :
	public CVIBuffer
{
private:
	explicit CRcTex(MYGDI* pMyGDI);
	//explicit CRcTex(const CRcTex& rhs);
public:
	virtual ~CRcTex();

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(TCHAR* szFullPath = L"");

public:
	virtual void Render(void);

public:
	static CRcTex* Create(MYGDI* pMyGDI);

private:
	ID3D11ShaderResourceView* m_pDiffuseMapSRV;

private:
};

END