#include "stdafx.h"
#include "LandObject.h"

CLandObject::CLandObject(Engine::MYGDI * pMyGDI)
	:CGameObject(pMyGDI),
	m_pInfo(nullptr),
	m_pObjectObserver(nullptr)
{
}

CLandObject::~CLandObject()
{
}
