#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CObserver;

class ENGINE_DLL CSubject abstract
{
protected:
	CSubject();
	virtual ~CSubject();

private:
	void Release(void);

public:
	virtual void Subscribe(CObserver* pObserver);
	virtual void UnSubscribe(CObserver* pObserver);
	virtual void Notify(int iMsg);

protected:
	typedef list<CObserver*>	OBSERVERLIST;
	OBSERVERLIST	m_Observerlist;
};

END