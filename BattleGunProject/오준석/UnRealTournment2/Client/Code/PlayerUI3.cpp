#include "stdafx.h"
#include "PlayerUI3.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Camera.h"
#include "Mouse.h"
//아이템 창UI

CPlayerUI3::CPlayerUI3(Engine::MYGDI* pMyGDI)
	: CUI(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
	, m_bMenu(false)
	, m_iMenu(17)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CPlayerUI3::~CPlayerUI3(void)
{

}

HRESULT CPlayerUI3::Initialize(void)
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
	m_DirLight.Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.4f);
	m_DirLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Direction = XMFLOAT3(1.f, 0.f, 1.f);

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

void CPlayerUI3::Release(void)
{

}

int CPlayerUI3::Update(void)
{
	Engine::CGameObject::Update();

	if (m_bMenu == true)
	{
		if (m_DirLight.Ambient.x < 1.f)
		{
			m_DirLight.Ambient.x += Engine::Get_TimeMgr()->DeltaTime()*0.5f;
			m_DirLight.Ambient.y += Engine::Get_TimeMgr()->DeltaTime()*0.5f;
			m_DirLight.Ambient.z += Engine::Get_TimeMgr()->DeltaTime()*0.5f;
			m_DirLight.Ambient.w += Engine::Get_TimeMgr()->DeltaTime()*0.5f;
		}
	}
	else 
	{
		if (m_DirLight.Ambient.x >0.4f)
		m_DirLight.Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.4f);
	}





	static bool BUTON77 = false;
	if (Engine::Get_Input()->Get_DIKeyState(DIK_CAPSLOCK))
	{
		BUTON77 = true;
	}
	else
	{
		if (BUTON77)
		{
			//기능
			if (m_bMenu == true)
				m_bMenu = false;
			else
				m_bMenu = true;

			m_iMenu = 17;

	
			BUTON77 = false;
		}
	}

	static bool BUTON5 = false;
	if (Engine::Get_Input()->Get_DIKeyState(DIK_1)&& m_bMenu==true)
	{
		BUTON5 = true;
	}
	else
	{
		if (BUTON5)
		{
			//기능
			m_iMenu = 5;
			BUTON5 = false;
		}
	}

	static bool BUTON6 = false;
	if (Engine::Get_Input()->Get_DIKeyState(DIK_2) && m_bMenu == true)
	{
		BUTON6 = true;
	}
	else
	{
		if (BUTON6)
		{
			//기능
			m_iMenu = 6;
			BUTON6 = false;
		}
	}

	static bool BUTON7 = false;
	if (Engine::Get_Input()->Get_DIKeyState(DIK_3) && m_bMenu == true)
	{
		BUTON7 = true;
	}
	else
	{
		if (BUTON7)
		{
			//기능
			m_iMenu = 7;
			BUTON7 = false;
		}
	}


	static bool BUTON8 = false;
	if (Engine::Get_Input()->Get_DIKeyState(DIK_4) && m_bMenu == true)
	{
		BUTON8 = true;
	}
	else
	{
		if (BUTON8)
		{
			//기능
			m_iMenu = 8;
			BUTON8 = false;
		}
	}


	static bool BUTON9 = false;
	if (Engine::Get_Input()->Get_DIKeyState(DIK_5) && m_bMenu == true)
	{
		BUTON9 = true;
	}
	else
	{
		if (BUTON9)
		{
			//기능
			m_iMenu = 9;
			BUTON9 = false;
		}
	}


	return 0;
}

void CPlayerUI3::Render(void)
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


	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetUIWeapon();
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	m_pMyGDI->pEffect->SetTexTransform(matWorld);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetWorld(matWorld);

	XMMATRIX wv, mvp;
	XMFLOAT4X4 Last;
	XMMATRIX matScale, matRotX, matRotY, matRotZ, matTrans;
	XMFLOAT4X4			m_matWorld;


	//아이템여러개이미지 좌측하단
	matScale = XMMatrixScaling(100.f,200.f,1.f);
	matRotX = XMMatrixRotationX(0);
	matRotY = XMMatrixRotationY(0);
	matRotZ = XMMatrixRotationZ(XMConvertToRadians(270));
	matTrans = XMMatrixTranslation(-280.f,-230.f,0.f);
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


	// ItemImg
	/*matScale = XMMatrixScaling(100.f, 200.f, 1.f);
	matRotX = XMMatrixRotationX(0);
	matRotY = XMMatrixRotationY(0);
	matRotZ = XMMatrixRotationZ(XMConvertToRadians(270));
	matTrans = XMMatrixTranslation(-180.f, -130.f, 0.f);
	matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
	XMStoreFloat4x4(&m_matWorld, matWorld);
	m_pTexture->Render(0, 5);

	wv = XMMatrixMultiply(XMLoadFloat4x4(&m_matWorld), XMLoadFloat4x4(&mView));
	mvp = XMMatrixMultiply(wv, XMLoadFloat4x4(&mProj));
	XMStoreFloat4x4(&Last, mvp);
	m_pBuffer->Render(Last);*/
}

HRESULT CPlayerUI3::AddComponent(void)
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

CPlayerUI3* CPlayerUI3::Create(Engine::MYGDI* pMyGDI, Engine::CGameObject* pCamera, const Engine::CTransform* pGameObj)
{
	CPlayerUI3*		pGameObject = new CPlayerUI3(pMyGDI);
	pGameObject->m_pCamera = pCamera;
	pGameObject->m_pTargetInfo = pGameObj;
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}