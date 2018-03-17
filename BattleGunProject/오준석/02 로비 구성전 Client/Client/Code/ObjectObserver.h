#pragma once

#include "Observer.h"

namespace Engine
{
	class CTransform;
}

class CObjectObserver
	:public Engine::CObserver
{
private:
	CObjectObserver(void);
public:
	virtual ~CObjectObserver(void);

public:
	virtual int Update(int iMsg);

public:
	const list<Engine::CTransform*>* GetObjInfoList(void) { return m_pInfolist; }
	const Engine::CTransform* GetPlayerInfo(void) { return m_pPlayerInfo; }

public:
	static CObjectObserver* Create(void);

private:
	const list<Engine::CTransform*>* m_pInfolist;
	const Engine::CTransform*	m_pPlayerInfo;
};
