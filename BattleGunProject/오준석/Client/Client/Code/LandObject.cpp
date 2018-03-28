#include "stdafx.h"
#include "LandObject.h"
#include "Transform.h"

CLandObject::CLandObject(Engine::MYGDI * pMyGDI)
	:CGameObject(pMyGDI),
	m_pInfo(nullptr),
	m_pObjectObserver(nullptr)
{
	m_DirLight.Ambient = XMFLOAT4(1, 1, 1, 1);
	m_DirLight.Diffuse = XMFLOAT4(1, 1, 1, 1);
	m_DirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Direction = XMFLOAT3(1.f, -2.f, 1.f);

	m_tMtrl.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

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
