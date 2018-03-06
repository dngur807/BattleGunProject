#include "stdafx.h"
#include "PlayerUI2.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Camera.h"
#include "Mouse.h"


//네비 UI
CPlayerUI2::CPlayerUI2(Engine::MYGDI* pMyGDI)
	: CUI(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
	, m_iTex(0)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CPlayerUI2::~CPlayerUI2(void)
{

}

HRESULT CPlayerUI2::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos = XMFLOAT3(-300.f, 250.f, 0.f);
	m_pInfo->m_vScale = XMFLOAT3(30.f, 120.f, 0.f);
	m_pInfo->m_fAngle[Engine::ANGLE_Z] = XMConvertToRadians(270);
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

void CPlayerUI2::Release(void)
{

}

int CPlayerUI2::Update(void)
{
	//CMouse::SetMousePos();
	Engine::CGameObject::Update();
	return 0;
}

void CPlayerUI2::Render(void)
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
	XMMATRIX P = XMMatrixOrthographicLH(800.f, 600.f, 0.f, 1.f);
	XMStoreFloat4x4(&mProj, P);


	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetUINaviTech();

	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	m_pMyGDI->pEffect->SetTexTransform(matWorld);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetWorld(matWorld);
	XMMATRIX wv, mvp;
	XMFLOAT4X4 Last;

	//
	XMMATRIX matScale, matRotX, matRotY, matRotZ, matTrans;
	XMFLOAT4X4			m_matWorld;

	// Navi
	matScale = XMMatrixScaling(50.f, 250.f, 1.f);
	matRotX = XMMatrixRotationX(0);
	matRotY = XMMatrixRotationY(0);
	matRotZ = XMMatrixRotationZ(XMConvertToRadians(270));
	matTrans = XMMatrixTranslation(0, 250.f, 0.f);
	matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
	XMStoreFloat4x4(&m_matWorld, matWorld);
	m_pTexture->Render(0, m_iTex);

	wv = XMMatrixMultiply(XMLoadFloat4x4(&m_matWorld), XMLoadFloat4x4(&mView));
	mvp = XMMatrixMultiply(wv, XMLoadFloat4x4(&mProj));
	XMStoreFloat4x4(&Last, mvp);

	m_pBuffer->Render(Last);
	//

	Engine::CTransform*	pRealPlayer = NULL;
	pRealPlayer = const_cast<Engine::CTransform*>(m_pTargetInfo);
	m_pMyGDI->pEffect->SetPlayerAngleY(XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y])*4 - 12.f);

	//cout << XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 -12.f << endl;

	if (XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f > -1.4f &&XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f < -0.4f)
		m_iTex = 2;
	else if (XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f > -2.4f && XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f < -1.4f)
		m_iTex = 3;
	else if (XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f > -3.4f && XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f < -2.4f)
		m_iTex = 0;
	else if (XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f> 0.6f &&XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f < 1.6f)
		m_iTex = 2;
	else if (XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f > 1.6f && XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f < 2.6f)
		m_iTex = 3;
	else if (XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f > 2.6f && XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f < 3.6f)
		m_iTex = 0;
	else if (XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f > -0.4f && XMConvertToRadians(pRealPlayer->m_fAngle[Engine::ANGLE_Y]) * 4 - 12.f < 0.6f)
		m_iTex = 1;
	else
		m_iTex = 0;


}

HRESULT CPlayerUI2::AddComponent(void)
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

CPlayerUI2* CPlayerUI2::Create(Engine::MYGDI* pMyGDI, Engine::CGameObject* pCamera, const Engine::CTransform* pGameObj)
{
	CPlayerUI2*		pGameObject = new CPlayerUI2(pMyGDI);
	pGameObject->m_pCamera = pCamera;
	pGameObject->m_pTargetInfo = pGameObj;
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}