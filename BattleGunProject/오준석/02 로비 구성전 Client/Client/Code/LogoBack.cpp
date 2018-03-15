#include "stdafx.h"
#include "LogoBack.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"

CLogoBack::CLogoBack(Engine::MYGDI* pMyGDI)
	: Engine::CGameObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CLogoBack::~CLogoBack(void)
{

}

HRESULT CLogoBack::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	return S_OK;
}

void CLogoBack::Release(void)
{

}

int CLogoBack::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}

void CLogoBack::Render(void)
{
	//XMMATRIX matWorld;
	//matWorld = XMMatrixIdentity();

	//m_pMyGDI->pEffect->SetTexTransform(matWorld/*XMLoadFloat4x4(&m_pInfo->m_matWorld)*/);
	//m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	//m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	//m_pMyGDI->pEffect->SetWorld(matWorld);



	m_pTexture->Render(0, 0);
	m_pBuffer->Render(m_pInfo->m_matWorld);
} 

HRESULT CLogoBack::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_LOGO, L"Texture_LogoBack");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CLogoBack* CLogoBack::Create(Engine::MYGDI* pMyGDI)
{
	CLogoBack*		pGameObject = new CLogoBack(pMyGDI);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}