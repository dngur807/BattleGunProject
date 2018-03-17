#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex :
	public CVIBuffer
{
private:
	explicit CCubeTex(MYGDI* pMyGDI);
public:
	virtual ~CCubeTex();

public:
	virtual HRESULT CreateBuffer(void);

public:
	virtual CResources* CloneResource(void);

public:
	static CCubeTex* Create(MYGDI* pMyGDI);
};

END

