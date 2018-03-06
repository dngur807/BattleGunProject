#include "stdafx.h"
#include "LandObject.h"

CLandObject::CLandObject(Engine::MYGDI * pMyGDI)
	:CGameObject(pMyGDI),
	m_pInfo(nullptr),
	m_pObjectObserver(nullptr),
	m_fSpeed(0.f),
	fTime(0.f)
{
}

CLandObject::~CLandObject()
{

}
