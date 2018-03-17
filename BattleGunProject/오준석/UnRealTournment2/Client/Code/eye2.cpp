#include "stdafx.h"
#include "Eye2.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"

CEye2::CEye2(Engine::MYGDI* pMyGDI)
	: Engine::CGameObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CEye2::~CEye2(void)
{

}

HRESULT CEye2::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	//m_pInfo->m_fAngle[Engine::ANGLE_Z] += 270.f;
	//m_pInfo->m_matWorld 

	m_pInfo->m_vPos = XMFLOAT3(0.04f, -0.18f, 0.f);
	m_pInfo->m_vScale = XMFLOAT3(0.01f, 0.02f, 1.f);
	m_pInfo->m_fAngle[Engine::ANGLE_Z] = XMConvertToRadians(270);
	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//빛초기화!
	m_DirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Direction = XMFLOAT3(1.f, -2.f, 1.f);

	D3D11_RASTERIZER_DESC dxRasterDesc;
	::memset(&dxRasterDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	//dxRasterDesc.FillMode = D3D11_FILL_WIREFRAME;       //와이어
	dxRasterDesc.FillMode = D3D11_FILL_SOLID;          //아닌거
	dxRasterDesc.CullMode = D3D11_CULL_NONE;
	dxRasterDesc.DepthClipEnable = TRUE;

	m_pMyGDI->pDevice->CreateRasterizerState(&dxRasterDesc, &m_pWire);
	m_pMyGDI->pContext->RSSetState(m_pWire);
	return S_OK;
}

void CEye2::Release(void)
{

}

int CEye2::Update(void)
{

	m_DirLight.Ambient.x += Engine::Get_TimeMgr()->DeltaTime()*0.5f;
	m_DirLight.Ambient.y += Engine::Get_TimeMgr()->DeltaTime()*0.5f;
	m_DirLight.Ambient.z += Engine::Get_TimeMgr()->DeltaTime()*0.5f;
	m_DirLight.Ambient.w += Engine::Get_TimeMgr()->DeltaTime()*0.5f;

	Engine::CGameObject::Update();

	return 0;
}

void CEye2::Render(void)
{

	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	m_pMyGDI->pEffect->SetTexTransform(matWorld/*XMLoadFloat4x4(&m_pInfo->m_matWorld)*/);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetWorld(matWorld);
	m_pTexture->Render(0, 0);
	m_pBuffer->Render(m_pInfo->m_matWorld);
}

HRESULT CEye2::AddComponent(void)
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
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_LOGO, L"Texture_Logo1");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CEye2* CEye2::Create(Engine::MYGDI* pMyGDI)
{
	CEye2*		pGameObject = new CEye2(pMyGDI);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}