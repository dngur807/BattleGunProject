#include "stdafx.h"
#include "UI.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Camera.h"

CUI::CUI(Engine::MYGDI* pMyGDI)
	: Engine::CGameObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CUI::~CUI(void)
{

}

HRESULT CUI::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos = XMFLOAT3(-300.f, 250.f, 0.f);
	m_pInfo->m_vScale = XMFLOAT3(30.f, 120.f, 0.f);
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
	dxRasterDesc.DepthClipEnable = false;

	m_pMyGDI->pDevice->CreateRasterizerState(&dxRasterDesc, &m_pWire);
	m_pMyGDI->pContext->RSSetState(m_pWire);
	return S_OK;
}

void CUI::Release(void)
{

}

int CUI::Update(void)
{
	Engine::CGameObject::Update();
	return 0;
}

void CUI::Render(void)
{
	//2차원 UI만드는 코드
	XMFLOAT4X4 mView, mProj;
	XMFLOAT4X4 mViewOld, mProjOld;
	XMMATRIX I = XMMatrixIdentity();

	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	mViewOld = dynamic_cast<Engine::CCamera*>(m_pCamera)->GetView();
	XMMATRIX PP = XMLoadFloat4x4(&dynamic_cast<Engine::CCamera*>(m_pCamera)->GetProj());
	XMStoreFloat4x4(&mProjOld, PP);


	//mView = dynamic_cast<Engine::CCamera*>(m_pCamera)->GetView();
	XMMATRIX P = XMMatrixOrthographicLH(800.f, 600.f, 0.f, 1.f);
	XMStoreFloat4x4(&mProj, P);


	//m_pInfo->m_matWorld._11 *= 20;
	//m_pInfo->m_matWorld._22 *= 20;

	//dynamic_cast<Engine::CCamera*>(m_pCamera)->Invalidate_ViewMatrix();
	//dynamic_cast<Engine::CCamera*>(m_pCamera)->Invalidate_ProjMatrix();


	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight0Tech();
	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetUITech();
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	m_pMyGDI->pEffect->SetTexTransform(matWorld);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetWorld(matWorld);
	//m_pTexture->Render(0, 18);

	XMMATRIX wv, mvp;
	XMFLOAT4X4 Last;

	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(matWorld);

	wv = XMMatrixMultiply(XMLoadFloat4x4(&m_pInfo->m_matWorld), XMLoadFloat4x4(&mView));
	//wv = XMMatrixMultiply(worldInvTranspose, XMLoadFloat4x4(&mView));
	mvp = XMMatrixMultiply(wv, XMLoadFloat4x4(&mProj));
	XMStoreFloat4x4(&Last, mvp);
	//XMStoreFloat4x4(&Last, worldInvTranspose);
	m_pBuffer->Render(Last);
	//m_pBuffer->Render(m_pInfo->m_matWorld);



}

HRESULT CUI::AddComponent(void)
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
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_UI1");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CUI* CUI::Create(Engine::MYGDI* pMyGDI, Engine::CGameObject* pCamera)
{
	CUI*		pGameObject = new CUI(pMyGDI);
	pGameObject->m_pCamera = pCamera;
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}