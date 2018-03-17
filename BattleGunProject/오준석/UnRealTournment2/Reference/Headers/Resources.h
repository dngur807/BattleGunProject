#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResources :
	public CComponent
{
protected:
	explicit CResources(MYGDI* pMyGDI);
	explicit CResources(const CResources& rhs);
public:
	virtual ~CResources();

public:
	virtual CResources* CloneResource(void) PURE;

public:
	virtual DWORD Release(void) PURE;

protected:
	MYGDI*	m_pMyGDI;
};

END