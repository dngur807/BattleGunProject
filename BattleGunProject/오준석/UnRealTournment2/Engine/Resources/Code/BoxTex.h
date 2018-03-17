#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CBoxTex :
	public CVIBuffer
{
private:
	explicit CBoxTex(MYGDI* pMyGDI);
public:
	virtual ~CBoxTex();

public:
	virtual HRESULT CreateBuffer(void);

public:
	virtual CResources* CloneResource(void);

public:
	static CBoxTex* Create(MYGDI* pMyGDI);
};

END

