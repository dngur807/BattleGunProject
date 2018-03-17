#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollision :
	public CComponent
{
protected:
	explicit CCollision(MYGDI* pMyGDI);
public:
	virtual ~CCollision();

protected:
	DWORD Release(void);

public:
	virtual CCollision* Clone(void) PURE;

protected:
	MYGDI* m_pMyGDI;
};

END