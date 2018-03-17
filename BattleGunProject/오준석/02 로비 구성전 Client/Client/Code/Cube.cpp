#include "stdafx.h"
#include "Cube.h"

#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Transform.h"
#include "CubeTex.h"

CCube::CCube(Engine::MYGDI * pMyGDI)
	:CLandObject(pMyGDI), 
	m_pResourceMgr(Engine::Get_ResourceMgr()), 
	m_pTexture(nullptr)
{
}

CCube::~CCube()
{
}

int CCube::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}

void CCube::Render(void)
{
	m_pTexture->Render(0, 0);
	m_pBuffer->Render(m_pInfo->m_matWorld);
}

HRESULT CCube::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos = XMFLOAT3(10.f, 0.f, 10.f);
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = XMConvertToRadians(30);

	return S_OK;
}

HRESULT CCube::AddComponent(void)
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Cube");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CCubeTex*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"CubeTex", pComponent));

	return S_OK;
}

CCube * CCube::Create(Engine::MYGDI * pMyGDI)
{
	CCube* pLandObject = new CCube(pMyGDI);
	if (FAILED(pLandObject->Initialize()))
		Engine::Safe_Delete(pLandObject);

	return pLandObject;
}
