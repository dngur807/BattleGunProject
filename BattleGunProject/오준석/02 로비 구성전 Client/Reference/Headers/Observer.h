#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CObserver
{
protected:
	CObserver();
public:
	virtual ~CObserver();

public:
	virtual int Update(int iMsg) PURE;
};

END