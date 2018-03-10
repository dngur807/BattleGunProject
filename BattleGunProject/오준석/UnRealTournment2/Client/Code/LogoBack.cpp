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
	, m_fTime(0.f)
	, m_iTex(0)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CLogoBack::~CLogoBack(void)
{

}

HRESULT CLogoBack::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	//m_pInfo->m_fAngle[Engine::ANGLE_Z] += 270.f;
	//m_pInfo->m_matWorld 

	m_pInfo->m_vScale = XMFLOAT3(2.f, 2.f, 1.f);
	m_pInfo->m_fAngle[Engine::ANGLE_Z]= XMConvertToRadians(270);
	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//빛초기화!
	m_DirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 0.2f);
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

void CLogoBack::Release(void)
{

}

int CLogoBack::Update(void)
{
	m_fTime += Engine::Get_TimeMgr()->DeltaTime();
	if (m_fTime < 2.5f)
	{
		m_iTex = 0;
		if (m_DirLight.Ambient.x < 1.f)
		{
			m_DirLight.Ambient.x += Engine::Get_TimeMgr()->DeltaTime()*0.25f;
			m_DirLight.Ambient.y += Engine::Get_TimeMgr()->DeltaTime()*0.25f;
			m_DirLight.Ambient.z += Engine::Get_TimeMgr()->DeltaTime()*0.25f;
			m_DirLight.Ambient.w += Engine::Get_TimeMgr()->DeltaTime()*0.25f;

		}
	}
	else if (m_fTime < 5.f &&  m_fTime >= 2.5f)
	{
		m_iTex = 1;

	}
	else 
	{
		m_iTex = 5;

	}
	
	Engine::CGameObject::Update();

	return 0;
}

void CLogoBack::Render(void)
{
	// 재질 설정
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();


	m_pMyGDI->pEffect->SetTexTransform(matWorld/*XMLoadFloat4x4(&m_pInfo->m_matWorld)*/);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetWorld(matWorld);
	m_pTexture->Render(0, m_iTex);
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
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_LOGO, L"Texture_Logo");
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