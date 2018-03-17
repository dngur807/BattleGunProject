#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CComponent
{
protected:
	CComponent(void);
	CComponent(const CComponent& rhs);
public:
	virtual ~CComponent(void);

public:
	virtual int Update(void);
	virtual DWORD Release(void) PURE;

public:
	void AddRefCnt(void);

protected:
	DWORD m_dwRefCnt;
};

END