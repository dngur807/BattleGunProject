#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSkyBoxTex :
	public CVIBuffer
{
private:
	explicit CSkyBoxTex(MYGDI* pMyGDI);
public:
	virtual ~CSkyBoxTex();

public:
	virtual HRESULT CreateBuffer(void);

public:
	virtual CResources* CloneResource(void);

public:
	static CSkyBoxTex* Create(MYGDI* pMyGDI);
};

END

