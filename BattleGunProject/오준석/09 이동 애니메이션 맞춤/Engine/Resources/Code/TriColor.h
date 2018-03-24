#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriColor :
	public CVIBuffer
{
private:
	explicit CTriColor(MYGDI* pMyGDI);
public:
	virtual ~CTriColor();

public:
	virtual void Render(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	static CTriColor* Create(MYGDI* pMyGDI);
};

END