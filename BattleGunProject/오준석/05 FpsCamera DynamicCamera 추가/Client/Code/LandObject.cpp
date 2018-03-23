#include "stdafx.h"
#include "LandObject.h"
#include "Transform.h"

CLandObject::CLandObject(Engine::MYGDI * pMyGDI)
	:CGameObject(pMyGDI),
	m_pInfo(nullptr),
	m_pObjectObserver(nullptr)
{
}

CLandObject::~CLandObject()
{
}

void CLandObject::SetPos(XMFLOAT3 vPos)
{
	if (m_pInfo)
		m_pInfo->m_vPos = vPos;
}

HRESULT CLandObject::Initialize()
{
	return S_OK;
}

void CLandObject::SetScale(XMFLOAT3 vScale)
{
	if (m_pInfo)
		m_pInfo->m_vScale = vScale;
}

int CLandObject::Update(void)
{
	CGameObject::Update();//컴포넌트 업데이트
	return 0;
}

void CLandObject::Render(void)
{

}
