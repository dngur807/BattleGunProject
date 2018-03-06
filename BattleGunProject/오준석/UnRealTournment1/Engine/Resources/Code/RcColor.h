#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcColor :
	public CVIBuffer
{
private:
	explicit CRcColor(MYGDI* pMyGDI);
public:
	virtual ~CRcColor();

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	static CRcColor* Create(MYGDI* pMyGDI);
};

END	