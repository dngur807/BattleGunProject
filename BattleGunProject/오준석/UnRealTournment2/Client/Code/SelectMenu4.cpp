#include "stdafx.h"
#include "SelectMenu4.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Mouse.h"
CSelectMenu4::CSelectMenu4(Engine::MYGDI* pMyGDI)
	: Engine::CGameObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
	, m_fTime(0.f)
	, m_bRight(false)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CSelectMenu4::~CSelectMenu4(void)
{

}

HRESULT CSelectMenu4::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	//m_pInfo->m_fAngle[Engine::ANGLE_Z] += 270.f;
	//m_pInfo->m_matWorld 

	
	m_pInfo->m_vPos = XMFLOAT3(0.f, 0.05f, 0.f);
	m_pInfo->m_vScale = XMFLOAT3(0.2f, 0.6f, 0.0f);
	//m_pInfo->m_vScale = XMFLOAT3(1.f, 1.f, 1.f);
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
	//dxRasterDesc.DepthClipEnable = TRUE;

	m_pMyGDI->pDevice->CreateRasterizerState(&dxRasterDesc, &m_pWire);
	m_pMyGDI->pContext->RSSetState(m_pWire);
	return S_OK;
}

void CSelectMenu4::Release(void)
{

}

int CSelectMenu4::Update(void)
{
	if (CMouse::GetMousePos().x >= 310 && CMouse::GetMousePos().x <= 480 &&
		CMouse::GetMousePos().y >= 280 && CMouse::GetMousePos().y <= 310)
		m_bMouseOn = true;
	else
		m_bMouseOn = false;

	m_fTime += Engine::Get_TimeMgr()->DeltaTime();


	if (m_bMouseOn == true)
	{
		if (m_DirLight.Ambient.x > 1.f && m_bRight == false)
			m_bRight = true;
		if (m_DirLight.Ambient.x < 0.2f && m_bRight == true)
			m_bRight = false;
		if (m_bRight)
		{
			m_DirLight.Ambient.x -= Engine::Get_TimeMgr()->DeltaTime()*0.5f;
			m_DirLight.Ambient.y -= Engine::Get_TimeMgr()->DeltaTime()*0.5f;
			m_DirLight.Ambient.z -= Engine::Get_TimeMgr()->DeltaTime()*0.5f;
			m_DirLight.Ambient.w -= Engine::Get_TimeMgr()->DeltaTime()*0.5f;

		}
		else
		{
			m_DirLight.Ambient.x += Engine::Get_TimeMgr()->DeltaTime()*0.5f;
			m_DirLight.Ambient.y += Engine::Get_TimeMgr()->DeltaTime()*0.5f;
			m_DirLight.Ambient.z += Engine::Get_TimeMgr()->DeltaTime()*0.5f;
			m_DirLight.Ambient.w += Engine::Get_TimeMgr()->DeltaTime()*0.5f;

		}
		XMVECTOR Temp = XMLoadFloat3(&m_pInfo->m_vScale);
		XMVECTOR Temp1 = XMVectorSet(1.3f, 1.3f, 2.f, 2.f);
		if (m_pInfo->m_vScale.x < 0.22f)
			XMStoreFloat3(&m_pInfo->m_vScale, XMVectorMultiply(Temp, Temp1));
	}
	else
	{
		m_pInfo->m_vScale = XMFLOAT3(0.2f, 0.6f, 0.0f);
		m_DirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	}
	Engine::CGameObject::Update();
	return 0;
}

void CSelectMenu4::Render(void)
{
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	m_pMyGDI->pEffect->SetTexTransform(matWorld);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	//m_pMyGDI->pEffect->SetWorld(matWorld);

	//if (m_fTime > 5.f)
	{
		m_pTexture->Render(0, 4);
		m_pBuffer->Render(m_pInfo->m_matWorld);

	}
}

HRESULT CSelectMenu4::AddComponent(void)
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
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_SELECT, L"Texture_Select1");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CSelectMenu4* CSelectMenu4::Create(Engine::MYGDI* pMyGDI)
{
	CSelectMenu4*		pGameObject = new CSelectMenu4(pMyGDI);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}