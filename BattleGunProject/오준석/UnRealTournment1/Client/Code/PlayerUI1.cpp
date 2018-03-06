#include "stdafx.h"
#include "PlayerUI1.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Camera.h"
#include "Mouse.h"

//hp,exp,전체메뉴 등등
CPlayerUI1::CPlayerUI1(Engine::MYGDI* pMyGDI)
	: CUI(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
	, m_bMenu(false)
	, m_iMenu(10)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CPlayerUI1::~CPlayerUI1(void)
{

}

HRESULT CPlayerUI1::Initialize(void)
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

void CPlayerUI1::Release(void)
{

}

int CPlayerUI1::Update(void)
{
	CMouse::SetMousePos();
	Engine::CGameObject::Update();

	//cout<<CMouse::GetMousePos().x << "\t" << CMouse::GetMousePos().y << endl;
	

	if (m_bMenu == true)
	{
		if (CMouse::GetMousePos().x > 150 && CMouse::GetMousePos().x << 195 &&
			CMouse::GetMousePos().y > 450 && CMouse::GetMousePos().y < 520)
		{
			m_iMenu = 11;
		}
		if (CMouse::GetMousePos().x > 200 && CMouse::GetMousePos().x << 250 &&
			CMouse::GetMousePos().y > 450 && CMouse::GetMousePos().y < 520)
		{
			m_iMenu = 12;
		}
		if (CMouse::GetMousePos().x > 260 && CMouse::GetMousePos().x << 300 &&
			CMouse::GetMousePos().y > 450 && CMouse::GetMousePos().y < 520)
		{
			m_iMenu = 13;
		}
		if (CMouse::GetMousePos().x > 310 && CMouse::GetMousePos().x << 360 &&
			CMouse::GetMousePos().y > 450 && CMouse::GetMousePos().y < 520)
		{
			m_iMenu = 14;
		}
		if (CMouse::GetMousePos().x > 370 && CMouse::GetMousePos().x << 410 &&
			CMouse::GetMousePos().y > 450 && CMouse::GetMousePos().y < 520)
		{
			m_iMenu = 15;
		}
		if (CMouse::GetMousePos().x > 425 && CMouse::GetMousePos().x << 470 &&
			CMouse::GetMousePos().y > 450 && CMouse::GetMousePos().y < 520)
		{
			m_iMenu = 16;
			if (GetAsyncKeyState(VK_LBUTTON) && m_bMenu==true)
				m_bMenu = false;
		}
	

	}

	

	static bool BUTON7 = false;
	if (Engine::Get_Input()->GetDIKeyState(DIK_TAB))
	{
		BUTON7 = true;
	}
	else
	{
		if (BUTON7)
		{
			//기능
			if (m_bMenu == true)
				m_bMenu = false;
			else
				m_bMenu = true;

			m_iMenu = 10;
			BUTON7 = false;
		}
	}
	//cout << m_iMenu << endl;

	return 0;
}

void CPlayerUI1::Render(void)
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


	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetUITech();

	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	m_pMyGDI->pEffect->SetTexTransform(matWorld);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	//m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetWorld(matWorld);
	//m_pTexture->Render(0, 18);

	XMMATRIX wv, mvp;
	XMFLOAT4X4 Last;
	wv = XMMatrixMultiply(XMLoadFloat4x4(&m_pInfo->m_matWorld), XMLoadFloat4x4(&mView));
	mvp = XMMatrixMultiply(wv, XMLoadFloat4x4(&mProj));
	XMStoreFloat4x4(&Last, mvp);
	//m_pBuffer->Render(Last);
	//
	XMMATRIX matScale, matRotX, matRotY, matRotZ, matTrans;
	XMFLOAT4X4			m_matWorld;

	// 전체메뉴
	matScale = XMMatrixScaling(600.f, 800.f, 1.f);
	matRotX = XMMatrixRotationX(0);
	matRotY = XMMatrixRotationY(0);
	matRotZ = XMMatrixRotationZ(XMConvertToRadians(270));
	matTrans = XMMatrixTranslation(0.f, 0.f, 0.0f);
	matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;


	XMStoreFloat4x4(&m_matWorld, matWorld);
	wv = XMMatrixMultiply(XMLoadFloat4x4(&m_matWorld), XMLoadFloat4x4(&mView));
	mvp = XMMatrixMultiply(wv, XMLoadFloat4x4(&mProj));
	XMStoreFloat4x4(&Last, mvp);
	if (m_bMenu == true)
	{
		m_pTexture->Render(0, m_iMenu);
		m_pBuffer->Render(Last);
	}

	//아이템여러개이미지 좌측하단
	/*matScale = XMMatrixScaling(100.f,200.f,1.f);
	matRotX = XMMatrixRotationX(0);
	matRotY = XMMatrixRotationY(0);
	matRotZ = XMMatrixRotationZ(XMConvertToRadians(270));
	matTrans = XMMatrixTranslation(-280.f,-230.f,0.f);
	matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
	XMStoreFloat4x4(&m_matWorld, matWorld);
	m_pTexture->Render(0, 17);

	wv = XMMatrixMultiply(XMLoadFloat4x4(&m_matWorld), XMLoadFloat4x4(&mView));
	mvp = XMMatrixMultiply(wv, XMLoadFloat4x4(&mProj));
	XMStoreFloat4x4(&Last, mvp);
	m_pBuffer->Render(Last);*/

	// ItemImg
	//matScale = XMMatrixScaling(100.f, 200.f, 1.f);
	//matRotX = XMMatrixRotationX(0);
	//matRotY = XMMatrixRotationY(0);
	//matRotZ = XMMatrixRotationZ(XMConvertToRadians(270));
	//matTrans = XMMatrixTranslation(-180.f, -130.f, 0.f);
	//matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
	//XMStoreFloat4x4(&m_matWorld, matWorld);
	//m_pTexture->Render(0, 5);

	//wv = XMMatrixMultiply(XMLoadFloat4x4(&m_matWorld), XMLoadFloat4x4(&mView));
	//mvp = XMMatrixMultiply(wv, XMLoadFloat4x4(&mProj));
	//XMStoreFloat4x4(&Last, mvp);
	//m_pBuffer->Render(Last);

	// Navi
	/*matScale = XMMatrixScaling(50.f, 1000.f, 1.f);
	matRotX = XMMatrixRotationX(0);
	matRotY = XMMatrixRotationY(0);
	matRotZ = XMMatrixRotationZ(XMConvertToRadians(270));
	matTrans = XMMatrixTranslation(0, 250.f, 0.f);
	matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
	XMStoreFloat4x4(&m_matWorld, matWorld);
	m_pTexture->Render(0, 4);
	wv = XMMatrixMultiply(XMLoadFloat4x4(&m_matWorld), XMLoadFloat4x4(&mView));
	mvp = XMMatrixMultiply(wv, XMLoadFloat4x4(&mProj));
	XMStoreFloat4x4(&Last, mvp);
	m_pBuffer->Render(Last);*/

	//HP
	Engine::CTransform*	pRealPlayer = NULL;
	pRealPlayer = const_cast<Engine::CTransform*>(m_pTargetInfo);
	//pRealPlayer->m_vDir
	for (int i = 0; i < pRealPlayer->m_tPlayer.iHp / 12; ++i)
	{
		matScale = XMMatrixScaling(25.f, 6.f, 1.f);
		matRotX = XMMatrixRotationX(0);
		matRotY = XMMatrixRotationY(0);
		matRotZ = XMMatrixRotationZ(XMConvertToRadians(270));
		matTrans = XMMatrixTranslation(-353.f+i*7.5f, 249.f, -0.0f);
		matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
		XMStoreFloat4x4(&m_matWorld, matWorld);
		m_pTexture->Render(0, 19);
		wv = XMMatrixMultiply(XMLoadFloat4x4(&m_matWorld), XMLoadFloat4x4(&mView));
		mvp = XMMatrixMultiply(wv, XMLoadFloat4x4(&mProj));
		XMStoreFloat4x4(&Last, mvp);
		m_pBuffer->Render(Last);

	}
	//EXP
	for (int i = 0; i < pRealPlayer->m_tPlayer.iExp / 12; ++i)
	{
		matScale = XMMatrixScaling(25.f, 6.f, 1.f);
		matRotX = XMMatrixRotationX(0);
		matRotY = XMMatrixRotationY(0);
		matRotZ = XMMatrixRotationZ(XMConvertToRadians(270));
		matTrans = XMMatrixTranslation(-353.f + i*7.5f, 252.f, -0.0f);
		matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
		XMStoreFloat4x4(&m_matWorld, matWorld);
		m_pTexture->Render(0, 20);
		wv = XMMatrixMultiply(XMLoadFloat4x4(&m_matWorld), XMLoadFloat4x4(&mView));
		mvp = XMMatrixMultiply(wv, XMLoadFloat4x4(&mProj));
		XMStoreFloat4x4(&Last, mvp);
		m_pBuffer->Render(Last);

	}
	//HP BAR배경
	matScale = XMMatrixScaling(30.f, 120.f, 1.f);
	matRotX = XMMatrixRotationX(0);
	matRotY = XMMatrixRotationY(0);
	matRotZ = XMMatrixRotationZ(XMConvertToRadians(270));
	matTrans = XMMatrixTranslation(-300.f, 250.f, -0.0f);
	matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
	XMStoreFloat4x4(&m_matWorld, matWorld);
	m_pTexture->Render(0, 18);

	wv = XMMatrixMultiply(XMLoadFloat4x4(&m_matWorld), XMLoadFloat4x4(&mView));
	mvp = XMMatrixMultiply(wv, XMLoadFloat4x4(&mProj));
	XMStoreFloat4x4(&Last, mvp);
	m_pBuffer->Render(Last);


}

HRESULT CPlayerUI1::AddComponent(void)
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

CPlayerUI1* CPlayerUI1::Create(Engine::MYGDI* pMyGDI, Engine::CGameObject* pCamera, const Engine::CTransform* pGameObj)
{
	CPlayerUI1*		pGameObject = new CPlayerUI1(pMyGDI);
	pGameObject->m_pCamera = pCamera;
	pGameObject->m_pTargetInfo = pGameObj;
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}